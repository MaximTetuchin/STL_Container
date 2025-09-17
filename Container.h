/* Maxim Tetuchin tetuhin@inbox.ru | st128993@student.spbu.ru
        STL_container
*/
#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>
#include <iterator>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>

namespace stl_containers {

/**
 * @class CircularList
 * @brief A circular doubly-linked list container implementation in STL style
 * @tparam T Type of elements stored in the list
 * @tparam Allocator Allocator type for memory management (default: std::allocator<T>)
 * 
 * This class provides a circular doubly-linked list implementation with full STL compatibility.
 * It supports bidirectional iterators, exception safety, and follows RAII principles.
 */
template <typename T, typename Allocator = std::allocator<T>>
class CircularList {
public:
    // Type aliases for STL compliance
    using value_type = T;                              ///< Type of elements
    using allocator_type = Allocator;                  ///< Allocator type
    using size_type = std::size_t;                     ///< Type for size representation
    using difference_type = std::ptrdiff_t;            ///< Type for difference between iterators
    using reference = value_type&;                     ///< Reference to element type
    using const_reference = const value_type&;         ///< Const reference to element type
    using pointer = typename std::allocator_traits<Allocator>::pointer;          ///< Pointer to element type
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer; ///< Const pointer to element type

    /**
     * @struct Node
     * @brief Internal node structure for storing list elements
     */
    struct Node {
        T value;        ///< Stored value
        Node* next;     ///< Pointer to next node
        Node* prev;     ///< Pointer to previous node
        
        /**
         * @brief Constructor with copy semantics
         * @param val Value to copy into the node
         */
        Node(const T& val) : value(val), next(nullptr), prev(nullptr) {}
        
        /**
         * @brief Constructor with move semantics
         * @param val Value to move into the node
         */
        Node(T&& val) : value(std::move(val)), next(nullptr), prev(nullptr) {}
        
        /**
         * @brief Variadic constructor for perfect forwarding
         * @tparam Args Types of arguments to forward
         * @param args Arguments to forward to value constructor
         */
        template<typename... Args>
        Node(Args&&... args) : value(std::forward<Args>(args)...), next(nullptr), prev(nullptr) {}
    };

    // Forward declaration of const_iterator
    class const_iterator;

    /**
     * @class iterator
     * @brief Bidirectional iterator for CircularList
     */
    class iterator {
    public:
        // Iterator traits for STL compatibility
        using iterator_category = std::bidirectional_iterator_tag; ///< Iterator category
        using value_type = T;                                      ///< Value type
        using difference_type = std::ptrdiff_t;                    ///< Difference type
        using pointer = T*;                                        ///< Pointer type
        using reference = T&;                                      ///< Reference type

        /**
         * @brief Constructor
         * @param node Pointer to current node
         * @param temp_node Pointer to sentinel node (end marker)
         */
        iterator(Node* node = nullptr, Node* temp_node = nullptr) 
            : current_(node), temp_node_(temp_node) {}

        /**
         * @brief Dereference operator
         * @return Reference to the current element
         * @throw std::out_of_range if attempting to dereference end iterator
         */
        reference operator*() const { 
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot dereference end iterator");
            }
            return current_->value; 
        }
        
        /**
         * @brief Member access operator
         * @return Pointer to the current element
         * @throw std::out_of_range if attempting to access end iterator
         */
        pointer operator->() const { 
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot access end iterator");
            }
            return &current_->value; 
        }

