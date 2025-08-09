#pragma once

#include <cstdint>
#include <memory>

struct Node {
    using key_type = int64_t;

    explicit Node(key_type key);
    Node(key_type key, std::weak_ptr<Node> parent);

    key_type m_key;
    key_type m_sum;

    int64_t m_height {1};
    size_t m_size {1};

    std::weak_ptr<Node> m_parent;
    std::shared_ptr<Node> m_left, m_right;
};
