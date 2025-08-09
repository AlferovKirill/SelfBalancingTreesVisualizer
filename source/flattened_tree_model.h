#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <cstdint>

#include "avl_tree.hpp"

class FlattenedTreeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum roles_enum {
        key_role = Qt::UserRole + 1,
        sum_role,
        height_role,
        size_role,
        top_offset_role,
        right_offset_role,
    };
    Q_ENUM(roles_enum)

    explicit FlattenedTreeModel(QObject* parent = nullptr);

    auto rowCount(const QModelIndex& parent) const -> int override;
    auto data(const QModelIndex& index, int role) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;

    Q_INVOKABLE void append(int value);
    Q_INVOKABLE void remove(int value);

private:
    using tree_type = AVLTree<int64_t>;

    tree_type m_tree;
    std::vector<tree_type::node_dto_type> m_flattened_tree_cache;
};
