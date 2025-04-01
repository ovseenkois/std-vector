#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR_MEMORY_IMPLEMENTED
#include <cstddef>
#include <iterator>
#include <memory>
#include <algorithm>

template <class T>
class Vector;

template <class T>
bool operator!=(const Vector<T>&, const Vector<T>&);
template <class T>
bool operator<(const Vector<T>&, const Vector<T>&);
template <class T>
bool operator>(const Vector<T>&, const Vector<T>&);
template <class T>
bool operator<=(const Vector<T>&, const Vector<T>&);
template <class T>
bool operator>=(const Vector<T>&, const Vector<T>&);
template <class T>
bool operator==(const Vector<T>&, const Vector<T>&);

template <class T>
class Vector {
 private:
  T* buffer_;
  size_t size_;
  size_t capacity_;

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = Pointer;
  using ConstIterator = ConstPointer;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() noexcept;
  explicit Vector(size_t size);
  Vector(size_t size, const T& value);

  template <class FIterator,
            class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag,
                                                       typename std::iterator_traits<FIterator>::iterator_category>>>
  Vector(FIterator first, FIterator second) : size_(std::distance(first, second)), capacity_(size_) {
    if (size_ == 0) {
      buffer_ = nullptr;
    } else {
      try {
        buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
        std::uninitialized_copy(first, second, begin());
      } catch (...) {
        operator delete(buffer_);
        throw;
      }
    }
  }

  Vector(std::initializer_list<T> list);

  // правило пяти
  Vector(const Vector<T>& other);
  Vector(Vector<T>&& other) noexcept;
  Vector<T>& operator=(const Vector<T>& other);
  Vector<T>& operator=(Vector<T>&& other) noexcept;
  ~Vector() noexcept;

  size_t Size() const noexcept;
  size_t Capacity() const noexcept;
  bool Empty() const noexcept;
  T& operator[](size_t index) noexcept;
  const T& operator[](size_t index) const noexcept;
  T& At(size_t index);
  const T& At(size_t index) const;
  T& Front() noexcept;
  const T& Front() const noexcept;
  T& Back() noexcept;
  const T& Back() const noexcept;
  T* Data() noexcept;
  const T* Data() const noexcept;
  void Swap(Vector<T>& other) noexcept;
  void Resize(size_t new_size);
  void Resize(size_t new_size, const T& value);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Clear() noexcept;
  void PushBack(const T& value);
  void PushBack(T&& value);
  void PopBack() noexcept;
  template <class... Args>
  void EmplaceBack(Args&&... args);

  Iterator begin() noexcept;                      
  ConstIterator begin() const noexcept;           
  ConstIterator cbegin() const noexcept;          
  Iterator end() noexcept;                        
  ConstIterator end() const noexcept;             
  ConstIterator cend() const noexcept;            
  ReverseIterator rbegin() noexcept;              
  ConstReverseIterator rbegin() const noexcept;   
  ConstReverseIterator crbegin() const noexcept;  
  ReverseIterator rend() noexcept;                
  ConstReverseIterator rend() const noexcept;     
  ConstReverseIterator crend() const noexcept;    
};

template <class T>
void Vector<T>::Clear() noexcept {
  if (buffer_) {
    for (size_t i = 0; i < size_; ++i) {
      (buffer_ + i)->~T();
    }
    size_ = 0;
  }
}

// итераторы (начало)

template <class T>
typename Vector<T>::Iterator Vector<T>::begin() noexcept {
  return buffer_;
}

template <class T>
typename Vector<T>::ConstIterator Vector<T>::begin() const noexcept {
  return const_cast<const T*>(buffer_);
}

template <class T>
typename Vector<T>::ConstIterator Vector<T>::cbegin() const noexcept {
  return const_cast<const T*>(buffer_);
}

template <class T>
typename Vector<T>::Iterator Vector<T>::end() noexcept {
  return buffer_ + size_;
}

template <class T>
typename Vector<T>::ConstIterator Vector<T>::end() const noexcept {
  return const_cast<const T*>(buffer_ + size_);
}

template <class T>
typename Vector<T>::ConstIterator Vector<T>::cend() const noexcept {
  return const_cast<const T*>(buffer_ + size_);
}

