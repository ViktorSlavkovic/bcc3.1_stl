// File: stack.h
// Author: Viktor Slavkovic
// Date: May 2016

#ifndef _STL_STACK_H_
#define _STL_STACK_H_

#include "list.h"

// It seems that BCC doesn't support this:
// template<class T, class Container = list<T> >
template <class T>
class stack {
 public:
  typedef list<T> Container;
  typedef T value_type;
  typedef Container::size_type size_type;
  typedef Container::iterator iterator;

  stack() : container_() {}

  stack(const stack& cp) : container_(cp.container_) {}

  int empty() const { return container_.empty(); }

  size_type size() const { return container_.size(); }

  void clear() { container_.clear(); }

  value_type& top() { return container_.front(); };

  const value_type& top() const { return container_.front(); }

  void push(const value_type& val) { container_.push_front(val); }

  void pop() { container_.pop_front(); }

  int operator==(const stack& rhs) const {
    return container_ == rhs.container_;
  }

  int operator!=(const stack& rhs) const {
    return container_ != rhs.container_;
  }

  iterator begin() { return container_.begin(); }

  iterator end() { return container_.end(); }

 private:
  Container container_;
};

#endif  // _STL_STACK_H_
