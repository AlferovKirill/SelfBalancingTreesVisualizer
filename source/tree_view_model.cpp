// #include "tree_view_model.h"

// TreeViewModel::TreeViewModel(QObject* parent)
//     : QAbstractItemModel(parent) {
//     m_tree_model.insert(5);
//     m_tree_model.insert(4);
//     m_tree_model.insert(6);
//     m_tree_model.insert(7);
// }

// auto TreeViewModel::data(const QModelIndex& index, int role) const -> QVariant {
//     if (!index.isValid()) {
//         return {};
//     }

//     auto* node = static_cast<node_type*>(index.internalPointer());

//     if (node != nullptr) {
//         if (role == key) {
//             return node->key;
//         }
//     }

//     return {};
// }

// auto TreeViewModel::index(int row, int column, const QModelIndex& parent_index) const -> QModelIndex {
//     const node_type* parent_node = parent_index.isValid() ? static_cast<node_type*>(parent_index.internalPointer()) : m_tree_model.root();
//     node_type* child_node = nullptr;

//     if (row == 0) {
// 		child_node = parent_node->left.get();
// 	}
//     else if (row == 1) {
// 		child_node = parent_node->right.get();
// 	}

//     if (child_node != nullptr) {
//         return createIndex(row, column, child_node);
// 	}

//     return {};
// }

// auto TreeViewModel::parent(const QModelIndex& index) const -> QModelIndex {
//     if (!index.isValid()) {
//         return {};
//     }

//     const node_type* child_node = static_cast<node_type*>(index.internalPointer());
//     node_type* parent_node = child_node->parent.lock().get();

//     if ((parent_node == nullptr) || parent_node == m_tree_model.root()) {
//         return {};
//     }

//     node_type* grand_node = parent_node->parent.lock().get();
//     int row = ((grand_node != nullptr) && grand_node->left.get() == parent_node) ? 0 : 1;

//     return createIndex(row, 0, parent_node);
// }

// auto TreeViewModel::rowCount(const QModelIndex& parent_index) const -> int {
//     const auto* parent_node = parent_index.isValid() ? static_cast<node_type*>(parent_index.internalPointer()) : m_tree_model.root();
//     return static_cast<int>(parent_node->size - 1);
// }

// auto TreeViewModel::columnCount(const QModelIndex& /*parent_index*/) const -> int {
//     return 1;
// }

// auto TreeViewModel::clearModel() -> void {
//     if (m_tree_model.size() > 0) {
//         beginResetModel();
//         m_tree_model.clear();
//         endResetModel();
//     }
// }

// auto TreeViewModel::roleNames() const -> QHash<int, QByteArray> {
//     static const QHash<int, QByteArray> roles = {
//         {roles_enum::key, "key"},
//     };

//     return roles;
// }