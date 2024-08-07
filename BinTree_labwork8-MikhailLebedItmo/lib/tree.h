#pragma once
#include <type_traits>
#include <iterator>
#include <memory>
#include <iostream>

enum class WalkType {
    PreOrder,
    InOrder,
    PostOrder
};

template <typename T, WalkType WT>
class BinaryTreeIterator;

struct BaseNode {
    BaseNode* left = nullptr;
    BaseNode* right = nullptr;
    BaseNode* parent = nullptr;
};

template <typename T>
struct Node;

template <typename T,
        WalkType WT = WalkType::InOrder,
        typename Compare = std::less<T>,
        typename Alloc = std::allocator<T>>
class BinaryTree {
public:
    friend void swap(BinaryTree& left, BinaryTree& right) {
        left.swap(right);
    }

    using key_type = const T;
    using value_type = const T;
    using size_type = size_t;
    using difference_type = int;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Alloc;
    using reference = T&;
    using const_reference = const T&;
    using iterator = BinaryTreeIterator<T, WT>;
    using const_iterator = BinaryTreeIterator<T, WT>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = Node<T>;
    using insert_return_type = std::pair<iterator, bool>;

    explicit BinaryTree(const Compare& comp = Compare(),
                        const Alloc& alloc = Alloc()):
            compare_(comp),
            node_allocator_(),
            size_(0),
            end_node_({&end_node_, &end_node_, &end_node_}) {}

    explicit BinaryTree(const Alloc& alloc)
        : BinaryTree(Compare(), alloc) {}

    BinaryTree(const BinaryTree& other) 
            : node_allocator_(alloc_traits::select_on_container_copy_construction(other.node_allocator_))
            , compare_(other.compare_) {
        copy(other);
    }

    BinaryTree(const BinaryTree& other, const Alloc& alloc)
            : BinaryTree(alloc) {
        copy(other);
    }

    template<typename InputIterator>
    BinaryTree(InputIterator first,
               InputIterator last,
               const Compare& comp = Compare(),
               const Alloc& alloc = Alloc())
       : BinaryTree(comp, alloc) {
        for (;first != last; ++first) {
            insert(*first);
        }
    }

    template<typename InputIterator>
    BinaryTree(InputIterator first,
               InputIterator last,
               const Alloc& alloc)
       : BinaryTree(first, last, Compare(), alloc) {}

    BinaryTree(std::initializer_list<T> init,
               const Compare& comp = Compare(),
               const Alloc& alloc = Alloc())
       : BinaryTree(init.begin(), init.end(), comp, alloc) {}

    BinaryTree(std::initializer_list<T> init,
               const Alloc& alloc)
       : BinaryTree(init, Compare(), alloc) {}

    BinaryTree& operator=(BinaryTree other) {
        swap(*this, other);
        return *this;
    }

    iterator begin() const {
        if constexpr(WT == WalkType::PreOrder || WT == WalkType::PostOrder) {
            return end_node_.left;
        } else {
            return end_node_.right;
        }
    }

    iterator end() const {
        return &end_node_;
    }

    iterator cbegin() const {
        return begin();
    }

