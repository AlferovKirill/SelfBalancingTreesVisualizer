#include <cmath>

#include "avl_tree.hpp"

#include <qvariant.h>

AVLTree::AVLTree(QObject* parent)
    : QAbstractItemModel(parent)
    , m_dummy_root(std::make_shared<Node>(Node::key_type {})) {

        insert(1);
        insert(2);
        insert(3);
        insert(4);
        insert(5);
        insert(6);
        insert(7);
    }

auto AVLTree::data(const QModelIndex& index, int role) const -> QVariant {
    if (!index.isValid()) {
        return {};
    }

    const auto* node = static_cast<const Node*>(index.internalPointer());
    auto enum_role = static_cast<roles_enum>(role);

    if (node != nullptr) {
        if (enum_role == roles_enum::key) {
            return QVariant::fromValue(node->m_key);
        } else if (enum_role == roles_enum::is_left_child) {
            const auto* parent = node->m_parent.lock().get();
            auto result = (parent != nullptr) ? parent->m_left.get() == node : false;

            return result;
        }
    }

    return {};
}

auto AVLTree::index(int row, int column, const QModelIndex& parent_index) const -> QModelIndex {
    if (!hasIndex(row, column, parent_index)) {
        return {};
    }

    const auto* parent = parent_index.isValid() ? static_cast<const Node*>(parent_index.internalPointer()) : m_dummy_root.get();
    const auto* child_node = (row == 0) ? parent->m_left.get() : parent->m_right.get();

    if (child_node != nullptr) {
        return createIndex(row, column, child_node);
    }

    return {};
}

auto AVLTree::parent(const QModelIndex& index) const -> QModelIndex {
    if (!index.isValid()) {
        return {};
    }

    const auto* child = static_cast<const Node*>(index.internalPointer());
    const auto* parent = child->m_parent.lock().get();

    if (parent != nullptr && parent != m_dummy_root.get()) {
        const auto* grand_parent = parent->m_parent.lock().get();
        int row = (grand_parent != nullptr && grand_parent->m_right.get() == parent) ? 1 : 0;

        return createIndex(row, 0, parent);
    }

    return {};
}

auto AVLTree::rowCount(const QModelIndex& parent_index) const -> int {
    const auto* parent = parent_index.isValid() ? static_cast<const Node*>(parent_index.internalPointer()) : m_dummy_root.get();
    int result = 0;

    if (parent->m_left) {
        ++result;
    }
    if (parent->m_right) {
        ++result;
    }

    return result;
}

auto AVLTree::columnCount(const QModelIndex& /*parent_index*/) const -> int {
    return 1;
}

auto AVLTree::insert(Node::key_type key) -> void {
    if (m_root) {
        auto parent = m_root;

        while (true) {
            if (key > parent->m_key) {
                if (!parent->m_right) {
                    parent->m_right = std::make_shared<Node>(key, parent);
                    break;
                } else {
                    parent = parent->m_right;
                }
            } else if (key < parent->m_key) {
                if (!parent->m_left) {
                    parent->m_left = std::make_shared<Node>(key, parent);
                    break;
                } else {
                    parent = parent->m_left;
                }
            } else {
                return;
            }
        }

        while (parent != m_dummy_root) {
            balance(parent);
            parent = parent->m_parent.lock();
        }
    } else {
        m_root = std::make_shared<Node>(key);

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }
    }
}

auto AVLTree::find(Node::key_type key) const -> bool {
    return static_cast<bool>(findNode(key));
}

auto AVLTree::remove(Node::key_type key) -> void {
    remove(findNode(key));
}

auto AVLTree::max() const -> Node::key_type {
    auto node = max(m_root);
    return (node ? node->m_key : 0);
}

auto AVLTree::min() const -> Node::key_type {
    auto node = min(m_root);
    return (node ? node->m_key : 0);
}

auto AVLTree::sum(Node::key_type left_key, Node::key_type right_key) -> Node::key_type {
    auto [left, temp] = split(m_root, left_key - 1);

    m_root = temp;

    m_dummy_root->m_left = m_root;

    if (m_root) {
        m_root->m_parent = m_dummy_root;
    }

    auto [middle, right] = split(temp, right_key);
    auto result = middle ? middle->m_sum : 0;

    temp = merge(left, middle);
    m_root = merge(temp, right);

    m_dummy_root->m_left = m_root;

    if (m_root) {
        m_root->m_parent = m_dummy_root;
    }

    return result;
}

auto AVLTree::getMaxDepth() const -> int {
    return static_cast<int>(m_root->m_height);
}

auto AVLTree::roleNames() const -> QHash<int, QByteArray> {
    static const QHash<int, QByteArray> roles = {
        {static_cast<int>(roles_enum::key), "key"},
        {static_cast<int>(roles_enum::is_left_child), "isLeftChild"},
    };

    return roles;
}

