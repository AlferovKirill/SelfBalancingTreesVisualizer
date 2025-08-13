#include "flattened_tree_model.h"

#include <qvariant.h>

FlattenedTreeModel::FlattenedTreeModel(QObject* parent)
    : QAbstractListModel(parent) {
    for (int i = 1; i <= 255; ++i) {
        m_tree.insert(i);
    }

    m_flattened_tree_cache = m_tree.flatten();
}

auto FlattenedTreeModel::rowCount(const QModelIndex& parent) const -> int {
    if (parent.isValid()) {
        return 0;
    }

    return static_cast<int>(m_flattened_tree_cache.nodes.size());
}

auto FlattenedTreeModel::data(const QModelIndex& index, int role) const -> QVariant {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_flattened_tree_cache.nodes.size())) {
        return {};
    }

    const auto& item = m_flattened_tree_cache.nodes[static_cast<size_t>(index.row())];

    if (role == key_role) {
        return QVariant::fromValue(item.key);
    } else if (role == sum_role) {
        return QVariant::fromValue(item.sum);
    } else if (role == depth_role) {
        return QVariant::fromValue(item.depth);
    } else if (role == horizontal_offset_role) {
        return QVariant::fromValue(item.horizontal_offset);
    } else if (role == left_exists_role) {
        return QVariant::fromValue(item.left.exists);
    } else if (role == left_depth_role) {
        return QVariant::fromValue(item.left.depth);
    } else if (role == left_horizontal_offset_role) {
        return QVariant::fromValue(item.left.horizontal_offset);
    } else if (role == right_exists_role) {
        return QVariant::fromValue(item.right.exists);
    } else if (role == right_depth_role) {
        return QVariant::fromValue(item.right.depth);
    } else if (role == right_horizontal_offset_role) {
        return QVariant::fromValue(item.right.horizontal_offset);
    }

    return {};
}

auto FlattenedTreeModel::roleNames() const -> QHash<int, QByteArray> {
    static const QHash<int, QByteArray> role_names = {
        {key_role, "key"},
        {sum_role, "sum"},
        {depth_role, "depth"},
        {horizontal_offset_role, "horizontal_offset"},

        {left_exists_role, "left_exists"},
        {left_depth_role, "left_depth"},
        {left_horizontal_offset_role, "left_horizontal_offset"},

        {right_exists_role, "right_exists"},
        {right_depth_role, "right_depth"},
        {right_horizontal_offset_role, "right_horizontal_offset"},
    };

    return role_names;
}

auto FlattenedTreeModel::getHeight() const -> int {
    return static_cast<int>(m_flattened_tree_cache.height);
}

auto FlattenedTreeModel::getWidth() const -> int {
    return static_cast<int>(m_flattened_tree_cache.width);
}

void FlattenedTreeModel::append(int value) {  // NOLINT(modernize-use-trailing-return-type)
    beginResetModel();
    m_tree.insert(value);
    m_flattened_tree_cache = m_tree.flatten();
    endResetModel();

    emit heightChanged();
    emit widthChanged();
}

void FlattenedTreeModel::remove(int value) {  // NOLINT(modernize-use-trailing-return-type)
    beginResetModel();
    m_tree.remove(value);
    m_flattened_tree_cache = m_tree.flatten();
    endResetModel();

    emit heightChanged();
    emit widthChanged();
}
