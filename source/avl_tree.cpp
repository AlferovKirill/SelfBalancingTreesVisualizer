#include <cmath>
#include <cstdint>
#include <functional>

#include "avl_tree.hpp"

AVLTree::AVLTree(QObject* parent)
    : QAbstractListModel(parent) {
        for (size_t i = 0; i < 15; ++i) {
            insert(i);
        }
    }

void AVLTree::insert(key_type key) {
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

        while (parent) {
            balance(parent);
            parent = parent->m_parent.lock();
        }
    } else {
        m_root = std::make_shared<Node>(key);
    }

    // just caching and sending signals

    auto prev_size = static_cast<int>(m_flattened_tree_cache.nodes.size());
    m_flattened_tree_cache = flatten();

    beginInsertRows(QModelIndex(), prev_size, prev_size);
    endInsertRows();

    if (prev_size > 0) {
        emit dataChanged(index(0), index(prev_size));
    }

    emit heightChanged();
    emit widthChanged();
}

bool AVLTree::find(key_type key) const { // NOLINT(modernize-use-trailing-return-type)
    return static_cast<bool>(findNode(key));
}

void AVLTree::remove(key_type key) {
    auto node = findNode(key);

    if (node) {
        remove(node);

        // just caching and sending signals

        auto prev_size = static_cast<int>(m_flattened_tree_cache.nodes.size());
        m_flattened_tree_cache = flatten();

        beginRemoveRows(QModelIndex(), prev_size - 1, prev_size - 1);
        endRemoveRows();
        
        if (prev_size > 1) {
            emit dataChanged(index(0), index(prev_size - 2));
        }

        emit heightChanged();
        emit widthChanged();
    }
}

auto AVLTree::max() const -> key_type {
    auto node = max(m_root);
    return (node ? node->m_key : 0);
}

auto AVLTree::min() const -> key_type {
    auto node = min(m_root);
    return (node ? node->m_key : 0);
}

auto AVLTree::sum(key_type left_key, key_type right_key) -> key_type {
    auto [left, temp] = split(m_root, left_key - 1);

    m_root = temp;

    auto [middle, right] = split(temp, right_key);
    auto interval_sum = middle ? middle->m_sum : 0;

    temp = merge(left, middle);
    m_root = merge(temp, right);

    return interval_sum;
}

auto AVLTree::size() const -> int64_t {
    return m_root ? m_root->m_size : 0;
}

// TODO: make it iterative
auto AVLTree::flatten() const -> FlattenedTreeGuiDto {
    FlattenedTreeGuiDto flattened_tree;

    int64_t max_horizontal_offset = 0;

    std::function<void(std::shared_ptr<Node>, int64_t, int64_t)> compute_node_gui_dto = [&](const std::shared_ptr<Node>& node, int64_t depth, int64_t horizontal_offset)
    {
        if (!node) {
            return;
        }

        auto left_horizontal_offset = horizontal_offset - 1;
        auto right_horizontal_offset = horizontal_offset + 1;

        if (node->m_left && node->m_left->m_right) {
            left_horizontal_offset -= node->m_left->m_right->m_size;
        }

        if (node->m_right && node->m_right->m_left) {
            right_horizontal_offset += node->m_right->m_left->m_size;
        }

        flattened_tree.nodes.push_back({
            node->m_key,
            node->m_sum,
            depth,
            horizontal_offset,
            {
                static_cast<bool>(node->m_left),
                depth + 1,
                left_horizontal_offset,
            },
            {
                static_cast<bool>(node->m_right),
                depth + 1,
                right_horizontal_offset,
            },
        });

        if (horizontal_offset > max_horizontal_offset) {
            max_horizontal_offset = horizontal_offset;
        }

        compute_node_gui_dto(node->m_left, depth + 1, left_horizontal_offset);
        compute_node_gui_dto(node->m_right, depth + 1, right_horizontal_offset);
    };

    compute_node_gui_dto(m_root, 0, ((m_root && m_root->m_left) ? m_root->m_left->m_size : 0));

    flattened_tree.height = m_root ? m_root->m_height : 0;
    flattened_tree.width = m_root ? max_horizontal_offset + 1 : 0;

    return flattened_tree;
}

auto AVLTree::rowCount(const QModelIndex& parent) const -> int {
    if (parent.isValid()) {
        return 0;
    }

    return static_cast<int>(m_flattened_tree_cache.nodes.size());
}

auto AVLTree::data(const QModelIndex& index, int role) const -> QVariant {
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

auto AVLTree::roleNames() const -> QHash<int, QByteArray> {
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

auto AVLTree::getHeight() const -> int {
    return static_cast<int>(m_flattened_tree_cache.height);
}

auto AVLTree::getWidth() const -> int {
    return static_cast<int>(m_flattened_tree_cache.width);
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

    if (parent && parent->m_left == alpha) {
        parent->m_left = beta;
    } else if (parent && parent->m_right == alpha) {
        parent->m_right = beta;
    } else {
        m_root = beta;
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

    if (parent && parent->m_left == alpha) {
        parent->m_left = beta;
    } else if (parent && parent->m_right == alpha) {
        parent->m_right = beta;
    } else {
        m_root = beta;
    }

    updateHeuristics(alpha);
    updateHeuristics(beta);

    return beta;
}

auto AVLTree::findNode(key_type key) const -> std::shared_ptr<Node> {
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

auto AVLTree::remove(std::shared_ptr<Node> node) -> void {  // NOLINT(performance-unnecessary-value-param, misc-no-recursion, readability-function-cognitive-complexity)
    if (!node) {
        return;
    }

    auto parent = node->m_parent.lock();

    if (!node->m_left && !node->m_right) {
        if (parent) {
            if (parent->m_left == node) {
                parent->m_left = nullptr;
            } else {
                parent->m_right = nullptr;
            }

            node->m_parent.reset();
        } else {
            m_root = nullptr;
        }
    } else if (node->m_left && !node->m_right) {
        if (parent) {
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
        }
    } else if (!node->m_left && node->m_right) {
        if (parent) {
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
        }
    } else {
        auto successor = max(node->m_left);
        key_type successor_key = successor->m_key;

        remove(successor);

        node->m_key = successor_key;
        parent = node;
    }

    while (parent) {
        updateHeuristics(parent);
        balance(parent);

        parent = parent->m_parent.lock();
    }
}

auto AVLTree::mergeAVLWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> std::shared_ptr<Node> {  // NOLINT(performance-unnecessary-value-param, misc-no-recursion)
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

        auto temp = max(v_1);
        remove(temp);

        if (temp) {
            temp->m_parent.reset();
            temp->m_left.reset();
            temp->m_right.reset();
        }

        m_root = mergeAVLWithRoot(m_root, v_2, temp);
    } else if (v_2) {
        m_root = v_2;
    } else {
        return nullptr;
    }

    updateHeuristics(m_root);
    return m_root;
}

auto AVLTree::split(std::shared_ptr<Node> node_v, key_type key) -> std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>> {  // NOLINT(performance-unnecessary-value-param, misc-no-recursion)
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
