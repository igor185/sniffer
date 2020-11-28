// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "UI/InitPage.h"
#include "ui_init_page.h"
#include <iostream>
#include <QRegExp>

#include "core/core.h"

InitPage::InitPage(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::InitPage) {
    ui->setupUi(this);

    for (auto &&dev: core::device_list()) {
        l.append(QString(dev.c_str()));
    }


    ui->listWidget->addItems(l);
    this->setStyleSheet("QListWidget::item:hover {background-color:#095CF4; color: white;}");

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem * )), this,
            SLOT(callback_slot(QListWidgetItem * )));
}

InitPage::~InitPage() {
    delete ui;
}


void InitPage::on_lineEdit_textChanged(const QString &arg1) {
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget->clear();
    ui->listWidget->addItems(l.filter(regExp));
}

void InitPage::callback_slot(QListWidgetItem *item) {
    emit callback(item);
}