template <class T>
typename Vector<T>::ReverseIterator Vector<T>::rbegin() noexcept {
  return ReverseIterator(buffer_ + size_);
}

template <class T>
typename Vector<T>::ConstReverseIterator Vector<T>::rbegin() const noexcept {
  return ConstReverseIterator(const_cast<const T*>(buffer_ + size_));
}

template <class T>
typename Vector<T>::ConstReverseIterator Vector<T>::crbegin() const noexcept {
  return ConstReverseIterator(const_cast<const T*>(buffer_ + size_));
}

template <class T>
typename Vector<T>::ReverseIterator Vector<T>::rend() noexcept {
  return ReverseIterator(buffer_);
}

template <class T>
typename Vector<T>::ConstReverseIterator Vector<T>::rend() const noexcept {
  return ConstReverseIterator(const_cast<const T*>(buffer_));
}

template <class T>
typename Vector<T>::ConstReverseIterator Vector<T>::crend() const noexcept {
  return ConstReverseIterator(const_cast<const T*>(buffer_));
}

// итераторы (конец)

template <class T>
Vector<T>::Vector() noexcept : buffer_(nullptr), size_(0), capacity_(0) {
}

template <class T>
Vector<T>::Vector(size_t size) : size_(size), capacity_(size) {
  if (size == 0) {
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
      std::uninitialized_default_construct(begin(), end());
    } catch (...) {
      size_ = 0;
      capacity_ = 0;
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
  }
}

template <class T>
Vector<T>::Vector(size_t size, const T& value) : size_(size), capacity_(size) {
  if (size == 0) {
    buffer_ = nullptr;
    size_ = 0;
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
      std::uninitialized_fill(begin(), end(), value);
    } catch (...) {
      size_ = 0;
      capacity_ = 0;
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
  }
}

template <class T>
Vector<T>::Vector(std::initializer_list<T> list) : size_(list.size()), capacity_(size_) {
  if (size_ == 0) {
    buffer_ = nullptr;
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
      std::uninitialized_copy(list.begin(), list.end(), begin());
    } catch (...) {
      size_ = 0;
      capacity_ = 0;
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
  }
}

template <class T>
Vector<T>::Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
  if (other.buffer_) {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
      std::uninitialized_copy(other.begin(), other.end(), begin());
    } catch (...) {
      size_ = 0;
      capacity_ = 0;
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
  } else {
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
}

