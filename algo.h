// File: algo.h
// Author: Viktor Slavkovic
// Date: May 2016

#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//  Modifying sequence operations:
////////////////////////////////////////////////////////////////////////////////

template <class T>
void swap(T& a, T& b) {
  T temp(a);
  a = b;
  b = temp;
}

////////////////////////////////////////////////////////////////////////////////
//  Min / max:
////////////////////////////////////////////////////////////////////////////////

template <class T>
const T& min(const T& a, const T& b) {
  return b < a ? b : a;
}

template <class T>
const T& max(const T& a, const T& b) {
  return a < b ? b : a;
}

////////////////////////////////////////////////////////////////////////////////
//  Heap:
////////////////////////////////////////////////////////////////////////////////

template <class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
  _heap_percolate_up(first, last - first - 1);
}

template <class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last,
               Compare comp) {
  _heap_percolate_up(first, last - first - 1, comp);
}

template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
  int n = last - first;
  swap(first[0], first[n - 1]);
  _heap_percolate_down(first, n - 1, 0);
}

template <class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
              Compare comp) {
  int n = last - first;
  swap(first[0], first[n - 1]);
  _heap_percolate_down(first, n - 1, 0, comp);
}

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
  int n = last - first;
  for (int i = n / 2; i >= 0; i--) _heap_percolate_down(first, n, i);
}

template <class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last,
               Compare comp) {
  int n = last - first;
  for (int i = n / 2; i >= 0; i--) _heap_percolate_down(first, n, i, comp);
}

////////////////////////////////////////////////////////////////////////////////
//  Helper functions
////////////////////////////////////////////////////////////////////////////////

template <class RandomAccessIterator>
int _heap_min_child_idx(RandomAccessIterator first, int n, int idx) {
  int l = idx * 2 + 1;
  int r = (idx + 1) * 2;
  if (l >= n && r >= n) return -1;
  if (l >= n) return r;
  if (r >= n) return l;
  return (first[r] < first[l]) ? r : l;
}

template <class RandomAccessIterator, class Compare>
int _heap_min_child_idx(RandomAccessIterator first, int n, int idx,
                        Compare comp) {
  int l = idx * 2 + 1;
  int r = (idx + 1) * 2;
  if (l >= n && r >= n) return -1;
  if (l >= n) return r;
  if (r >= n) return l;
  return (comp(first[r], first[l])) ? r : l;
}

template <class RandomAccessIterator>
void _heap_percolate_up(RandomAccessIterator first, int idx) {
  int p = (idx - 1) / 2;
  while (idx && first[idx] < first[p]) {
    swap(first[p], first[idx]);
    idx = p;
    p = (idx - 1) / 2;
  }
}

template <class RandomAccessIterator, class Compare>
void _heap_percolate_up(RandomAccessIterator first, int idx, Compare comp) {
  int p = (idx - 1) / 2;
  while (idx && comp(first[idx], first[p])) {
    swap(first[p], first[idx]);
    idx = p;
    p = (idx - 1) / 2;
  }
}

template <class RandomAccessIterator>
void _heap_percolate_down(RandomAccessIterator first, int n, int idx) {
  int c = _heap_min_child_idx(first, n, idx);
  while (c != -1 && first[c] < first[idx]) {
    swap(first[c], first[idx]);
    idx = c;
    c = _heap_min_child_idx(first, n, idx);
  }
}

template <class RandomAccessIterator, class Compare>
void _heap_percolate_down(RandomAccessIterator first, int n, int idx,
                          Compare comp) {
  int c = _heap_min_child_idx(first, n, idx, comp);
  while (c != -1 && comp(first[c], first[idx])) {
    swap(first[c], first[idx]);
    idx = c;
    c = _heap_min_child_idx(first, n, idx, comp);
  }
}

#endif  // ALGORITHM_H_INCLUDED
