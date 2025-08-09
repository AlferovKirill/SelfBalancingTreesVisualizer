#pragma once

#include <QAbstractItemModel>

#include "node.hpp"

class AVLTree : public QAbstractItemModel {
    Q_OBJECT

    Q_PROPERTY(int maxDepth READ getMaxDepth NOTIFY maxDepthChanged FINAL)
public:
    enum class roles_enum {
        key = Qt::UserRole + 1,
        is_left_child,
    };
    Q_ENUM(roles_enum)

    explicit AVLTree(QObject* parent = nullptr);

    auto data(const QModelIndex& index, int role) const -> QVariant override;
    auto index(int row, int column, const QModelIndex& parent_index) const -> QModelIndex override;
    auto parent(const QModelIndex& index) const -> QModelIndex override;
    auto rowCount(const QModelIndex& parent_index) const -> int override;
    auto columnCount(const QModelIndex& parent_index) const -> int override;

    auto insert(Node::key_type key) -> void;
    auto find(Node::key_type key) const -> bool;
    auto remove(Node::key_type key) -> void;

    auto max() const -> Node::key_type;
    auto min() const -> Node::key_type;

    auto sum(Node::key_type left_key, Node::key_type right_key) -> Node::key_type;

    auto getMaxDepth() const -> int;

signals:
    auto maxDepthChanged() -> void;

protected:
    auto roleNames() const -> QHash<int, QByteArray> override;

private:
    std::shared_ptr<Node> m_dummy_root;
    std::shared_ptr<Node> m_root;

    static auto updateHeuristics(std::shared_ptr<Node> node) -> void;
    static auto max(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    static auto min(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    static auto mergeWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> void;
    static auto disconnectNodeFromParent(std::shared_ptr<Node> node) -> void;

    auto balance(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto leftRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto rightRotate(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;

    auto findNode(Node::key_type key) const -> std::shared_ptr<Node>;
    auto remove(std::shared_ptr<Node> node) -> void;

    auto mergeAVLWithRoot(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2, std::shared_ptr<Node> node) -> std::shared_ptr<Node>;
    auto merge(std::shared_ptr<Node> v_1, std::shared_ptr<Node> v_2) -> std::shared_ptr<Node>;

    auto split(std::shared_ptr<Node> node_v, Node::key_type key) -> std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>;
};
