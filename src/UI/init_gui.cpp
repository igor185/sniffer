// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <QApplication>
#include <QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QThread>
#include <QCloseEvent>
#include <QMessageBox>

#include "util/utils.h"
#include "UI/UI.h"
#include "InitPage/InitPage.h"
#include "Table/Table.h"
#include <IO/IO.h>
#include <QFileDialog>
#include <QToolBar>

class MyThread : public QThread {
public:
    explicit MyThread(core::config &conf) : config(conf) {}

protected:
    void run() override {
        core::init_listening(config);
    };
private:
    core::config &config;
};

MainWindow *window1 = nullptr; // TODO global var??

void UI::add_to_table(const struct pcap_pkthdr *pkt_hdr, const u_char *packet) {
    window1->add_to_table(sockets::parse_packet(nullptr, pkt_hdr, packet));
}

class MainGui : public QMainWindow {
public:
    explicit MainGui(core::config &config1) : config(config1) {
        setWindowIcon(QIcon(":images/logo.png"));
        w = new InitPage;

        setCentralWidget(w);

        resize(800, 600);

        if (config.device.empty() && !config.from_file) {
            connect(w, &InitPage::callback, this, &MainGui::callback);
        } else {
            init_second_step();
        }
    }


private:
    InitPage *w;
    MyThread* thread = nullptr;
    core::config &config;

    void init_second_step() {
        window1 = new MainWindow(config);

        if (w != nullptr) {
            w->close();
        }
        setCentralWidget(window1);

        thread = new MyThread(config);
        thread->start();

        connect(window1, &MainWindow::pause_sniffer, this, &MainGui::stop_thread);
        connect(window1, &MainWindow::continue_sniffer, this, &MainGui::continue_thread);
    }

    void closeEvent(QCloseEvent *event) override {
        QMessageBox::StandardButton resBtn;
        if (config.captured) {
            resBtn = QMessageBox::question(this, "sniffer",
                                           tr("Save file before exit?\n"),
                                           QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                           QMessageBox::Yes);
            if (resBtn == QMessageBox::No) {
                event->accept();
            } else if (resBtn == QMessageBox::Yes) {
                event->ignore();
                if(!config.to_file) {
                    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                                    "",
                                                                    tr("Sniffer (*.pcap)"));
                    config.to_file = true;
                    int index = fileName.lastIndexOf(".");
                    if(index != -1 && fileName.toStdString().substr(index) == "pcap") {
                        config.to_file_name = fileName.toStdString() + ".pcap";
                    }
                }
                std::vector<pcap_pkthdr> pkt_hdr;
                std::vector<const u_char *> packet;

                window1->get_proxying_data(pkt_hdr, packet);
                core::write_to_file(config, pkt_hdr, packet);
                window1->mutex.unlock();
                event->accept();
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
private slots:
    static void stop_thread(){
        core::pause_capturing();
    };

    void continue_thread(){
        thread = new MyThread(config);
        thread->start();
    }

    void callback(QListWidgetItem *item) {
        config.device = item->text().toStdString();

        init_second_step();
    };
};

int UI::init_gui(int argc, char **argv, core::config &config) {
    QApplication app(argc, argv);

    MainGui mainGui = MainGui(config);

    mainGui.show();

    return QApplication::exec();
}
