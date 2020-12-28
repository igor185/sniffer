// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ProxyModel.h"


ProxyModel::ProxyModel(QWidget *parent) : QSortFilterProxyModel(parent){
        primary_filters = {};
        auto* filter = new Filter(nullptr);
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
    bool prev = false, curr;
    Logic prevLogic = Or, currLogic;
    for(auto filter = primary_filters.rbegin(); filter != primary_filters.rend(); filter++){
        QModelIndex index = sourceModel()->index(sourceRow, (*filter)->type, sourceParent);
        auto *socket = data[index.sibling(index.row(), 0).data().value<size_t>() - 1];

        curr = (*filter)->filter(socket);
        currLogic = (*filter)->connect_with_prev;

        if(prevLogic == Or){
            prev = prev || curr;
            prevLogic = currLogic;
            continue;
        }

        if(!prev)
            return false;

        prev = curr;
        prevLogic = currLogic;
    }
    return prev;
}
