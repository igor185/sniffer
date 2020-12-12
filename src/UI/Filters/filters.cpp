#include "filters.h"
#include "ProxyModel.h"
#include "QPushButton"
#include "QLabel"
#include <QCloseEvent>


FiltersView::FiltersView(ProxyModel* proxy, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FiltersView)
{
    ui->setupUi(this);

//    auto* btn = new QPushButton;
//    btn->setIcon(QPixmap(":images/ok.png"));
//
//    connect(btn, SIGNAL (released()), this, SLOT (handleButton()));
//
//    ui->verticalLayout_2->addWidget(btn);

    for(Filter* filter: proxy->getFilters()){
        ui->verticalLayout_2->addWidget(filter->draw());
    }


    bar->addAction(andIcon);
    bar->addAction(orIcon);

    addToolBar(bar);
}

void FiltersView::add(QWidget*layout) {
    ui->verticalLayout_2->addWidget(layout);
}
FiltersView::~FiltersView()
{
    delete ui;
}

void FiltersView::handleButton() { // TODO
//    new Filter(this);
}

Filter::Filter() {
    type = static_cast<Types>(Types::Protocol + 1);

    comboBox = new QComboBox(this);
    comboBox->setObjectName(QString::fromUtf8("comboBox"));
    comboBox->addItem("Source");
    comboBox->addItem("Destination");
    comboBox->addItem("Length");
    comboBox->addItem("Protocol");
    comboBox->addItem("Info");
    comboBox->setCurrentIndex(3);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_textChanged(const QString &)));
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_edit(int)));
}

void Filter::draw(QHBoxLayout *layout) {
    layout->addWidget(comboBox);
    layout->addWidget(lineEdit);
}

bool Filter::filter(const QModelIndex &index) const {
    QRegExp regExp(value, Qt::CaseInsensitive, QRegExp::Wildcard);
    return index.data().toString().contains(regExp);
}

void Filter::on_lineEdit_textChanged(const QString &new_value) {
    value = new_value;
    emit changed();
}

void Filter::on_comboBox_edit(int new_type) {
    type = static_cast<Types>(new_type + 1);
    value = "";
    lineEdit->clear();
    lineEdit->setFocus();
    emit changed();
}

QWidget *Filter::draw() {
    auto layout_h = new QHBoxLayout(this);
    layout_h->addWidget(comboBox);
    layout_h->addWidget(lineEdit);

    auto *w = new QWidget;
    w->setLayout(layout_h);
    return w;
}


