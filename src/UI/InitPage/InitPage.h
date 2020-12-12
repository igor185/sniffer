// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef SNIFFER_INITPAGE_H
#define SNIFFER_INITPAGE_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "core/core.h"
#include "ui_init_page.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InitPage; }
QT_END_NAMESPACE

class InitPage : public QMainWindow
{
Q_OBJECT

public:
    explicit InitPage(QWidget *parent = nullptr);
    ~InitPage() override;

private:
    Ui::InitPage *ui;
    QStringList l;
signals:
    void callback(QListWidgetItem* item);
private slots:
    void callback_slot(QListWidgetItem* item);
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif //SNIFFER_INITPAGE_H
