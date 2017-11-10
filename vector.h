#ifndef _STL_VECTOR_H_
#define _STL_VECTOR_H_

#include "algo.h"

#include <assert.h>

template <class T>
class vector {
 public:
  typedef T value_type;
  typedef unsigned size_type;

  vector(size_type size = 0)
      : size_(size), cap_(max(32u, size_)), v_(new value_type[cap_]) {}

  vector(size_type size, const value_type& val)
      : size_(size), cap_(max(32u, size_)), v_(new value_type[cap_]) {
    for (size_type i = 0; i < size_; i++) v_[i] = val;
  }

  vector(const vector& cp)
      : size_(cp.size_), cap_(cp.cap_), v_(new value_type[cap_]) {
    for (size_type i = 0; i < size_; i++) v_[i] = cp.v_[i];
  }

  ~vector() { delete[] v_; }

  vector& operator=(const vector& cp) {
    if (this != &cp) {
      delete[] v_;
      cap_ = cp.cap_;
      size_ = cp.size_;
      v_ = new value_type[cap_];
      for (size_type i = 0; i < size_; i++) v_[i] = cp.v_[i];
    }
    return *this;
  }

  value_type& operator[](size_type idx) {
    assert(idx < size_);
    return v_[idx];
  }

  const value_type& operator[](size_type idx) const {
    assert(idx < size_);
    return v_[idx];
  }

  size_type size() const { return size_; }
  size_type capacity() const { return cap_; }
  int empty() const { return !size_; }

  void clear() { resize(0); }

  void resize(size_type size) {
    if (size_ == size) return;
    if (size < size_)
      size_ = size;
    else {
      if (size > cap_) reserve(new_capacity(size));
      size_ = size;
    }
  }

  void resize(size_type size, const value_type& val) {
    size_type old_size = size_;
    resize(size);
    for (int i = old_size; i < size_; i++) v_[i] = val;
  }

  void reserve(size_type cap) {
    if (cap <= cap_) return;
    value_type* temp = new value_type[cap];
    for (size_type i = 0; i < size_; i++) temp[i] = v_[i];
    delete[] v_;
    v_ = temp;
  }

  void push_back(const value_type& val) {
    resize(size_ + 1);
    v_[size_ - 1] = val;
  }

  void pop_back() {
    assert(size_);
    size_--;
  }

  class iterator {
   public:
    value_type& operator*() { return *ptr_; }
    value_type& operator[](int n) { return *(ptr_ + n); }

    int operator==(const iterator& rhs) {
      if (!owner_) return 0;
      if (owner_ != rhs.owner_) return 0;
      return ptr_ == rhs.ptr_;
    }

    int operator!=(const iterator& rhs) { return !(*this == rhs); }

    iterator& operator++(int k) {
      ptr_++;
      return *this;
    }
    iterator& operator++() {
      ptr_++;
      return *this;
    }
    iterator& operator--(int k) {
      ptr_--;
      return *this;
    }
    iterator& operator--() {
      ptr_--;
      return *this;
    }

    iterator& operator+=(int rhs) {
      ptr_ += rhs;
      return *this;
    }
    iterator& operator-=(int rhs) {
      ptr_ -= rhs;
      return *this;
    }

    friend int operator-(const iterator& lhs, const iterator& rhs) {
      return lhs.ptr_ - rhs.ptr_;
    }

    friend iterator operator+(const iterator& lhs, int rhs) {
      return iterator(lhs.owner_, lhs.ptr_ + rhs);
    }

    friend iterator operator+(int lhs, const iterator& rhs) {
      return iterator(rhs.owner_, rhs.ptr_ + lhs);
    }

    friend iterator operator-(const iterator& lhs, int rhs) {
      return iterator(lhs.owner_, lhs.ptr_ - rhs);
    }

   private:
    value_type* ptr_;
    vector<value_type>* owner_;
    iterator(vector<value_type>* owner, value_type* ptr)
        : owner_(owner), ptr_(ptr) {}

    friend class vector<value_type>;
  };

  iterator begin() { return iterator(this, v_); }
  iterator end() { return iterator(this, v_ + size_); }

 private:
  size_type size_, cap_;
  value_type* v_;

  size_type new_capacity(size_type new_size) {
    return (new_size / 32u + 1) * 32u;
    //    size_type delta = new_size - cap_ - (cap_ - cap_ / 32u);
    //    return delta / 32u + ((delta % 32u) ? 0 : 1);
  }
};

#endif  // _STL_VECTOR_H_
