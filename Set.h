#include <cstddef>
#include <list>
#include <iostream>
#include <cmath>
#include <cassert>

template<class ValueType>  // only "<" for ValueType
class Set {
 public:
    typedef typename std::list<ValueType>::const_iterator iterator;
 
 private:
    struct Node {
        iterator it;
        ValueType value;
        size_t height;
        Node *left;
        Node *right;

        Node(const ValueType &value, const iterator &it):
            it(it),
            value(value),
            height(1),
            left(nullptr),
            right(nullptr)
        {}
    };

    Node *root_;
    size_t size_;
    std::list<ValueType> list_;

    template<typename T>
    bool isNotEqual(const T &a, const T &b) const {
        return a < b || b < a; 
    }

    template<typename T>
    bool isEqual(const T &a, const T &b) const {
        return !isNotEqual(a, b);
    }

 public:
    Set(): root_(nullptr), size_(0) {}

    template<typename Iterator>
    Set(Iterator first, Iterator last): Set() {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    Set(const std::initializer_list<ValueType> &elems):
        Set(elems.begin(), elems.end()) {}

    Set & operator = (const Set<ValueType> &other) {
        if (this == &other) {
            return *this;
        }
        deleteAll_();
        for (const auto &i : other) {
            insert(i);
        }
        return *this;
    }
    
    Set(const Set<ValueType> &other): Set() {
        for (const auto &i : other) {
            insert(i);
        }
    }

    ~Set() {
        deleteAll_();
    };

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t getHeight() const {
        if (!root_) {
            return 0;
        }
        return root_->height;
    }

    void insert(const ValueType &elem) {
        root_ = add_(elem, root_);
    }

    void erase(const ValueType &elem) {
        root_ = remove_(elem, root_);
    }

    iterator begin() const {
        return list_.cbegin();
    }

    iterator end() const {
        return list_.cend();
    }

    iterator find(const ValueType& elem) const {
        iterator it = lower_bound(elem);
        if (it != end() && isEqual(*it, elem)) {
            return it;
        } else {
            return end();
        }
    }

    iterator lower_bound(const ValueType& elem) const {
        iterator it = upper_bound(elem);
        if (it == begin()) {
            return it;
        }
        iterator itGreater = it--;
        if (*it < elem) {
            return itGreater;
        } else {
            return it;
        }
    }

    iterator upper_bound(const ValueType &elem) const {
        Node *ans = nullptr;
        getUpperBoundVertex_(elem, root_, ans);
        iterator res = end();
        if (ans != nullptr) {
            res = ans->it;
        }
        return res;
    }

 private:
    int getHeight_(Node *v) {
        if (!v) {
            return 0;
        }
        return v->height;
    }

    int getBalanceFactor_(Node *v) {
        if (!v) {
            return 0;
        }
        return getHeight_(v->right) - getHeight_(v->left);
    }

    void update_(Node *v) {
        if (!v) {
            return;
        }
        v->height = std::max(getHeight_(v->left), getHeight_(v->right)) + 1;
    }

    Node * rotateRight_(Node *v) {
        Node *u = v->left;
        v->left = u->right;
        u->right = v;
        update_(v);
        update_(u);
        return u;
    }

    Node * rotateLeft_(Node *v) {
        Node *u = v->right;
        v->right = u->left;
        u->left = v;
        update_(v);
        update_(u);
        return u;
    }

    Node * balance_(Node *v) {
        if (!v) {
            return v;
        }
        update_(v);
        if (getBalanceFactor_(v) == 2) {
            if (getBalanceFactor_(v->right) < 0) {
                v->right = rotateRight_(v->right);
            }
            return rotateLeft_(v);
        }
        if (getBalanceFactor_(v) == -2) {
            if (getBalanceFactor_(v->left) > 0) {
                v->left = rotateLeft_(v->left);
            }
            return rotateRight_(v);
        }
        return v;
    }

    Node * add_(const ValueType &elem, Node *v) {
        if (!v) {
            ++size_;
            iterator upper = upper_bound(elem);
            v = new Node(elem, list_.insert(upper, elem));
            return v;
        }
        if (isEqual(elem, v->value)) {
            return v;
        }
        if (elem < v->value) {
            v->left = add_(elem, v->left);
        } else {
            v->right = add_(elem, v->right);
        }
        return balance_(v);
    }

    Node * getLeftVertexInSubtree_(Node *v) {
        if (!v->left) {
            return v;
        }
        return getLeftVertexInSubtree_(v->left);
    }

    Node * remove_(const ValueType &elem, Node *v) {
        if (!v) {
            return v;
        }
        if (isEqual(elem, v->value)) {
            if (v->left && v->right) {
                auto u = getLeftVertexInSubtree_(v->right);
                v->value = u->value;
                swap(v->it, u->it);
                v->right = remove_(v->value, v->right);
            } else {
                --size_;
                Node *u = v; 
                if (v->left) {
                    v = v->left;
                } else {
                    v = v->right;
                }
                list_.erase(u->it);
                delete u;
            }
        } else if (elem < v->value) {
            v->left = remove_(elem, v->left);
        } else {
            v->right = remove_(elem, v->right);
        }
        return balance_(v);
    }

    void getUpperBoundVertex_(const ValueType &elem, Node *v, Node *&ans) const {
        if (!v) {
            return;
        }
        if (elem < v->value) {
            ans = v;
            getUpperBoundVertex_(elem, v->left, ans);
        } else {
            getUpperBoundVertex_(elem, v->right, ans);
        }
    }

    void deleteAll_() {
        delete_(root_);
        root_ = nullptr;
        size_ = 0;
        list_.clear();
    }

    void delete_(Node *v) {
        if (!v) {
            return;
        }
        Node *left = v->left;
        Node *right = v->right;
        delete v;
        delete_(left);
        delete_(right);
    }
};