#include <vector>

template <typename T>
class vector {
    vector() {

    }
    vector(size_t size, const T& default_val): v_(size, default_val) {}
private:
    std::vector<T> v_;
    size_t row_size_;
    size_t col_size_; // one of them is 1 by default
};
