#include <QAbstractListModel>
#include <cmath>

#include "node.hpp"
#include "node_gui_dto.hpp"

class AVLTree : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int height READ getHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(int width READ getWidth NOTIFY widthChanged FINAL)

public:
    enum roles_enum {
        key_role = Qt::UserRole + 1,
        sum_role,
        depth_role,
        horizontal_offset_role,

        left_exists_role,
        left_depth_role,
        left_horizontal_offset_role,

        right_exists_role,
        right_depth_role,
        right_horizontal_offset_role,
    };
    Q_ENUM(roles_enum)

    using key_type = Node::key_type;

    explicit AVLTree(QObject* parent = nullptr);

    Q_INVOKABLE void insert(key_type key);
    Q_INVOKABLE bool find(key_type key) const; // NOLINT(modernize-use-trailing-return-type)
    Q_INVOKABLE void remove(key_type key);

    auto max() const -> key_type;
    auto min() const -> key_type;

    auto sum(key_type left_key, key_type right_key) -> key_type;
    auto size() const -> int64_t;

    auto flatten() const -> FlattenedTreeGuiDto;

    auto rowCount(const QModelIndex& parent) const -> int override;
    auto data(const QModelIndex& index, int role) const -> QVariant override;
    auto roleNames() const -> QHash<int, QByteArray> override;

    auto getHeight() const -> int;
    auto getWidth() const -> int;

signals:
    auto heightChanged() -> void;
    auto widthChanged() -> void;

private:
    std::shared_ptr<Node> m_root;
    FlattenedTreeGuiDto m_flattened_tree_cache;

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