auto AVLTree::updateHeuristics(std::shared_ptr<Node> node) -> void {  // NOLINT(performance-unnecessary-value-param)
    if (!node) {
        return;
    }

    if (node->m_left && node->m_right) {
        node->m_sum = node->m_left->m_sum + node->m_right->m_sum + node->m_key;
        node->m_height = std::max(node->m_left->m_height, node->m_right->m_height) + 1;
        node->m_size = node->m_left->m_size + node->m_right->m_size + 1;
    } else if (node->m_left) {
        node->m_sum = node->m_left->m_sum + node->m_key;
        node->m_height = node->m_left->m_height + 1;
        node->m_size = node->m_left->m_size + 1;
    } else if (node->m_right) {
        node->m_sum = node->m_right->m_sum + node->m_key;
        node->m_height = node->m_right->m_height + 1;
        node->m_size = node->m_right->m_size + 1;
    } else {
        node->m_sum = node->m_key;
        node->m_height = 1;
        node->m_size = 1;
    }
}

auto AVLTree::max(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {
    while (node && node->m_right) {
        node = node->m_right;
    }

    return node;
}

auto AVLTree::min(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {
    while (node && node->m_left) {
        node = node->m_left;
    }

    return node;
}

auto AVLTree::mergeWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> void {  // NOLINT(performance-unnecessary-value-param)
    if (node) {
        node->m_left = v_1;
        node->m_right = v_2;

        if (v_1) {
            v_1->m_parent = node;
        }
        if (v_2) {
            v_2->m_parent = node;
        }
    }
}

auto AVLTree::disconnectNodeFromParent(std::shared_ptr<Node> node) -> void {  // NOLINT(performance-unnecessary-value-param)
    if (!node) {
        return;
    }

    auto parent = node->m_parent.lock();

    if (parent) {
        if (node == parent->m_left) {
            parent->m_left = nullptr;
        } else {
            parent->m_right = nullptr;
        }
    }

    node->m_parent.reset();
}

auto AVLTree::balance(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {  // NOLINT(readability-function-cognitive-complexity)
    if (!node) {
        return nullptr;
    }

    updateHeuristics(node);

    auto l_height = node->m_left ? node->m_left->m_height : 0;
    auto r_height = node->m_right ? node->m_right->m_height : 0;

    if (labs(l_height - r_height) > 1) {
        if (l_height < r_height) {
            auto rr_height = node->m_right->m_right ? node->m_right->m_right->m_height : 0;
            auto rl_height = node->m_right->m_left ? node->m_right->m_left->m_height : 0;

            if (rr_height >= rl_height) {
                node = leftRotate(node);
            } else {
                node->m_right = rightRotate(node->m_right);
                node = leftRotate(node);
            }
        } else {
            auto ll_height = node->m_left->m_left ? node->m_left->m_left->m_height : 0;
            auto lr_height = node->m_left->m_right ? node->m_left->m_right->m_height : 0;

            if (ll_height >= lr_height) {
                node = rightRotate(node);
            } else {
                node->m_left = leftRotate(node->m_left);
                node = rightRotate(node);
            }
        }
    }

    return node;
}

auto AVLTree::leftRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {  // NOLINT(performance-unnecessary-value-param)
    if (!node) {
        return nullptr;
    }

    const auto& alpha = node;
    auto beta = node->m_right;
    auto node_b = beta ? beta->m_left : nullptr;

    alpha->m_right = node_b;

    if (node_b) {
        node_b->m_parent = alpha;
    }

    beta->m_parent = alpha->m_parent;
    alpha->m_parent = beta;
    beta->m_left = alpha;

    auto parent = beta->m_parent.lock();

    if (parent && parent != m_dummy_root && parent->m_left == alpha) {
        parent->m_left = beta;
    } else if (parent && parent != m_dummy_root && parent->m_right == alpha) {
        parent->m_right = beta;
    } else {
        m_root = beta;

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }
    }

    updateHeuristics(alpha);
    updateHeuristics(beta);

    return beta;
}

auto AVLTree::rightRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node> {  // NOLINT(performance-unnecessary-value-param)
    if (!node) {
        return nullptr;
    }

    const auto& alpha = node;
    auto beta = node->m_left;
    auto node_b = beta ? beta->m_right : nullptr;

    alpha->m_left = node_b;

    if (node_b) {
        node_b->m_parent = alpha;
    }

    beta->m_parent = alpha->m_parent;
    alpha->m_parent = beta;
    beta->m_right = alpha;

    auto parent = beta->m_parent.lock();

    if (parent && parent != m_dummy_root && parent->m_left == alpha) {
        parent->m_left = beta;
    } else if (parent && parent != m_dummy_root && parent->m_right == alpha) {
        parent->m_right = beta;
    } else {
        m_root = beta;

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }
    }

    updateHeuristics(alpha);
    updateHeuristics(beta);

    return beta;
}

auto AVLTree::findNode(Node::key_type key) const -> std::shared_ptr<Node> {
    auto current_node = m_root;

    while (current_node) {
        if (key > current_node->m_key) {
            current_node = current_node->m_right;
        } else if (key < current_node->m_key) {
            current_node = current_node->m_left;
        } else {
            return current_node;
        }
    }

    return nullptr;
}

auto AVLTree::remove(std::shared_ptr<Node> node) -> void {  // NOLINT(readability-function-cognitive-complexity, misc-no-recursion, performance-unnecessary-value-param)
    if (!node) {
        return;
    }

    auto parent = node->m_parent.lock();

    if (!node->m_left && !node->m_right) {
        if (parent != m_dummy_root) {
            if (parent->m_left == node) {
                parent->m_left = nullptr;
            } else {
                parent->m_right = nullptr;
            }

            node->m_parent.reset();
        } else {
            m_root = nullptr;
            m_dummy_root->m_left = nullptr;
        }
    } else if (node->m_left && !node->m_right) {
        if (parent != m_dummy_root) {
            if (parent->m_left == node) {
                parent->m_left = node->m_left;
            } else {
                parent->m_right = node->m_left;
            }

            node->m_left->m_parent = parent;

            node->m_parent.reset();
            node->m_left.reset();
        } else {
            m_root = node->m_left;
            m_root->m_parent.reset();

            m_dummy_root->m_left = m_root;

            if (m_root) {
                m_root->m_parent = m_dummy_root;
            }
        }
    } else if (!node->m_left && node->m_right) {
        if (parent != m_dummy_root) {
            if (parent->m_left == node) {
                parent->m_left = node->m_right;
            } else {
                parent->m_right = node->m_right;
            }

            node->m_right->m_parent = parent;

            node->m_parent.reset();
            node->m_right.reset();
        } else {
            m_root = node->m_right;
            m_root->m_parent.reset();

            m_dummy_root->m_left = m_root;

            if (m_root) {
                m_root->m_parent = m_dummy_root;
            }
        }
    } else {
        auto successor = max(node->m_left);
        Node::key_type successor_key = successor->m_key;

        remove(successor);

        node->m_key = successor_key;
        parent = node;
    }

    while (parent != m_dummy_root) {
        updateHeuristics(parent);
        balance(parent);

        parent = parent->m_parent.lock();
    }
}

auto AVLTree::mergeAVLWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> std::shared_ptr<Node> {  // NOLINT(misc-no-recursion, performance-unnecessary-value-param)
    auto l_height = v_1 ? v_1->m_height : 0;
    auto r_height = v_2 ? v_2->m_height : 0;

    if (labs(l_height - r_height) <= 1) {
        mergeWithRoot(v_1, v_2, node);
        updateHeuristics(node);

        return node;
    } else if (l_height > r_height) {
        auto temp = mergeAVLWithRoot(v_1->m_right, v_2, node);
        v_1->m_right = temp;
        temp->m_parent = v_1;

        return balance(v_1);
    } else {
        auto temp = mergeAVLWithRoot(v_1, v_2->m_left, node);
        v_2->m_left = temp;
        temp->m_parent = v_2;

        return balance(v_2);
    }
}

auto AVLTree::merge(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2) -> std::shared_ptr<Node> {  // NOLINT(performance-unnecessary-value-param)
    if (v_1) {
        m_root = v_1;

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }

        auto temp = max(v_1);
        remove(temp);

        if (temp) {
            temp->m_parent.reset();
            temp->m_left.reset();
            temp->m_right.reset();
        }

        m_root = mergeAVLWithRoot(m_root, v_2, temp);

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }
    } else if (v_2) {
        m_root = v_2;

        m_dummy_root->m_left = m_root;

        if (m_root) {
            m_root->m_parent = m_dummy_root;
        }
    } else {
        return nullptr;
    }

    updateHeuristics(m_root);

    m_dummy_root->m_left = m_root;

    if (m_root) {
        m_root->m_parent = m_dummy_root;
    }

    return m_root;
}

auto AVLTree::split(std::shared_ptr<Node> node_v, Node::key_type key) -> std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> {  // NOLINT(misc-no-recursion, performance-unnecessary-value-param)
    if (!node_v) {
        return {nullptr, nullptr};
    }

    disconnectNodeFromParent(node_v);

    if (key < node_v->m_key) {
        auto [v_1, v_2] = split(node_v->m_left, key);
        auto right = node_v->m_right;

        disconnectNodeFromParent(v_2);
        disconnectNodeFromParent(right);

        auto temp = mergeAVLWithRoot(v_2, right, node_v);

        return {v_1, temp};
    } else {
        auto [v_1, v_2] = split(node_v->m_right, key);
        auto left = node_v->m_left;

        disconnectNodeFromParent(v_1);
        disconnectNodeFromParent(left);

        auto temp = mergeAVLWithRoot(left, v_1, node_v);

        return {temp, v_2};
    }
}