        /**
         * @brief Pre-increment operator
         * @return Reference to this iterator after incrementing
         * @throw std::out_of_range if attempting to increment end iterator
         */
        iterator& operator++() {
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot increment end iterator");
            }
            current_ = current_->next;
            return *this;
        }

        /**
         * @brief Post-increment operator
         * @return Copy of iterator before incrementing
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Pre-decrement operator
         * @return Reference to this iterator after decrementing
         */
        iterator& operator--() {
            current_ = current_->prev;
            if (current_ == temp_node_) {
                current_ = current_->prev;
            }
            return *this;
        }

        /**
         * @brief Post-decrement operator
         * @return Copy of iterator before decrementing
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator
         * @param other Iterator to compare with
         * @return True if iterators point to the same node, false otherwise
         */
        bool operator==(const iterator& other) const { 
            return current_ == other.current_; 
        }
        
        /**
         * @brief Inequality comparison operator
         * @param other Iterator to compare with
         * @return True if iterators point to different nodes, false otherwise
         */
        bool operator!=(const iterator& other) const { 
            return !(*this == other); 
        }

    private:
        Node* current_;     ///< Pointer to current node
        Node* temp_node_;   ///< Pointer to sentinel node (end marker)
        
        // Grant access to const_iterator and CircularList for conversion
        friend class const_iterator;
        friend class CircularList<T, Allocator>;
    };

    /**
     * @class const_iterator
     * @brief Constant bidirectional iterator for CircularList
     */
    class const_iterator {
    public:
        // Iterator traits for STL compatibility
        using iterator_category = std::bidirectional_iterator_tag; ///< Iterator category
        using value_type = const T;                                ///< Value type
        using difference_type = std::ptrdiff_t;                    ///< Difference type
        using pointer = const T*;                                  ///< Pointer type
        using reference = const T&;                                ///< Reference type

        /**
         * @brief Constructor
         * @param node Pointer to current node
         * @param temp_node Pointer to sentinel node (end marker)
         */
        const_iterator(Node* node = nullptr, Node* temp_node = nullptr) 
            : current_(node), temp_node_(temp_node) {}
            
        /**
         * @brief Conversion constructor from iterator
         * @param it Iterator to convert to const_iterator
         */
        const_iterator(iterator it) : current_(it.current_), temp_node_(it.temp_node_) {}

        /**
         * @brief Dereference operator
         * @return Const reference to the current element
         * @throw std::out_of_range if attempting to dereference end iterator
         */
        reference operator*() const { 
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot dereference end iterator");
            }
            return current_->value; 
        }
        
        /**
         * @brief Member access operator
         * @return Const pointer to the current element
         * @throw std::out_of_range if attempting to access end iterator
         */
        pointer operator->() const { 
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot access end iterator");
            }
            return &current_->value; 
        }

        /**
         * @brief Pre-increment operator
         * @return Reference to this iterator after incrementing
         * @throw std::out_of_range if attempting to increment end iterator
         */
        const_iterator& operator++() {
            if (current_ == temp_node_) {
                throw std::out_of_range("Cannot increment end iterator");
            }
            current_ = current_->next;
            return *this;
        }

        /**
         * @brief Post-increment operator
         * @return Copy of iterator before incrementing
         */
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Pre-decrement operator
         * @return Reference to this iterator after decrementing
         */
        const_iterator& operator--() {
            current_ = current_->prev;
            if (current_ == temp_node_) {
                current_ = current_->prev;
            }
            return *this;
        }

        /**
         * @brief Post-decrement operator
         * @return Copy of iterator before decrementing
         */
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator
         * @param other Iterator to compare with
         * @return True if iterators point to the same node, false otherwise
         */
        bool operator==(const const_iterator& other) const { 
            return current_ == other.current_; 
        }
        
        /**
         * @brief Inequality comparison operator
         * @param other Iterator to compare with
         * @return True if iterators point to different nodes, false otherwise
         */
        bool operator!=(const const_iterator& other) const { 
            return !(*this == other); 
        }

    private:
        Node* current_;     ///< Pointer to current node
        Node* temp_node_;   ///< Pointer to sentinel node (end marker)
        
        // Grant access to CircularList
        friend class CircularList<T, Allocator>;
    };

    // Reverse iterator types using std::reverse_iterator
    using reverse_iterator = std::reverse_iterator<iterator>;                ///< Reverse iterator type
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;    ///< Const reverse iterator type

    /**
     * @brief Default constructor
     * @post Creates an empty list with a sentinel node
     */
    CircularList() : temp_node_(new Node(T())), size_(0) {
        temp_node_->next = temp_node_;
        temp_node_->prev = temp_node_;
    }

    /**
     * @brief Initializer list constructor
     * @param init Initializer list with elements to add to the list
     */
    CircularList(std::initializer_list<T> init) : CircularList() {
        for (const auto& item : init) {
            push_back(item);
        }
    }

    /**
     * @brief Copy constructor
     * @param other Another CircularList to copy elements from
     */
    CircularList(const CircularList& other) : CircularList() {
        for (const auto& item : other) {
            push_back(item);
        }
    }

    /**
     * @brief Move constructor
     * @param other Another CircularList to move elements from
     * @post other is left in a valid but unspecified state
     */
    CircularList(CircularList&& other) noexcept 
        : temp_node_(other.temp_node_), size_(other.size_) {
        other.temp_node_ = nullptr;
        other.size_ = 0;
    }

    /**
     * @brief Destructor
     * @post All elements are destroyed and memory is deallocated
     */
    ~CircularList() {
        clear();
        delete temp_node_;
    }

    /**
     * @brief Copy assignment operator
     * @param other Another CircularList to copy elements from
     * @return Reference to this list
     */
    CircularList& operator=(const CircularList& other) {
        if (this != &other) {
            clear();
            for (const auto& item : other) {
                push_back(item);
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator
     * @param other Another CircularList to move elements from
     * @return Reference to this list
     * @post other is left in a valid but unspecified state
     */
    CircularList& operator=(CircularList&& other) noexcept {
        if (this != &other) {
            clear();
            delete temp_node_;
            
            temp_node_ = other.temp_node_;
            size_ = other.size_;
            
            other.temp_node_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief Add element to the end of the list
     * @param value Value to add to the end of the list
     */
    void push_back(const T& value) {
        insert(end(), value);
    }

    /**
     * @brief Add element to the beginning of the list
     * @param value Value to add to the beginning of the list
     */
    void push_front(const T& value) {
        insert(begin(), value);
    }

    /**
     * @brief Construct element in-place at specified position
     * @tparam Args Types of arguments to forward to element constructor
     * @param pos Iterator before which the new element will be inserted
     * @param args Arguments to forward to element constructor
     * @return Iterator pointing to the inserted element
     */
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        Node* newNode = new Node(std::forward<Args>(args)...);
        Node* target = pos.current_;
        
        // Update links to insert new node
        newNode->next = target;
        newNode->prev = target->prev;
        target->prev->next = newNode;
        target->prev = newNode;
        
        ++size_;
        return iterator(newNode, temp_node_);
    }

    /**
     * @brief Insert element by copy at specified position
     * @param pos Iterator before which the new element will be inserted
     * @param value Value to insert
     * @return Iterator pointing to the inserted element
     */
    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    /**
     * @brief Insert element by move at specified position
     * @param pos Iterator before which the new element will be inserted
     * @param value Value to insert
     * @return Iterator pointing to the inserted element
     */
    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    /**
     * @brief Remove element at specified position
     * @param pos Iterator to the element to remove
     * @return Iterator following the last removed element
     * @throw std::out_of_range if list is empty or pos is end iterator
     */
    iterator erase(const_iterator pos) {
        if (empty()) {
            throw std::out_of_range("Cannot erase from empty list");
        }
        
        Node* target = pos.current_;
        if (target == temp_node_) {
            throw std::out_of_range("Cannot erase end iterator");
        }
        
        // Update links to remove node
        target->prev->next = target->next;
        target->next->prev = target->prev;
        
        iterator next(target->next, temp_node_);
        delete target;
        --size_;
        
        return next;
    }

    /**
     * @brief Remove first element
     * @throw std::out_of_range if list is empty
     */
    void pop_front() {
        erase(begin());
    }

    /**
     * @brief Remove last element
     * @throw std::out_of_range if list is empty
     */
    void pop_back() {
        erase(--end());
    }

    /**
     * @brief Remove all elements from the list
     * @post List becomes empty
     */
    void clear() noexcept {
        while (!empty()) {
            pop_front();
        }
    }

    /**
     * @brief Access first element
     * @return Reference to the first element
     * @throw std::out_of_range if list is empty
     */
    reference front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return temp_node_->next->value;
    }

    /**
     * @brief Access first element (const version)
     * @return Const reference to the first element
     * @throw std::out_of_range if list is empty
     */
    const_reference front() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return temp_node_->next->value;
    }

    /**
     * @brief Access last element
     * @return Reference to the last element
     * @throw std::out_of_range if list is empty
     */
    reference back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return temp_node_->prev->value;
    }

    /**
     * @brief Access last element (const version)
     * @return Const reference to the last element
     * @throw std::out_of_range if list is empty
     */
    const_reference back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return temp_node_->prev->value;
    }

    /**
     * @brief Get number of elements in the list
     * @return Number of elements in the list
     */
    size_type size() const noexcept { return size_; }
    
    /**
     * @brief Check if list is empty
     * @return True if list is empty, false otherwise
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Get iterator to the beginning of the list
     * @return Iterator to the first element
     */
    iterator begin() noexcept { return iterator(temp_node_->next, temp_node_); }
    
    /**
     * @brief Get const iterator to the beginning of the list
     * @return Const iterator to the first element
     */
    const_iterator begin() const noexcept { return const_iterator(temp_node_->next, temp_node_); }
    
    /**
     * @brief Get const iterator to the beginning of the list
     * @return Const iterator to the first element
     */
    const_iterator cbegin() const noexcept { return const_iterator(temp_node_->next, temp_node_); }
    
    /**
     * @brief Get iterator to the end of the list (sentinel node)
     * @return Iterator to the element following the last element
     */
    iterator end() noexcept { return iterator(temp_node_, temp_node_); }
    
    /**
     * @brief Get const iterator to the end of the list (sentinel node)
     * @return Const iterator to the element following the last element
     */
    const_iterator end() const noexcept { return const_iterator(temp_node_, temp_node_); }
    
    /**
     * @brief Get const iterator to the end of the list (sentinel node)
     * @return Const iterator to the element following the last element
     */
    const_iterator cend() const noexcept { return const_iterator(temp_node_, temp_node_); }

    /**
     * @brief Get reverse iterator to the beginning of reversed list
     * @return Reverse iterator to the last element
     */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    
    /**
     * @brief Get const reverse iterator to the beginning of reversed list
     * @return Const reverse iterator to the last element
     */
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    
    /**
     * @brief Get const reverse iterator to the beginning of reversed list
     * @return Const reverse iterator to the last element
     */
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    
    /**
     * @brief Get reverse iterator to the end of reversed list
     * @return Reverse iterator to the element preceding the first element
     */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    
    /**
     * @brief Get const reverse iterator to the end of reversed list
     * @return Const reverse iterator to the element preceding the first element
     */
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    
    /**
     * @brief Get const reverse iterator to the end of reversed list
     * @return Const reverse iterator to the element preceding the first element
     */
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

private:
    Node* temp_node_;   ///< Sentinel node (dummy node marking list boundaries)
    size_type size_;    ///< Number of elements in the list
};

}
#endif
