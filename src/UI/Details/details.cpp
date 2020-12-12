#include "details.h"


Details::Details(QWidget *viewer, std::vector<sockets::detail_view>& view, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Details)
{

    ui->setupUi(this);

    ui->treeWidget->setHeaderHidden(true);

    QList<QTreeWidgetItem *> items;

    for(auto&& i: view){
        auto item = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("%1").arg(i.preview.c_str())));

        QList<QTreeWidgetItem *> items_inner;
        for(auto&& j: i.properties){
            items_inner.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("%1: %2").arg(j.first.c_str()).arg(j.second.c_str()))));
        }
        item->addChildren(items_inner);

        items.append(item);
    }

    ui->treeWidget->insertTopLevelItems(0, items);

    ui->gridLayout->addWidget(viewer);
}

Details::~Details()
{
    delete ui;
}