template <class T>
Vector<T>::Vector(Vector<T>&& other) noexcept : size_(other.size_), capacity_(other.capacity_) {
  buffer_ = other.buffer_;
  size_ = other.size_;
  capacity_ = other.capacity_;
  other.buffer_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  if (this != &other) {
    if (buffer_) {
      if (other.buffer_) {
        if (capacity_ >= other.size_) {
          Clear();
          size_ = other.size_;
          try {
            std::uninitialized_copy(other.begin(), other.end(), begin());
          } catch (...) {
            size_ = 0;
            capacity_ = 0;
            operator delete(buffer_);
            buffer_ = nullptr;
            throw;
          }
        } else {
          Clear();
          try {
            operator delete(buffer_);
            size_ = other.size_;
            capacity_ = other.capacity_;
            buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
            std::uninitialized_copy(other.begin(), other.end(), begin());
          } catch (...) {
            size_ = 0;
            capacity_ = 0;
            operator delete(buffer_);
            buffer_ = nullptr;
            throw;
          }
        }

      } else {
        Clear();
        operator delete(buffer_);
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }

    } else {
      if (other.buffer_) {
        try {
          size_ = other.size_;
          capacity_ = other.capacity_;
          buffer_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
          std::uninitialized_copy(other.begin(), other.end(), begin());
        } catch (...) {
          size_ = 0;
          capacity_ = 0;
          operator delete(buffer_);
          buffer_ = nullptr;
          throw;
        }
      } else {
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }
    }
  }
  return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
  if (this != &other) {
    Clear();
    operator delete(buffer_);
    buffer_ = other.buffer_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template <class T>
Vector<T>::~Vector() noexcept {
  Clear();
  operator delete(buffer_);
}

template <class T>
size_t Vector<T>::Size() const noexcept {
  return size_;
}

template <class T>
size_t Vector<T>::Capacity() const noexcept {
  return capacity_;
}

template <class T>
bool Vector<T>::Empty() const noexcept {
  return (size_ == 0);
}

template <class T>
T& Vector<T>::operator[](size_t index) noexcept {
  return *(buffer_ + index);
}

template <class T>
const T& Vector<T>::operator[](size_t index) const noexcept {
  return *(buffer_ + index);
}

template <class T>
T& Vector<T>::At(size_t index) {
  if (index < size_) {
    return *(buffer_ + index);
  }
  throw std::out_of_range{"123"};
}

template <class T>
const T& Vector<T>::At(size_t index) const {
  if (index < size_) {
    return *(buffer_ + index);
  }
  throw std::out_of_range{"123"};
}

template <class T>
T& Vector<T>::Front() noexcept {
  return *(buffer_);
}

template <class T>
const T& Vector<T>::Front() const noexcept {
  return *(buffer_);
}

template <class T>
T& Vector<T>::Back() noexcept {
  return *(buffer_ + size_ - 1);
}

template <class T>
const T& Vector<T>::Back() const noexcept {
  return *(buffer_ + size_ - 1);
}

template <class T>
T* Vector<T>::Data() noexcept {
  return buffer_;
}

template <class T>
const T* Vector<T>::Data() const noexcept {
  return buffer_;
}

template <class T>
void Vector<T>::Swap(Vector<T>& other) noexcept {
  std::swap(buffer_, other.buffer_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <class T>
void Vector<T>::Resize(size_t new_size) {
  if (new_size == 0) {
    Clear();
    return;
  }
  if (new_size <= size_) {
    for (size_t i = new_size; i < size_; ++i) {
      (buffer_ + i)->~T();
    }
    size_ = new_size;
    return;
  }
  if (new_size > size_) {
    if (new_size > capacity_) {
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * new_size));
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      try {
        std::uninitialized_default_construct(new_buffer + size_, new_buffer + new_size);
      } catch (...) {
        std::uninitialized_move(new_buffer, new_buffer + size_, buffer_);
        std::destroy(new_buffer, new_buffer + size_);
        operator delete(new_buffer);
        throw;
      }
      capacity_ = new_size;
      size_ = new_size;
      operator delete(buffer_);
      buffer_ = new_buffer;
    } else {
      int old_size = size_;
      try {
        std::uninitialized_default_construct(buffer_ + size_, buffer_ + new_size);
        size_ = new_size;
      } catch (...) {
        size_ = old_size;
        throw;
      }
    }
  }
}

template <class T>
void Vector<T>::Resize(size_t new_size, const T& value) {
  if (new_size == 0) {
    Clear();
    return;
  }
  if (new_size <= size_) {
    for (size_t i = new_size; i < size_; ++i) {
      (buffer_ + i)->~T();
    }
    size_ = new_size;
    return;
  }
  if (new_size > size_) {
    if (new_size > capacity_) {
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * new_size));
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      try {
        std::uninitialized_fill(new_buffer + size_, new_buffer + new_size, value);
      } catch (...) {
        std::uninitialized_move(new_buffer, new_buffer + size_, buffer_);
        std::destroy(new_buffer, new_buffer + size_);
        operator delete(new_buffer);
        throw;
      }
      capacity_ = new_size;
      size_ = new_size;
      operator delete(buffer_);
      buffer_ = new_buffer;
    } else {
      int old_size = size_;
      try {
        std::uninitialized_fill(buffer_ + size_, buffer_ + new_size, value);
        size_ = new_size;
      } catch (...) {
        size_ = old_size;
        throw;
      }
    }
  }
}

template <class T>
void Vector<T>::Reserve(size_t new_cap) {
  if (new_cap <= capacity_) {
    return;
  }
  if (buffer_) {
    T* new_buffer = nullptr;
    try {
      new_buffer = static_cast<T*>(operator new(sizeof(T) * new_cap));
    } catch (...) {
      operator delete(new_buffer);
      throw;
    }
    std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
    std::destroy(buffer_, buffer_ + size_);
    operator delete(buffer_);
    buffer_ = new_buffer;
    capacity_ = new_cap;
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * new_cap));
    } catch (...) {
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
    capacity_ = new_cap;
  }
}

