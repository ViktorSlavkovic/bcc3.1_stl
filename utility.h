// File: utility.h
// Author: Viktor Slavkovic
// Date: May 2016

#ifndef _STL_UTILITY_H_
#define _STL_UTILITY_H_

template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  pair(const first_type& first, const second_type& second)
      : first(first), second(second) {}

  first_type first;
  second_type second;
};

template <class T1, class T2>
int operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
int operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs.first == rhs.first && lhs.second == rhs.second);
}

template <class T1, class T2>
int operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return lhs.first < rhs.first ||
         (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <class T1, class T2>
int operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(rhs < lhs);
}

template <class T1, class T2>
int operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return rhs < lhs;
}

template <class T1, class T2>
int operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
  return !(lhs < rhs);
}

template <class T1, class T2>
pair<T1, T2> make_pair(T1 x, T2 y) {
  return pair<T1, T2>(x, y);
}

#endif  // _STL_UTILITY_H_
