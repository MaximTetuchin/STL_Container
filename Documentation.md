# STL-Compatible CircularList Container

## Table of Contents
1. [Overview](#overview)
2. [Type Aliases](#type-aliases)
3. [Iterators](#iterators)
4. [Constructors and Destructors](#constructors-and-destructors)
5. [Element Access](#element-access)
6. [Container Modification](#container-modification)
7. [Capacity Information](#capacity-information)
8. [Iterator Access](#iterator-access)
9. [Internal Structure](#internal-structure)

## Overview

`CircularList` is an STL-style implementation of a circular doubly-linked list container with full compatibility with the C++ Standard Library. The container supports bidirectional iterators, exception safety, and follows RAII principles.

**Key Features:**
- Circular data structure
- STL iterator support
- Exception safety
- Move semantics support
- Range-based for loop compatibility

## Type Aliases

```cpp
using value_type = T;
using allocator_type = Allocator;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;
using reference = value_type&;
using const_reference = const value_type&;
using pointer = typename std::allocator_traits<Allocator>::pointer;
using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
```

## Iterators

The container provides a complete set of iterators in accordance with the STL standard:

### iterator
- Category: `std::bidirectional_iterator_tag`
- Supports increment/decrement operations
- Performs bounds checking when dereferencing

### const_iterator
- Constant version of iterator
- Can be constructed from iterator

### reverse_iterator and const_reverse_iterator
- Implemented via `std::reverse_iterator`
- Provide reverse traversal of the container

## Constructors and Destructors

### Main Constructors:
```cpp
CircularList();                          // Default
CircularList(std::initializer_list<T>);  // Initializer list
CircularList(const CircularList&);       // Copy
CircularList(CircularList&&) noexcept;   // Move
```

### Destructor:
- Automatically deallocates all memory
- Follows RAII principles

### Assignment Operators:
```cpp
CircularList& operator=(const CircularList&);  // Copy assignment
CircularList& operator=(CircularList&&) noexcept; // Move assignment
```

## Element Access

### front() / back()
Return references to the first/last element. Throw `std::out_of_range` when the container is empty.

## Container Modification

### Adding Elements:
```cpp
void push_back(const T& value);
void push_front(const T& value);
iterator insert(const_iterator pos, const T& value);
iterator insert(const_iterator pos, T&& value);
template<typename... Args>
iterator emplace(const_iterator pos, Args&&... args);
```

### Removing Elements:
```cpp
iterator erase(const_iterator pos);  // Throws exception if list is empty
void pop_front();                    // Throws exception if list is empty
void pop_back();                     // Throws exception if list is empty
void clear() noexcept;               // Safe clear operation
```

## Capacity Information

```cpp
size_type size() const noexcept;  // Current size
bool empty() const noexcept;      // Empty check
```

## Iterator Access

Complete set of methods for container traversal:
```cpp
iterator begin() noexcept;
const_iterator begin() const noexcept;
const_iterator cbegin() const noexcept;

iterator end() noexcept;
const_iterator end() const noexcept;
const_iterator cend() const noexcept;

reverse_iterator rbegin() noexcept;
const_reverse_iterator rbegin() const noexcept;
const_reverse_iterator crbegin() const noexcept;

reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
const_reverse_iterator crend() const noexcept;
```

## Internal Structure

The container uses:
- Special sentinel node to mark boundaries
- Doubly-linked circular node structure
- Template Node class with perfect forwarding support

**Implementation Details:**
- Exception throwing on invalid element access
- Exception safety guarantees
- Optimized memory management