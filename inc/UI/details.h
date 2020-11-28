#ifndef DETAILS_H
#define DETAILS_H

#include <QMainWindow>
#include <QDialog>
#include <vector>

#include "core/core.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Details; }
QT_END_NAMESPACE

class Details : public QMainWindow
{
    Q_OBJECT

public:
    Details(QWidget* viewer, std::vector<sockets::detail_view>& view, QWidget *parent = nullptr);
    ~Details();

private:
    Ui::Details *ui;
};
#endif // DETAILS_H
