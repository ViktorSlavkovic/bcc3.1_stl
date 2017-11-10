// File: map.h
// Author: Viktor Slavkovic
// Date: May 2016
//
// Description: Left-Leaning Red-Black tree implementation of 2-3 tree
//              Info: http://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf

#ifndef _STL_MAP_H_
#define _STL_MAP_H_

#include "stack.h"
#include "utility.h"

template <class Key, class Value>
class map {
 public:
  typedef Key key_type;
  typedef Value mapped_type;
  typedef unsigned size_type;

  map() : root_(0), size_(0) {}

  ~map() { clear(); }

  void clear() {
    root_ = clean_up(root_);
    size_ = 0;
  }

  size_type size() const { return size_; }
  int empty() const { return !size_; }

  mapped_type& at(const key_type& key) {
    // BCC 3.1 doesn't supprot const casting.
    pnode temp = search(root_, key);
    assert(temp);
    return temp->kv.second;
  }

  const mapped_type& at(const key_type& key) const {
    pnode temp = search(root_, key);
    assert(temp);
    return temp->kv.second;
  }

  mapped_type& operator[](const key_type& key) {
    // TODO(viktors): Optimize.
    pnode temp = search(root_, key);
    // Default value.
    if (!temp) insert(key, mapped_type());
    temp = search(root_, key);
    return temp->kv.second;
  }

  void insert(const key_type& key, const mapped_type& val) {
    root_ = insert(root_, key, val);
  }

  void erase(const key_type& key) { root_ = erase(root_, key); }

 private:
  struct node {
    pair<Key, Value> kv;
    node *left, *right;
    int color;
    node(const Key& key, const Value& val)
        : kv(make_pair(key, val)), left(0), right(0), color(1) {}
    node(const Key& key)
        : kv(make_pair(key, Value())), left(0), right(0), color(1) {}
  };
  typedef node* pnode;

  pnode root_;
  size_type size_;

  int exists_and_red(pnode p) { return p && p->color; }

  pnode search(pnode p, const Key& key) {
    if (!p) return 0;
    if (p->kv.first == key) return p;
    if (p->kv.first < key)
      return search(p->right, key);
    else
      return search(p->left, key);
  }

  pnode& search_min(pnode& p) {
    if (!p) return p;
    if (p->left)
      return search_min(p->left);
    else
      return p;
  }

  pnode insert(pnode p, const Key& key, const Value& val) {
    if (!p) {
      size_++;
      return new node(key, val);
    }
    if (p->kv.first == key)
      p->kv.second = val;
    else if (p->kv.first > key)
      p->left = insert(p->left, key, val);
    else
      p->right = insert(p->right, key, val);

    p = fix_up(p);

    return p;
  }

  pnode erase(pnode p, const Key& key) {
    if (!p) return 0;
    if (key < p->kv.first) {
      if (!exists_and_red(p->left) && p->left && !exists_and_red(p->left->left))
        p = move_red_left(p);
      p->left = erase(p->left, key);
    } else {
      if (exists_and_red(p->left)) p = rotate_right(p);
      if (key == p->kv.first && !p->right) {
        size_--;
        delete p;
        return 0;
      }
      if (!exists_and_red(p->right) && p->right &&
          !exists_and_red(p->right->left))
        p = move_red_right(p);
      if (key == p->kv.first) {
        pnode& successor = search_min(p->right);
        p->kv.second = successor->kv.second;
        p->kv.first = successor->kv.first;
        size_--;
        delete successor;
        successor = 0;
      } else
        p->right = erase(p->right, key);
    }
    return fix_up(p);
  }

  pnode flip_color(pnode p) {
    p->color = !p->color;
    p->left->color = !p->left->color;
    p->right->color = !p->right->color;
    return p;
  }

  pnode rotate_left(pnode p) {
    pnode temp = p->right;
    p->right = temp->left;
    temp->left = p;
    temp->color = p->color;
    p->color = 1;
    return temp;
  }

  pnode rotate_right(pnode p) {
    pnode temp = p->left;
    p->left = temp->right;
    temp->right = p;
    temp->color = p->color;
    p->color = 1;
    return temp;
  }

