// File: list.h
// Author: Viktor Slavkovic
// Date: May 2016

#ifndef _STL_LIST_H_
#define _STL_LIST_H_

#include <assert.h>

template <class T>
class list {
 public:
  typedef T value_type;
  typedef unsigned size_type;

  list() : head_(0), tail_(0), size_(0u) {}
  ~list() { clear(); }

  value_type& front() {
    assert(size_);
    return head_->val;
  }

  const value_type& front() const {
    assert(size_);
    return head_->val;
  }

  value_type& back() {
    assert(size_);
    return tail_->val;
  }

  const value_type& back() const {
    assert(size_);
    return tail_->val;
  }

  size_type size() const { return size_; }

  int empty() const { return !size_; }

  void clear() {
    while (head_) {
      pnode temp = head_;
      head_ = head_->next;
      delete temp;
    }
    tail_ = 0;
    size_ = 0;
  }

  void push_back(const value_type& val) {
    pnode temp = new node(val);
    if (!size_++)
      head_ = tail_ = temp;
    else {
      tail_->next = temp;
      tail_ = temp;
    }
  }

  void push_front(const value_type& val) {
    head_ = new node(val, head_);
    if (!size_++) tail_ = head_;
  }

  // O(size)
  void pop_back() {
    if (!size_) return;
    pnode* p = &head_;
    while ((*p)->next) p = &((*p)->next);
    delete (*p);
    *p = 0;
    if (!--size_) head_ = tail_ = 0;
  }

  void pop_front() {
    if (!size_) return;
    pnode temp = head_;
    head_ = head_->next;
    delete temp;
    if (!--size_) head_ = tail_ = 0;
  }

  int operator==(const list& rhs) const {
    if (size_ != rhs.size_) return 0;
    for (pnode p1 = head_, p2 = rhs.head_; p1; p1 = p1->next, p2 = p2->next)
      if (p1->val != p2->val) return 0;
    return 1;
  }

  int operator!=(const list& rhs) const { return !(*this == rhs); }

 private:
  struct node {
    node(const value_type& val, node* next = 0) : val(val), next(next) {}
    value_type val;
    node* next;
  };
  typedef node* pnode;

  pnode head_, tail_;
  unsigned size_;

 public:
  class iterator {
   public:
    iterator() : owner(0), p(0) {}

    void operator++() {
      if (p) p = p->next;
    }

    void operator++(int k) {
      if (p) p = p->next;
    }

    value_type& operator*() {
      assert(p);
      return p->val;
    }

    int operator==(const iterator& rhs) const {
      if (!owner) return 0;
      if (owner != rhs.owner) return 0;
      return (p == rhs.p);
    }

    int operator!=(const iterator& rhs) const { return !(*this == rhs); }

   private:
    typedef list<value_type>::node* pnode;
    pnode p;

    list<value_type>* owner;
    iterator(list<value_type>* owner, const pnode& p) : owner(owner), p(p) {}

    friend class list<value_type>;
  };

  iterator begin() { return iterator(this, head_); }

  iterator end() { return iterator(this, (pnode)0); }
};

#endif  // _STL_LIST_H_
