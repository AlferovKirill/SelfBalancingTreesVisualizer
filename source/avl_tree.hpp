#include <cmath>

#include "node.hpp"
#include "node_gui_dto.hpp"

class AVLTree {
public:
    using key_type = Node::key_type;

    auto insert(key_type key) -> void;
    auto find(key_type key) const -> bool;
    auto remove(key_type key) -> void;

    auto max() const -> key_type;
    auto min() const -> key_type;
    
    auto sum(key_type left_key, key_type right_key) -> key_type;

    auto flatten() const -> FlattenedTreeGuiDto;

private:
    std::shared_ptr<Node> m_root;

    static auto updateHeuristics(std::shared_ptr<Node> node) -> void;
    static auto max(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    static auto min(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    static auto mergeWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> void;
    static auto disconnectNodeFromParent(std::shared_ptr<Node> node) -> void;

    auto balance(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto leftRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto rightRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto findNode(key_type key) const -> std::shared_ptr<Node>;
    auto remove(std::shared_ptr<Node> node) -> void;
    auto mergeAVLWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto merge(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2) -> std::shared_ptr<Node>;
    auto split(std::shared_ptr<Node> node_v, key_type key) -> std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>;
};