  pnode move_red_right(pnode p) {
    p = flip_color(p);
    if (exists_and_red(p->left->left)) {
      p = rotate_right(p);
      p = flip_color(p);
    }
    return p;
  }

  pnode move_red_left(pnode p) {
    p = flip_color(p);
    if (exists_and_red(p->right->left)) {
      p->right = rotate_right(p->right);
      p = rotate_left(p);
      p = flip_color(p);
    }
    return p;
  }

  pnode fix_up(pnode p) {
    if (exists_and_red(p->right)) p = rotate_left(p);
    if (exists_and_red(p->left) && exists_and_red(p->left->left))
      p = rotate_right(p);
    if (exists_and_red(p->left) && exists_and_red(p->right)) p = flip_color(p);
    return p;
  }

  pnode clean_up(pnode p) {
    if (p) {
      p->left = clean_up(p->left);
      p->right = clean_up(p->right);
      delete p;
    }
    return 0;
  }

 public:
  class iterator {
   public:
    iterator() : owner_(0), pt_(0) {}

    void operator++() { increment(); }

    void operator++(int k) { increment(); }

    pair<key_type, mapped_type> operator*() {
      assert(pt_);
      return pt_->kv;
    }

    pair<key_type, mapped_type>* operator->() { return &(pt_->kv); }

    int operator==(const iterator& rhs) const {
      if (!owner_) return 0;
      if (owner_ != rhs.owner_) return 0;
      if ((!pt_) != (!rhs.pt_)) return 0;
      if (!pt_) return 1;
      return (traversal_stack_ == rhs.traversal_stack_);
    }

    int operator!=(const iterator& rhs) const { return !(*this == rhs); }

   private:
    typedef map<key_type, mapped_type>::node* pnode;

    stack<pair<pnode, unsigned char> > traversal_stack_;
    pnode pt_;
    map<key_type, mapped_type>* owner_;

    iterator(map<key_type, mapped_type>* owner, pnode root, int is_end)
        : owner_(owner) {
      if (!owner || !root) is_end = 1;
      if (!is_end) {
        traversal_stack_.push(make_pair(root, (unsigned char)0));
        increment();
      } else
        pt_ = 0;
    }

    iterator(map<key_type, mapped_type>* owner, pnode root, int is_end,
             const key_type& k)
        : owner_(owner) {
      if (!owner || !root)
        pt_ = 0;
      else {
        pnode p = root;
        while (p) {
          if (p->kv.first == k) {
            traversal_stack_.push(make_pair(p, (unsigned char)2));
            pt_ = p;
            break;
          } else if (k < p->kv.first) {
            traversal_stack_.push(make_pair(p, (unsigned char)1));
            p = p->left;
          } else {
            traversal_stack_.push(make_pair(p, (unsigned char)3));
            p = p->right;
          }
        }
        if (!p) {
          traversal_stack_.clear();
          pt_ = 0;
        }
      }
    }

    void increment() {
      for (;;) {
        if (traversal_stack_.empty()) {
          pt_ = 0;
          return;
        }
        pair<pnode, unsigned char>& curr = traversal_stack_.top();
        switch (curr.second) {
          case 0: {
            curr.second++;
            if (curr.first->left) {
              traversal_stack_.push(
                  make_pair(curr.first->left, (unsigned char)0));
            }
            break;
          }
          case 1: {
            curr.second++;
            pt_ = curr.first;
            return;
          }
          case 2: {
            curr.second++;
            if (curr.first->right) {
              traversal_stack_.push(
                  make_pair(curr.first->right, (unsigned char)0));
            }
            break;
          }
          case 3: {
            traversal_stack_.pop();
          }
        }
      }
    }

    friend class map<key_type, mapped_type>;
  };

  iterator begin() { return iterator(this, root_, 0); }

  iterator end() { return iterator(this, root_, 1); }

  iterator find(const key_type& key) { return iterator(this, root_, 0, key); }
};

#endif  // _STL_MAP_H_