    iterator cend() const {
        return end();
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    reverse_iterator rend() const {
        return reverse_iterator(end());
    }

    const_reverse_iterator crend() const {
        return rend();
    }


    bool operator==(const BinaryTree& other) const {
        if (size_ != other.size_) {
            return false;
        }

        return std::equal(begin(), end(), other.begin());
    }

    bool operator!=(const BinaryTree& other) const {
        return !(*this == other);
    }

    std::pair<iterator, bool> insert(const_reference value) {
        if (size_ == 0) {
            Node<T>* new_node = alloc_traits::allocate(node_allocator_, 1);
            alloc_traits::construct(node_allocator_, new_node, value);
            if constexpr(WT == WalkType::PreOrder) {
                new_node->left = &end_node_;
            }
            new_node->parent = &end_node_;
            end_node_.parent = new_node;
            end_node_.left = new_node;
            end_node_.right = new_node;
            ++size_;
            return {iterator(new_node), true};
        }

        return insert(value, get_root());
    }

    const_iterator find(const_reference value) const {
        return find(value, get_root());
    }

    size_t erase(const_reference value) {
        auto node = find(value, get_root());
        if (node == &end_node_) {
            return 0;
        }
        erase(node);

        return 1;
    }

    iterator erase(iterator it) {
        auto node = const_cast<BaseNode*>(it.current);
        iterator response = it;
        ++response;
        cut(node);
        --size_;
        if (size_ != 0) {
            update_left();
            update_right();
        }        
        alloc_traits::destroy(node_allocator_, static_cast<Node<T>*>(node));
        alloc_traits::deallocate(node_allocator_, static_cast<Node<T>*>(node), 1);

        return response;
    }

    void merge(BinaryTree& other) {
        auto it = other.begin();
        while (it.current != other.end()) {
            insert(*it);
            it = other.erase(it);
        }
    }

    void swap(BinaryTree& other) {
        using std::swap;
        swap(size_, other.size_);
        swap(compare_, other.compare_);
        swap(node_allocator_, other.node_allocator_);
        swap(end_node_, other.end_node_);
    }

    size_type size() const {
        return size_;
    }

    size_type max_size() const {
        return 4000;
    }

    bool empty() const {
        return size_ == 0;
    }

    key_compare key_comp() const {
        return compare_;
    }

    value_compare value_comp() const {
        return value_comp();
    };

    void clear() {
        if (size_ == 0) {
            return;
        }
        recursive_free(get_root());
        size_ = 0;
        end_node_ = {&end_node_, &end_node_, &end_node_};
    }

    ~BinaryTree() {
        clear();
    }


private:
    using node_allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<node_type>;
    using alloc_traits = std::allocator_traits<node_allocator_type>;
    // [[no_unique_address]] node_allocator_type node_allocator_;
    node_allocator_type node_allocator_;
    Compare compare_;
    BaseNode end_node_;
    size_t size_;

    void update_left() {
        auto node = get_root();
        if constexpr(WT == WalkType::PreOrder || WT == WalkType::InOrder) {
            while (node->left && node->left != &end_node_) {
                node = node->left;
            }
            end_node_.right = node;
        } else {
            while ((node->left && node->left != &end_node_) ||
              (node->right && node->right != &end_node_)) { 
                if (node->left && node->left != &end_node_) {
                    node = node->left;
                    continue;
                }
                node = node->right;
            }
            end_node_.left = node;
        }

    }

    void update_right() {
        auto node = get_root();
        if constexpr(WT == WalkType::PreOrder) {
            if (end_node_.parent != &end_node_ &&
                    end_node_.parent->left == &end_node_) {
                end_node_.parent->left = nullptr;
            }
            while ((node->left && node->left != &end_node_) ||
              (node->right && node->right != &end_node_)) { 
                if (node->right && node->right != &end_node_) {
                    node = node->right;
                    continue;
                }
                node = node->left;
            }
            end_node_.parent = node;
            node->left = &end_node_;
        } else {
            while (node->right && node->right != &end_node_) {
                node = node->right;
            }
            end_node_.parent = node;
        }
    }

    BaseNode* get_root() {
        if constexpr(WT == WalkType::PreOrder || WT == WalkType::InOrder ) {
            return end_node_.left;
        } else {
            return end_node_.right;
        }
    }

    const BaseNode* get_root() const {
        if constexpr(WT == WalkType::PreOrder || WT == WalkType::InOrder ) {
            return end_node_.left;
        } else {
            return end_node_.right;
        }
    }

    BaseNode* get_next(BaseNode* node) {
        auto next = node->right;
        while (next->left && next->left != &end_node_) {
            next = node->left;
        }

        return next;
    }

    const BaseNode* get_next(const BaseNode* node) const {
        auto next = node->right;
        while (next->left && next->left != &end_node_) {
            next = node->left;
        }

        return next;
    }

    BaseNode* find(const_reference value, BaseNode* cur_node) {
        if (!cur_node || cur_node == &end_node_) {
            return &end_node_;
        }
        const auto& cur_value = static_cast<Node<T>*>(cur_node)->value;
        if (compare_(value, cur_value)) {
            return find(value, cur_node->left);
        }
        if (compare_(cur_value, value)) {
            return find(value, cur_node->right);
        }

        return cur_node;
    }

    const BaseNode* find(const_reference value, const BaseNode* cur_node) const {
        if (!cur_node || cur_node == &end_node_) {
            return &end_node_;
        }
        const auto& cur_value = static_cast<const Node<T>*>(cur_node)->value;
        if (compare_(value, cur_value)) {
            return find(value, cur_node->left);
        }
        if (compare_(cur_value, value)) {
            return find(value, cur_node->right);
        }

        return cur_node;
    }

    const_iterator lower_bound(const_reference value) const {
        const BaseNode* cur_node = get_root();
        const_iterator response = cur_node;
        while (cur_node != &end_node_) {
            const_reference cur_value = static_cast<Node<T>>(cur_node)->value;
            if (compare_(cur_value, value)) {
                //cur_value < value
                cur_node = cur_node->right;
            } else {
                // cur_value >= value
                response = cur_node;
                cur_node = cur_node->left;
            }
        }

        return response;
    }

    const_iterator upper_bound(const_reference value) const {
        const BaseNode* cur_node = get_root();
        const_iterator response = cur_node;
        while (cur_node != &end_node_) {
            const_reference cur_value = static_cast<Node<T>>(cur_node)->value;
            if (compare_(value, cur_value)) {
                //value < cur_value
                response = cur_node;
                cur_node = cur_node->left;
            } else {
                // value >= cur_value
                cur_node = cur_node->right;
            }
        }

        return response;
    }

    size_t count(const_reference value) const {
        auto node = find(value);
        if (node == &end_node_) {
            return 0;
        }

        return 1;
    }

    bool contains(const_reference value) const {
        return count(value);
    }

    std::pair<iterator, bool> insert(const_reference value, BaseNode* cur_node) {
        const auto& cur_value = static_cast<Node<T>*>(cur_node)->value;
        if (compare_(value, cur_value)) {
            if (!cur_node->left || cur_node->left == &end_node_) {
                Node<T>* new_node = alloc_traits::allocate(node_allocator_, 1);
                alloc_traits::construct(node_allocator_, new_node, value);
                cur_node->left = new_node;
                new_node->parent = cur_node;
                ++size_;
                update_left();
                update_right();

                return {iterator(new_node), true};
            }

            return insert(value, cur_node->left);
        }
        if (compare_(cur_value, value)) {
            if (!cur_node->right || cur_node->right == &end_node_) {
                Node<T>* new_node = alloc_traits::allocate(node_allocator_, 1);
                alloc_traits::construct(node_allocator_, new_node, value);
                cur_node->right = new_node;
                new_node->parent = cur_node;
                ++size_;
                update_left();
                update_right();

                return {iterator(new_node), true};
            }

            return insert(value, cur_node->right);
        }

        return {iterator(cur_node), false};
    }

    void cut(BaseNode* node) {
        // cut node but not dealocate, and not update end_node_'s pointers
        if (size_ == 1) {
            end_node_ = {&end_node_, &end_node_, &end_node_};
            return;
        }
        auto update_parent = [](BaseNode* old_sun, BaseNode* new_sun) {
            if (old_sun->parent->right == old_sun) {
                old_sun->parent->right = new_sun;
            }
            if (old_sun->parent->left == old_sun) {
                old_sun->parent->left = new_sun;
            }
        };
        if ((!node->right || node->right == &end_node_) &&
            (!node->left || node->right == &end_node_)) {
            // node is leaf
            update_parent(node, nullptr);
            return;
        }
        if (!node->right || node->right == &end_node_) {
            // node has only left child
            update_parent(node, node->left);
            node->left->parent = node->parent;
            return;
        }
        if (!node->left || node->left == &end_node_) {
            // node has only right child
            update_parent(node, node->right);
            node->right->parent = node->parent;
            return;
        }
        auto next = get_next(node);
        node->left->parent = next;
        next->left = node->left;
        if (next->parent->left == next) {
            if (next->right && next->right != &end_node_) {
                next->right->parent = next->parent;
            }
            next->parent->left = next->right;
            next->right = node->right;
            next->right->parent = next;
            next->parent = node->parent;
            update_parent(node, next);
            return;
        }
        update_parent(node, next);
        next->parent = node->parent;
    }

    void recursive_free(BaseNode* root) {
        if (root->right && root->right != &end_node_){
            recursive_free(root->right);
        }
//        root->right = nullptr;
        if (root->left && root->left != &end_node_) {
            recursive_free(root->left);
        }

        alloc_traits::destroy(node_allocator_, static_cast<Node<T>*>(root));
        alloc_traits::deallocate(node_allocator_, static_cast<Node<T>*>(root), 1);
    }

    void recursive_copy(BaseNode* this_cur, const BaseNode* other_cur, const BaseNode* other_end) {
        if (other_cur->left && other_cur->left != other_end) {
            Node<T>* new_node = alloc_traits::allocate(node_allocator_, 1);
            alloc_traits::construct(node_allocator_, new_node,
                                    static_cast<Node<T>*>(other_cur->left)->value);
            this_cur->left = new_node;
            new_node->parent = this_cur;
            recursive_copy(new_node, other_cur->left, other_end);
        } else {
            this_cur->left = nullptr;
        }
        if (other_cur->right && other_cur->right != other_end) {
            Node<T>* new_node = alloc_traits::allocate(node_allocator_, 1);
            alloc_traits::construct(node_allocator_, new_node,
                                    static_cast<Node<T>*>(other_cur->right)->value);
            this_cur->right = new_node;
            new_node->parent = this_cur;
            recursive_copy(new_node, other_cur->right, other_end);
        } else {
            this_cur->right = nullptr;
        }
    }

    void copy(const BinaryTree& other) {
        size_ = other.size_;
        end_node_ = {&end_node_, &end_node_, &end_node_};
        if (size_ == 0) {
            return;
        }
        if constexpr(WT == WalkType::PreOrder || WT == WalkType::InOrder ) {
            Node<T>* root = alloc_traits::allocate(node_allocator_, 1);
            alloc_traits::construct(node_allocator_, root,
                                    static_cast<const Node<T>*>(other.end_node_.left)->value);
            root->parent = &end_node_;
            end_node_.left = root;
            recursive_copy(root, other.end_node_.left, &other.end_node_);
        } else {
            Node<T>* root = alloc_traits::allocate(node_allocator_, 1);
            alloc_traits::construct(node_allocator_, root,
                                    static_cast<const Node<T>*>(other.end_node_.right)->value);
            root->parent = &end_node_;
            end_node_.right = root;
            recursive_copy(root, other.end_node_.right, &other.end_node_);
        }
        update_left();
        update_right();
    }
};

template <typename T>
struct Node : BaseNode {
    typename BinaryTree<T>::value_type value;
    
