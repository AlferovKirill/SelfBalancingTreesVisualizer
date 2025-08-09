#include <vector>

#include "node.hpp"
#include "node_dto.hpp"

template <typename KeyType>
class AVLTree {
public:
    using node_type = Node<KeyType>;
    using node_dto_type = NodeDto<KeyType>;

    auto insert(KeyType key) -> void;
    auto find(KeyType key) const -> bool;
    auto remove(KeyType key) -> void;

    auto max() const -> KeyType;
    auto min() const -> KeyType;

    auto sum(KeyType left_key, KeyType right_key) -> KeyType;

    auto flattenInOrder() const -> std::vector<node_dto_type>;

private:
    std::shared_ptr<node_type> m_root;

    static auto updateHeuristics(std::shared_ptr<node_type> node) -> void;
    static auto max(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;
    static auto min(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;
    static auto mergeWithRoot(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2, std::shared_ptr<node_type> node) -> void;
    static auto disconnectNodeFromParent(std::shared_ptr<node_type> node) -> void;

    auto balance(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;
    auto leftRotate(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;
    auto rightRotate(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;

    auto findNode(KeyType key) const -> std::shared_ptr<node_type>;
    auto remove(std::shared_ptr<node_type> node) -> void;

    auto mergeAVLWithRoot(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2, std::shared_ptr<node_type> node) -> std::shared_ptr<node_type>;
    auto merge(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2) -> std::shared_ptr<node_type>;

    auto split(std::shared_ptr<node_type> node_v, KeyType key) -> std::pair<std::shared_ptr<node_type>, std::shared_ptr<node_type>>;
};

// ---- Definitions ----

#include <cmath>
#include <stack>

template <typename KeyType>
auto AVLTree<KeyType>::insert(KeyType key) -> void {
    if (m_root) {
        auto parent = m_root;

        while (true) {
            if (key > parent->m_key) {
                if (!parent->m_right) {
                    parent->m_right = std::make_shared<node_type>(key, parent);
                    break;
                } else {
                    parent = parent->m_right;
                }
            } else if (key < parent->m_key) {
                if (!parent->m_left) {
                    parent->m_left = std::make_shared<node_type>(key, parent);
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
        m_root = std::make_shared<node_type>(key);
    }
}

template <typename KeyType>
auto AVLTree<KeyType>::find(KeyType key) const -> bool {
    return static_cast<bool>(findNode(key));
}

template <typename KeyType>
auto AVLTree<KeyType>::remove(KeyType key) -> void {
    remove(findNode(key));
}

template <typename KeyType>
auto AVLTree<KeyType>::max() const -> KeyType {
    auto node = max(m_root);
    return (node ? node->m_key : 0);
}

template <typename KeyType>
auto AVLTree<KeyType>::min() const -> KeyType {
    auto node = min(m_root);
    return (node ? node->m_key : 0);
}

template <typename KeyType>
auto AVLTree<KeyType>::sum(KeyType left_key, KeyType right_key) -> KeyType {
    auto [left, temp] = split(m_root, left_key - 1);

    m_root = temp;

    auto [middle, right] = split(temp, right_key);

    auto interval_sum = middle ? middle->m_sum : 0;

    temp = merge(left, middle);
    m_root = merge(temp, right);

    return interval_sum;
}

template <typename KeyType>
auto AVLTree<KeyType>::flattenInOrder() const -> std::vector<node_dto_type> {
    std::vector<node_dto_type> result;
    result.reserve(m_root ? static_cast<size_t>(m_root->m_size) : 0);

    std::stack<std::shared_ptr<node_type>> stack;
    auto current = m_root;

    while (current || !stack.empty()) {
        while (current) {
            stack.push(current);
            current = current->m_left;
        }

        current = stack.top();
        stack.pop();

        node_dto_type node_gui_dto = {
            current->m_key,
            current->m_sum,
            current->m_height,
            current->m_size,
            0,
            0
        };
        
        result.push_back(std::move(node_gui_dto));
        current = current->m_right;
    }

    return result;
}

template <typename KeyType>
auto AVLTree<KeyType>::updateHeuristics(std::shared_ptr<node_type> node) -> void {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::max(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {
    while (node && node->m_right) {
        node = node->m_right;
    }

    return node;
}

template <typename KeyType>
auto AVLTree<KeyType>::min(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {
    while (node && node->m_left) {
        node = node->m_left;
    }

    return node;
}

template <typename KeyType>
auto AVLTree<KeyType>::mergeWithRoot(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2, std::shared_ptr<node_type> node) -> void {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::disconnectNodeFromParent(std::shared_ptr<node_type> node) -> void {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::balance(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {  // NOLINT(readability-function-cognitive-complexity)
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

template <typename KeyType>
auto AVLTree<KeyType>::leftRotate(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::rightRotate(std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::findNode(KeyType key) const -> std::shared_ptr<node_type> {
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

template <typename KeyType>
auto AVLTree<KeyType>::remove(std::shared_ptr<node_type> node) -> void {  // NOLINT(readability-function-cognitive-complexity, misc-no-recursion, performance-unnecessary-value-param)
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
        KeyType successor_key = successor->m_key;

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

template <typename KeyType>
auto AVLTree<KeyType>::mergeAVLWithRoot(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2, std::shared_ptr<node_type> node) -> std::shared_ptr<node_type> {  // NOLINT(misc-no-recursion, performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::merge(std::shared_ptr<node_type> v_1, std::shared_ptr<node_type> v_2) -> std::shared_ptr<node_type> {  // NOLINT(performance-unnecessary-value-param)
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

template <typename KeyType>
auto AVLTree<KeyType>::split(std::shared_ptr<node_type> node_v, KeyType key) -> std::pair<std::shared_ptr<node_type>, std::shared_ptr<node_type>> {  // NOLINT(misc-no-recursion, performance-unnecessary-value-param)
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
