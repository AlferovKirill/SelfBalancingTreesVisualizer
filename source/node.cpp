#include "node.hpp"

Node::Node(key_type key)
    : m_key(key)
    , m_sum(key) {}

Node::Node(key_type key, std::weak_ptr<Node> parent)
    : m_key(key)
    , m_sum(key)
    , m_parent(std::move(parent)) {}
