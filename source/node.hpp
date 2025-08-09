#include <cstdint>
#include <memory>

template <typename KeyType>
struct Node {
    using key_type = KeyType;

    explicit Node(KeyType key);
    Node(KeyType key, std::weak_ptr<Node> parent);

    KeyType m_key;
    KeyType m_sum;

    int64_t m_height {1};
    size_t m_size {1};

    std::weak_ptr<Node> m_parent;
    std::shared_ptr<Node> m_left, m_right;
};

template <typename KeyType>
Node<KeyType>::Node(key_type key)
    : m_key(key)
    , m_sum(key) {}

template <typename KeyType>
Node<KeyType>::Node(key_type key, std::weak_ptr<Node> parent)
    : m_key(key)
    , m_sum(key)
    , m_parent(std::move(parent)) {}
