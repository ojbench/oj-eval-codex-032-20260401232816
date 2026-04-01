// Implementation of a minimal Python-like list (int or nested list)
#ifndef PYLIST_H
#define PYLIST_H

#include <iostream>
#include <vector>
#include <memory>

class pylist {
private:
    bool is_int_;
    int int_val_;
    std::shared_ptr<std::vector<pylist>> list_;

public:
    // Default: empty list
    pylist() : is_int_(false), int_val_(0), list_(std::make_shared<std::vector<pylist>>()) {}

    // Construct from int
    pylist(int v) : is_int_(true), int_val_(v), list_(nullptr) {}

    // Rule of zero: default copy/move are fine (shared_ptr shares lists)
    pylist(const pylist&) = default;
    pylist(pylist&&) noexcept = default;
    pylist& operator=(const pylist&) = default;
    pylist& operator=(pylist&&) noexcept = default;

    // Assign from int: becomes an int value
    pylist& operator=(int v) {
        is_int_ = true;
        int_val_ = v;
        list_.reset();
        return *this;
    }

    // Append an int (O(1) amortized)
    void append(int v) {
        ensure_list_();
        list_->push_back(pylist(v));
    }

    // Append a pylist (shares underlying list pointer if x is a list)
    void append(const pylist &x) {
        ensure_list_();
        list_->push_back(x);
    }

    // Pop last element and return it (O(1) amortized). If empty, return empty list.
    pylist pop() {
        ensure_list_();
        if (list_->empty()) {
            return pylist();
        }
        pylist ret = list_->back();
        list_->pop_back();
        return ret;
    }

    // Indexing (non-const) (O(1))
    pylist &operator[](size_t i) {
        ensure_list_();
        return (*list_)[i];
    }

    // Indexing (const) (O(1))
    const pylist &operator[](size_t i) const {
        // For const access, assume it's a list; if not, behavior is undefined per problem guarantees
        return (*list_)[i];
    }

    // Implicit conversion to int for arithmetic/bitwise usage on int elements
    explicit operator int() const {
        return int_val_;
    }

    // Stream output
    friend std::ostream &operator<<(std::ostream &os, const pylist &v) {
        if (v.is_int_) {
            os << v.int_val_;
        } else {
            os << '[';
            if (v.list_) {
                for (size_t i = 0; i < v.list_->size(); ++i) {
                    if (i) os << ", ";
                    os << (*v.list_)[i];
                }
            }
            os << ']';
        }
        return os;
    }

private:
    inline void ensure_list_() {
        if (!is_int_) {
            if (!list_) list_ = std::make_shared<std::vector<pylist>>();
        } else {
            // If this is currently an int and list operation is attempted,
            // convert to an empty list as a safe fallback (not expected by tests)
            is_int_ = false;
            list_ = std::make_shared<std::vector<pylist>>();
        }
    }
};

#endif // PYLIST_H

