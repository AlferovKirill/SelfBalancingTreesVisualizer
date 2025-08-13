#include <cstdint>
#include <deque>

struct NodeGuiDto {
    using key_type = int64_t;

    key_type key {0};
    key_type sum {0};

    int64_t depth {0};
    int64_t horizontal_offset {0};

    struct {
        bool exists {false};

        int64_t depth {0};
        int64_t horizontal_offset {0};
    } left, right;
};

struct FlattenedTreeGuiDto {
    int64_t width {0};
    int64_t height {0};

    std::deque<NodeGuiDto> nodes;
};