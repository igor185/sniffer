#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QComboBox>
#include <QMutex>
#include <vector>
#include <QToolBar>

#include "core/core.h"
#include "../Filters/ProxyModel.h"
#include "../Filters/filters.h"
#include "ui_table.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Table; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(config& config1, QWidget *parent = nullptr);
    ~MainWindow() override;

    void add_to_table(sockets::base_socket* socket);

    void get_proxying_data(std::vector<pcap_pkthdr>& pkt_hdr, std::vector<const u_char *>& packet);

    QMutex mutex;
private:
    config& configs;
    std::vector<sockets::base_socket *> data;
    Ui::Table *ui;
    ProxyModel* proxy_model;
    QStandardItemModel* model;
//    QComboBox* comboBox;
//    QLineEdit* lineEdit;
    QToolBar* bar = new QToolBar;
    QAction* ok = bar->addAction(QPixmap(":images/ok.png"), "continue");
    QAction* stop = bar->addAction(QPixmap(":images/stop.png"), "stop");
    QAction* scroll = bar->addAction(QPixmap(":images/scroll.png"), "scroll");
    QAction* setting = bar->addAction(QPixmap(":images/settings.png"), "settings");
    std::atomic<bool> isScroll = true;
    FiltersView* filters;

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_comboBox_edit(int index);
    void on_doubleClick(const QModelIndex& index);
    void pause(bool state);
    void start(bool state);
    void press_scroll(bool state);
    void addItem();
    void settings_open(bool);

signals:
    void pause_sniffer();
    void continue_sniffer();
};
#endif // MAINWINDOW_H

