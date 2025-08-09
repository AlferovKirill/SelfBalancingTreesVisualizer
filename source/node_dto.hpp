#include <cstddef>
#include <cstdint>

template <typename KeyType>
struct NodeDto {
    KeyType key;
    KeyType sum;

    int64_t height;

    size_t size;
    size_t top_offset;
    size_t right_offset;
};