// #pragma once

// #include <QAbstractItemModel>
// #include <QString>
// #include <QVariant>

// #include "avl_tree.hpp"

// class TreeViewModel : public QAbstractItemModel {
//     Q_OBJECT

// public:
//     Q_DISABLE_COPY_MOVE(TreeViewModel)

//     enum roles_enum {
//         key = Qt::UserRole + 1,
//     };
//     Q_ENUM(roles_enum)

//     explicit TreeViewModel(QObject* parent = nullptr);
//     ~TreeViewModel() override = default;

//     auto data(const QModelIndex& index, int role) const -> QVariant override;
//     auto index(int row, int column, const QModelIndex& parent_index) const -> QModelIndex override;
//     auto parent(const QModelIndex& index) const -> QModelIndex override;
//     auto rowCount(const QModelIndex& parent_index) const -> int override;
//     auto columnCount(const QModelIndex& parent_index) const -> int override;

//     auto clearModel() -> void;

// signals:
//     void selectedCommandUpdated(const QModelIndex&) const;
//     void expandToCommand(const QModelIndex&) const;
//     void expandCommand(const QModelIndex&) const;
//     void collapseCommand(const QModelIndex&) const;
//     void recursivelyExpandCommand(const QModelIndex&) const;
//     void recursivelyCollapseCommand(const QModelIndex&) const;

//     void moveViewPositionAtCommand(const QModelIndex&) const;

// protected:
//     auto roleNames() const -> QHash<int, QByteArray> override;

// private:
//     using tree_key_type = int;
//     using tree_type = AVLTree<tree_key_type>;
//     using node_type = tree_type::Node;

//     tree_type m_tree_model;
// };