#include "flattened_tree_model.h"

#include <qvariant.h>

FlattenedTreeModel::FlattenedTreeModel(QObject* parent)
    : QAbstractListModel(parent) {
    for (int i = 1; i < 125; ++i) {
        m_tree.insert(i);
    }

    m_flattened_tree_cache = m_tree.flattenInOrder();
}

auto FlattenedTreeModel::rowCount(const QModelIndex& parent) const -> int {
    if (parent.isValid()) {
        return 0;
    }

    return static_cast<int>(m_flattened_tree_cache.size());
}

auto FlattenedTreeModel::data(const QModelIndex& index, int role) const -> QVariant {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_flattened_tree_cache.size())) {
        return {};
    }

    const auto& item = m_flattened_tree_cache[static_cast<size_t>(index.row())];

    if (role == key_role) {
        return QVariant::fromValue(item.key);
    } else if (role == sum_role) {
        return QVariant::fromValue(item.sum);
    } else if (role == height_role) {
        return QVariant::fromValue(item.height);
    } else if (role == size_role) {
        return QVariant::fromValue(item.size);
    } else if (role == top_offset_role) {
        return QVariant::fromValue(item.top_offset);
    } else if (role == right_offset_role) {
        return QVariant::fromValue(item.right_offset);
    }

    return {};
}

auto FlattenedTreeModel::roleNames() const -> QHash<int, QByteArray> {
    static const QHash<int, QByteArray> role_names = {
        {key_role, "key"},
        {sum_role, "sum"},
        {height_role, "node_height"},
        {size_role, "size"},
        {top_offset_role, "top_offset"},
        {right_offset_role, "right_offset"},
    };

    return role_names;
}

void FlattenedTreeModel::append(int value) {  // NOLINT(modernize-use-trailing-return-type)
    beginResetModel();
    m_tree.insert(value);
    m_flattened_tree_cache = m_tree.flattenInOrder();
    endResetModel();
}

void FlattenedTreeModel::remove(int value) {  // NOLINT(modernize-use-trailing-return-type)
    beginResetModel();
    m_tree.remove(value);
    m_flattened_tree_cache = m_tree.flattenInOrder();
    endResetModel();
}
