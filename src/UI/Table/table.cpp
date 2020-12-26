// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <QMainWindow>

#include "IO/IO.h"
#include "Table.h"
#include "../Details/details.h"
#include "UI/QHexView.h"
#include "../Filters/ProxyModel.h"


MainWindow::MainWindow(core::config &config1, QWidget *parent) // TODO rename it
        : QMainWindow(parent), config(config1), ui(new Ui::Table) {
    ui->setupUi(this);

    proxy_model = new ProxyModel();

    Filter* filter = proxy_model->getFirst();

    filter->draw(ui->horizontalLayout);

    ui->tableView->verticalHeader()->setVisible(false);

    model = new QStandardItemModel(0, 5, ui->tableView);



    ui->tableView->setModel(proxy_model);
    proxy_model->setSourceModel(model);
    proxy_model->setFilterKeyColumn(4);


    model->setHeaderData(0, Qt::Horizontal, tr("No"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Source"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Destination"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Length"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, tr("Protocol"), Qt::DisplayRole);
//    model->setHeaderData(5, Qt::Horizontal, tr("Info"), Qt::DisplayRole);

    connect(ui->tableView->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(addItem()));
    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_doubleClick(const QModelIndex &)));

    bar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

    ok->setObjectName("continue");
    stop->setObjectName("stop");
    scroll->setObjectName("scroll");
    setting->setObjectName("settings");

    bar->widgetForAction(ok)->setObjectName(ok->objectName());
    bar->widgetForAction(stop)->setObjectName(stop->objectName());
    bar->widgetForAction(scroll)->setObjectName(scroll->objectName());

    stop->setVisible(false);

    if (isScroll) {
        bar->setStyleSheet(
                "QToolButton#scroll { background:#D5F1FA }");
    }

    addToolBar(bar);

    connect(ok, SIGNAL(triggered(bool)), this, SLOT(pause(bool)));
    connect(stop, SIGNAL(triggered(bool)), this, SLOT(start(bool)));
    connect(scroll, SIGNAL(triggered(bool)), this, SLOT(press_scroll(bool)));
    connect(setting, SIGNAL(triggered(bool)), this, SLOT(settings_open(bool)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::add_to_table(sockets::base_socket *socket) {
    if (!config.captured) {
        config.captured = true;
    }
    auto row = socket->to_row();
    mutex.lock();
    QList<QStandardItem *> items;

    items.append(new QStandardItem(QString("%2").arg(model->rowCount() + 1)));
    items.append(new QStandardItem(row.source.c_str()));
    items.append(new QStandardItem(row.destination.c_str()));
    items.append(new QStandardItem(QString("%1").arg(row.size)));
    items.append(new QStandardItem(row.protocol.c_str()));
//    items.append(new QStandardItem(row.info.c_str()));

    model->appendRow(items);
    data.push_back(socket);
    mutex.unlock();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
    proxy_model->onChangeText(arg1);
}

void MainWindow::on_comboBox_edit(int index) {
    proxy_model->onChangeField(index);
}

void MainWindow::on_doubleClick(const QModelIndex &index) {
    auto *socket = data[index.sibling(index.row(), 0).data().value<size_t>() - 1];
    auto details_view = socket->to_view();

    QByteArray array((const char *)socket->packet, socket->pkt_hdr->len);

    auto *phexView = new QHexView;

    phexView->setData(new QHexView::DataStorageArray(array));

    auto* details = new Details(phexView, details_view, this);

    details->show();
}

void
MainWindow::get_proxying_data(std::vector<pcap_pkthdr> &pkt_hdr, std::vector<const u_char *> &packet) {
    mutex.lock();
    pkt_hdr.reserve(proxy_model->rowCount());
    packet.reserve(proxy_model->rowCount());
    for (size_t i = 0; i < proxy_model->rowCount(); i++) {
        auto *elem = data[proxy_model->index(i, 0).data().value<size_t>() - 1];
        pkt_hdr.push_back({elem->pkt_hdr->ts, elem->len, elem->caplen});
        packet.push_back(elem->packet);
    }
}

void MainWindow::pause(bool state) {
    emit pause_sniffer();
    ok->setVisible(false);
    stop->setVisible(true);
}

void MainWindow::start(bool state) {
    emit continue_sniffer();
    ok->setVisible(true);
    stop->setVisible(false);
}

void MainWindow::addItem() {
    if (isScroll) {
        emit ui->tableView->scrollToBottom();
    }
}

void MainWindow::press_scroll(bool state) {
    isScroll = !isScroll;
    if (isScroll) {
        bar->setStyleSheet(
                "QToolButton#scroll { background:#D5F1FA }");
    } else {
        bar->setStyleSheet(
                "QToolButton#scroll { background:none}");
    }
}

void MainWindow::settings_open(bool state) {

    filters = new FiltersView(proxy_model, this);

    filters->show();

}
