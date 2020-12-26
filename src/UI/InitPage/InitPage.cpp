// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "InitPage.h"
#include <iostream>
#include <QRegExp>
#include "utils/utils.h"
#include <QFileDialog>

#include "core/core.h"

InitPage::InitPage(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::InitPage) {
    ui->setupUi(this);

    for(auto&& file_name: utils::get_recent_files())
        file_names.append(QString(file_name.c_str()));

    for (auto &&dev: core::device_list()) {
        l.append(QString(dev.c_str()));
    }


    ui->listWidget->addItems(l);
    this->setStyleSheet("QListWidget::item:hover {background-color:#095CF4; color: white;}");

    ui->listWidget_3->addItems(file_names);
    this->setStyleSheet("QListWidget::item:hover {background-color:#095CF4; color: white;}");

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem * )), this,
            SLOT(callback_slot(QListWidgetItem * )));

    connect(ui->listWidget_3, SIGNAL(itemDoubleClicked(QListWidgetItem * )), this,
            SLOT(callback_slot_file(QListWidgetItem * )));

    connect(ui->lineEdit_3, SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_textChanged_file(const QString &)));

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(on_browse()));
}

InitPage::~InitPage() {
    delete ui;
}


void InitPage::on_lineEdit_textChanged(const QString &arg1) {
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget->clear();
    ui->listWidget->addItems(l.filter(regExp));
}

void InitPage::on_lineEdit_textChanged_file(const QString &arg1) {
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget_3->clear();
    ui->listWidget_3->addItems(file_names.filter(regExp));
}

void InitPage::callback_slot(QListWidgetItem *item) {
    emit callback(item);
}

void InitPage::callback_slot_file(QListWidgetItem *item) {
    emit callback_file(item->text());
}

void InitPage::on_browse() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    "",
                                                    tr("Sniffer (*.pcap)"));

    if(!fileName.isEmpty()){
        utils::set_recent_file(fileName.toStdString());
        emit callback_file(std::move(fileName));
    }
}
