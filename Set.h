#include <cstddef>
#include <set>

template<class ValueType>  // only "<" for ValueType
class Set {
public:
    Set(): size_(0) {}

    template<typename Iterator>
    Set(Iterator first, Iterator last): {
        Set();
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    Set(std::initializer_list<ValueType> elems): {
        Set(elems.begin(), elems.end());  // does it work?
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void insert(const ValueType &elem) {
        add_(elem, root_);
    }

    void erase(const ValueType &elem) {
        remove_(elem, root_);
    }

    class iterator {
     protected:
        Node *pointer; 
    }

    iterator begin() const {
        return data_.cbegin();
    }

    iterator end() const {
        return data_.cend();
    }

    iterator find(const ValueType& elem) const {
        return data_.find(elem);
    }

    iterator lower_bound(const ValueType& elem) const {
        return data_.lower_bound(elem);
    }
private:
    struct Node {
        ValueType value;
        Node *left;
        Node *right;
        size_t height;

        Node(const ValueType &value):
            this.value(value),
            height(1),
            left(nullptr),
            right(nullptr),
        {}
    };

    size_t getHeight_(Node *v) {
        if (!v) {
            return 0;
        }
        return v->height;
    }

    size_t getBalanceFactor_(Node *v) {
        if (!v) {
            return 0;
        }
        return getHeight_(v->right) - getHeight_(v->left);
    }

    void update_(Node *v) {
        if (!v) {
            return;
        }
        v->height = max(getHeight_(v->left), getHeight_(v->right)) + 1;
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
            v = new Node(elem);
            return v;
        }
        if (v->value == elem) {
            return v;
        }
        if (elem > v->value) {
            v->right = add_(elem, v->right);
        } else {
            v->left = add_(elem, v->left);
        }
        return balance_(v);
    }

    Node * getLeftVertextInSubtree_(Node *v) {
        if (!v->left) {  // v != nullptr
            return v;
        }
        return getLeftVertexInSubtree_(v->left);
    }

    Node * remove_(const ValueType &elem, Node *v) {
        if (!v) {
            return v;
        }
        if (elem == v->value) {
            if (v->left && v->right) {
                v->value = getLeftVertexInSubtree_(v->right)->value;
                v->right = remove_(v->value, v->right)
            } else if (v->left) {
                v = v->left;
            } else {
                v = v->right;
            }
        } else if (elem < v->value) {
            v->left = remove_(elem, v->left)
        } else {
            v->right = remove_(elem, v->right);
        }
        return balance_(v);
    }

    Node *root_;
    size_t size_;
    std::set<ValueType> data_;  // delete
};