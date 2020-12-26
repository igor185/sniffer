// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ProxyModel.h"


ProxyModel::ProxyModel(QWidget *parent) : QSortFilterProxyModel(parent){
        primary_filters = {};
        auto* filter = new Filter();
        primary_filters.push_back(filter);

        connect(filter, SIGNAL(changed()), this, SLOT(changed()));
}

void ProxyModel::onChangeText(const QString &value) {
    primary_filters[0]->value = value;
    invalidateFilter();
}

void ProxyModel::onChangeField(int index) {
    primary_filters[0]->type = static_cast<Types>(index);
    invalidateFilter();
}

Filter *ProxyModel::getFirst() {
    return primary_filters[0];
}

bool ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    for(Filter* filter: primary_filters){ // TODO use reversed iterator to use conneted with prev
        QModelIndex index = sourceModel()->index(sourceRow, filter->type, sourceParent);
        if(!filter->filter(index))
            return false;
    }
    return true;
}
