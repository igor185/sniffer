// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <UI/UI.h>
#include <vector>
#include <QSortFilterProxyModel>
#include "ui_filters.h"
#include "filters.h"
#include "iostream"
QT_BEGIN_NAMESPACE
namespace Ui { class FiltersView; }
QT_END_NAMESPACE

class ProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    std::vector<sockets::base_socket *> data;
    explicit ProxyModel(QWidget *parent = nullptr);

    void onChangeText(const QString& value);

    void onChangeField(int index);

    Filter* getFirst();

    std::vector<Filter*> getFilters() const{
        return primary_filters;
    }

    void add_filter(Filter* filter){
        primary_filters.push_back(filter);
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private slots:
    void changed(){
        invalidateFilter();
    };

    void remove(Filter* filter){
        size_t index = -1;
        for(size_t i = 0; i < primary_filters.size(); i++){
            if(primary_filters[i] == filter){
                index = i;
                break;
            }
        }
        if(index != -1){
            primary_filters.erase(primary_filters.begin() + index);
        }
        delete filter;
        invalidateFilter();
    }
private:
    std::vector<Filter*> primary_filters;
};

class FiltersView : public QMainWindow {
Q_OBJECT

public:
    explicit FiltersView(ProxyModel* proxy, QWidget *parent = nullptr);

    ~FiltersView() override;

private slots:
    void handleAnd();
    void handleOr();

private:
    QToolBar *bar = new QToolBar;
    Ui::FiltersView *ui;
    ProxyModel* proxy;

    QAction *orIcon = bar->addAction(QPixmap(":images/plus.png"), "or");
    QAction *andIcon = bar->addAction(QPixmap(":images/plus.png"), "and");

    void addFilter(Logic type);
};


#endif // PROXYMODEL_H