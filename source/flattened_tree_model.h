#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>

#include "avl_tree.hpp"

class FlattenedTreeModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int height READ getHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(int width READ getWidth NOTIFY widthChanged FINAL)

public:
    enum roles_enum {
        key_role = Qt::UserRole + 1,
        sum_role,
        depth_role,
        horizontal_offset_role,

        left_exists_role,
        left_depth_role,
        left_horizontal_offset_role,

        right_exists_role,
        right_depth_role,
        right_horizontal_offset_role,
    };
    Q_ENUM(roles_enum)

    explicit FlattenedTreeModel(QObject* parent = nullptr);

    auto rowCount(const QModelIndex& parent) const -> int override;
    auto data(const QModelIndex& index, int role) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;

    auto getHeight() const -> int;
    auto getWidth() const -> int;

    Q_INVOKABLE void append(int value);
    Q_INVOKABLE void remove(int value);

signals:
    auto heightChanged() -> void;
    auto widthChanged() -> void;

private:
    using tree_type = AVLTree;

    tree_type m_tree;
    FlattenedTreeGuiDto m_flattened_tree_cache;
};
