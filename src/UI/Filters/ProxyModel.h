// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <UI/UI.h>
#include <vector>
#include <QSortFilterProxyModel>
#include "ui_filters.h"
#include "filters.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FiltersView; }
QT_END_NAMESPACE

class ProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ProxyModel(QWidget *parent = nullptr);

    void onChangeText(const QString& value);

    void onChangeField(int index);

    Filter* getFirst();

    std::vector<Filter*> getFilters() const{
        return primary_filters;
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private slots:
    void changed(){
        invalidateFilter();
    };
private:
    std::vector<Filter*> primary_filters;
};

class FiltersView : public QMainWindow {
Q_OBJECT

public:
    explicit FiltersView(ProxyModel* proxy, QWidget *parent = nullptr);

    void add(QWidget *layout);

    ~FiltersView() override;

private slots:

    void handleButton();

private:
    QToolBar *bar = new QToolBar;
    Ui::FiltersView *ui;

    QAction *orIcon = bar->addAction(QPixmap(":images/or.png"), "or"); // TODO find better icons
    QAction *andIcon = bar->addAction(QPixmap(":images/and.png"), "and");
};


#endif // PROXYMODEL_H