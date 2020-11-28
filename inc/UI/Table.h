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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(core::config& config1, QWidget *parent = nullptr);
    ~MainWindow() override;

    void add_to_table(sockets::base_socket* socket);

    void get_proxying_data(std::vector<const struct pcap_pkthdr *>& pkt_hdr, std::vector<const u_char *>& packet);

    QMutex mutex;
private:
    core::config& config;
    std::vector<sockets::base_socket *> data;
    Ui::MainWindow *ui;
    QSortFilterProxyModel* proxy_model;
    QStandardItemModel* model;
    QComboBox* comboBox;
    QLineEdit* lineEdit;
    QToolBar* bar = new QToolBar;
    QAction* ok = bar->addAction(QPixmap(":images/ok.png"), "continue");
    QAction* stop = bar->addAction(QPixmap(":images/stop.png"), "stop");
    QAction* scroll = bar->addAction(QPixmap(":images/scroll.png"), "scroll");
    std::atomic<bool> isScroll = true;

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_comboBox_edit(int index);
    void on_doubleClick(const QModelIndex& index);
    void pause(bool state);
    void start(bool state);
    void press_scroll(bool state);
    void addItem();

signals:
    void pause_sniffer();
    void continue_sniffer();
};
#endif // MAINWINDOW_H