    Node(const T& value): BaseNode(), value(value){}
};

template <typename T>
class BaseBinaryTreeIterator {
public:
    BaseBinaryTreeIterator(const BaseNode* node): current(node) {}

    const T& operator*() const {
        return static_cast<const Node<T>*>(current)->value;
    }

    friend bool operator==(const BaseBinaryTreeIterator& lhs, const BaseBinaryTreeIterator& rhs) {
        return lhs.current == rhs.current;
    }

    friend bool operator!=(const BaseBinaryTreeIterator& lhs, const BaseBinaryTreeIterator& rhs) {
        return lhs.current != rhs.current;
    }
protected:
    const BaseNode* current;
};

template <typename T>
class BinaryTreeIterator<T, WalkType::PreOrder>: public BaseBinaryTreeIterator<T> {
public:
    friend class BinaryTree<T, WalkType::PreOrder>;

    BinaryTreeIterator& operator++() {
        if (current->left) {
            current = current->left;
            return *this;
        }
        if (current->right) {
            current = current->right;
            return *this;
        }
        while (current->parent->right == current) {
            current = current->parent;
        }
        current = current->parent;
        while (current->right == nullptr) {
            current = current->parent;
        }
        current = current->right;

        return *this;
    }

    BinaryTreeIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    BinaryTreeIterator& operator--() {
        if (!current->parent->left || current->parent->left == current) {
            current = current->parent;
            return *this;
        }
        current = current->parent->left;
        while (current->right) {
            current = current->right;
        }


        return *this;
    }

    BinaryTreeIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }
private:
    using BaseBinaryTreeIterator<T>::current;

    BinaryTreeIterator(const BaseNode* node): BaseBinaryTreeIterator<T>(node) {}
};

template <typename T>
class BinaryTreeIterator<T, WalkType::InOrder>: public BaseBinaryTreeIterator<T> {
public:
    friend class BinaryTree<T, WalkType::InOrder>;

    BinaryTreeIterator& operator++() {
        if (current->right) {
            current = current->right;
            while (current->left) {
                current = current->left;
            }
            return *this;
        }
        while (current->parent->left != current) {
            current = current->parent;
        }
        current = current->parent;
        return *this;
    }

    BinaryTreeIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    BinaryTreeIterator& operator --() {
        if (current->left) {
            current = current->left;
            while (current->right) {
                current = current->right;
            }
            return *this;
        }
        while (current->parent->right != current) {
            current = current->parent;
        }
        current = current->parent;
        return *this;
    }

    BinaryTreeIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }
    BinaryTreeIterator(const BaseNode* node): BaseBinaryTreeIterator<T>(node) {}
private:
    using BaseBinaryTreeIterator<T>::current;
};

template <typename T>
class BinaryTreeIterator<T, WalkType::PostOrder>: public BaseBinaryTreeIterator<T> {
public:
    friend class BinaryTree<T, WalkType::PostOrder>;

    BinaryTreeIterator& operator++() {
        if (current->parent->right == current) {
            current = current->parent;
            return *this;
        }
        current = current->parent->right;
        while (current->left || current->right) {
            if (current->left) {
                current = current->left;
                continue;
            }
            current = current->right;

        }
        return *this;
    }

    BinaryTreeIterator operator++(int) {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    BinaryTreeIterator& operator --() {
        if (current->right) {
            current = current->right;
            return *this;
        }
        current = current->left;

        return *this;
    }

    BinaryTreeIterator operator--(int) {
        auto copy = *this;
        --(*this);
        return copy;
    }
private:
    using BaseBinaryTreeIterator<T>::current;

    BinaryTreeIterator(const BaseNode* node): BaseBinaryTreeIterator<T>(node) {}
};