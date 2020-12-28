// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QThread>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>

#include "UI/UI.h"
#include "InitPage/InitPage.h"
#include "Table/Table.h"

MainWindow *window1 = nullptr;

class MyThread : public QThread {
public:
    explicit MyThread(core::sniffer &s) : sniffer(s) {}

protected:
    void run() override {
        sniffer.start_listen();
    };
private:
    core::sniffer& sniffer;
};

void UI::add_to_table(const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    window1->add_to_table(sockets::parse_packet(nullptr, pkt_hdr, packet));
}

class MainGui : public QMainWindow {
public:
    explicit MainGui(config &config) : configs(config), sniff(config){
        setWindowIcon(QIcon(":images/logo.png"));
        w = new InitPage;

        setCentralWidget(w);

        resize(800, 600);

        if (config.device.empty() && !config.from_file) {
            connect(w, &InitPage::callback, this, &MainGui::callback);
            connect(w, &InitPage::callback_file, this, &MainGui::callback_file);
        } else {
            init_second_step();
        }
    }


private:
    InitPage *w;
    MyThread* thread = nullptr;
    config &configs;
    core::sniffer sniff;

    void init_second_step() {
        window1 = new MainWindow(configs);

        if (w != nullptr) {
            w->close();
        }
        setCentralWidget(window1);

        thread = new MyThread(sniff);
        thread->start();

        connect(window1, &MainWindow::pause_sniffer, this, &MainGui::stop_thread);
        connect(window1, &MainWindow::continue_sniffer, this, &MainGui::continue_thread);
    }

    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton resBtn;
        if (configs.captured) {
            resBtn = QMessageBox::question(this, "sniffer",
                                           tr("Save file before exit?\n"),
                                           QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                           QMessageBox::Yes);
            if (resBtn == QMessageBox::No) {
                event->accept();
            } else if (resBtn == QMessageBox::Yes) {
                event->ignore();
                if(!configs.to_file) {
                    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                                    "",
                                                                    tr("Sniffer (*.pcap)"));
                    configs.to_file = true;
                    int index = fileName.lastIndexOf(".");
                    if(index != -1 && fileName.toStdString().substr(index) == "pcap") {
                        configs.to_file_name = fileName.toStdString() + ".pcap";
                    }
                }
                std::vector<pcap_pkthdr> pkt_hdr;
                std::vector<const u_char *> packet;

                window1->get_proxying_data(pkt_hdr, packet);
                core::write_to_file(configs, pkt_hdr, packet);
                window1->mutex.unlock();
                event->accept();
                close_s();
            }
        } else {
            resBtn = QMessageBox::question(this, "sniffer",
                                           tr("Are you sure?\n"),
                                           QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                           QMessageBox::No);
            if (resBtn != QMessageBox::Yes) {
                event->ignore();
            } else {
                event->accept();
            }
        }
    }

    void stop(){
        sniff.pause_listening();
        thread->exit();
    }

    void start(){
        thread->start();
    }

    void close_s(){
        sniff.close();
    }

private slots:
    void stop_thread(){
        stop();
    };

    void continue_thread(){
        start();
    }

    void close_sniffer(){
        close_s();
    }

    void callback(QListWidgetItem *item) {
        configs.device = item->text().toStdString();

        init_second_step();
    };

    void callback_file(QString file) {
        configs.from_file_name = file.toStdString();
        configs.from_file = true;

        init_second_step();
    };
};

int UI::init_gui(int argc, char **argv, config &config) {
    QApplication app(argc, argv);

    MainGui mainGui = MainGui(config);

    mainGui.show();

    return QApplication::exec();
}