template <class T>
void Vector<T>::ShrinkToFit() {
  if (size_ < capacity_) {
    if (size_ == 0) {
      capacity_ = 0;
      operator delete(buffer_);
      buffer_ = nullptr;
    } else {
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * size_));
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      capacity_ = size_;
    }
  }
}

template <class T>
void Vector<T>::PushBack(const T& value) {
  if (buffer_) {
    if (size_ >= capacity_) {
      auto new_capacity = capacity_ * 2;
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * new_capacity));
        // new_buffer[size_] = value;
        new (new_buffer + size_) T(value);
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      ++size_;
      capacity_ = new_capacity;
    } else {
      // buffer_[size_] = value;
      new (buffer_ + size_) T(value);
      ++size_;
    }
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * 2));
    } catch (...) {
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
    // buffer_[0] = value;
    new (buffer_) T(value);
    capacity_ = 2;
    size_ = 1;
  }
}

template <class T>
void Vector<T>::PushBack(T&& value) {
  if (buffer_) {
    if (size_ >= capacity_) {
      auto new_capacity = capacity_ * 2;
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * new_capacity));
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      new (new_buffer + size_) T(std::move(value));
      operator delete(buffer_);
      buffer_ = new_buffer;
      ++size_;
      capacity_ = new_capacity;
    } else {
      new (buffer_ + size_) T(std::move(value));
      ++size_;
    }
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * 2));
    } catch (...) {
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
    new (buffer_ + 0) T(std::move(value));
    capacity_ = 2;
    size_ = 1;
  }
}

template <class T>
void Vector<T>::PopBack() noexcept {
  (buffer_ + size_ - 1)->~T();
  --size_;
}

template <class T>
bool operator<(const Vector<T>& a, const Vector<T>& b) {
  if (a.Data() == nullptr && b.Data() != nullptr) {
    return true;
  }
  if (a.Data() != nullptr && b.Data() == nullptr) {
    return false;
  }
  if (a.Data() == nullptr && b.Data() == nullptr) {
    return false;
  }
  size_t i = 0;
  while (i < a.Size() && i < b.Size()) {
    if (a[i] < b[i]) {
      return true;
    }
    if (a[i] > b[i]) {
      return false;
    }
    ++i;
  }
  return (a.Size() < b.Size());
}

template <class T>
bool operator==(const Vector<T>& a, const Vector<T>& b) {
  return (!(a < b) && !(b < a));
}

template <class T>
bool operator<=(const Vector<T>& a, const Vector<T>& b) {
  return ((a < b) || (a == b));
}

template <class T>
bool operator>(const Vector<T>& a, const Vector<T>& b) {
  return !(a <= b);
}

template <class T>
bool operator>=(const Vector<T>& a, const Vector<T>& b) {
  return (a > b || a == b);
}

template <class T>
bool operator!=(const Vector<T>& a, const Vector<T>& b) {
  return !(a == b);
}

template <class T>
template <class... Args>
void Vector<T>::EmplaceBack(Args&&... args) {
  if (buffer_) {
    if (size_ >= capacity_) {
      auto new_capacity = capacity_ * 2;
      T* new_buffer = nullptr;
      try {
        new_buffer = static_cast<T*>(operator new(sizeof(T) * new_capacity));
        // new_buffer[size_] = value;  /// !!!
        new (new_buffer + size_) T(std::forward<Args>(args)...);
      } catch (...) {
        operator delete(new_buffer);
        throw;
      }
      std::uninitialized_move(buffer_, buffer_ + size_, new_buffer);
      std::destroy(buffer_, buffer_ + size_);
      operator delete(buffer_);
      buffer_ = new_buffer;
      ++size_;
      capacity_ = new_capacity;
    } else {
      // buffer_[size_] = value;  // !!!
      new (buffer_ + size_) T(std::forward<Args>(args)...);
      ++size_;
    }
  } else {
    try {
      buffer_ = static_cast<T*>(operator new(sizeof(T) * 2));
    } catch (...) {
      operator delete(buffer_);
      buffer_ = nullptr;
      throw;
    }
    // buffer_[0] = value; //!!!
    new (buffer_) T(std::forward<Args>(args)...);
    capacity_ = 2;
    size_ = 1;
  }
}

#endif
