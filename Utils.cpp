template <typename P, typename T> P find(P start, P beyond, const T &x) {
  while (start != beyond && *start != x) {
    ++start;
  }
  return start;
}

template <typename P, typename X> P binsearch(P start, P beyond, const X &x) {
  while (start < beyond) {
    P mid = start + (beyond - start) / 2;
    if (*mid == x) {
      return mid;
    }
    if (*mid < x) {
      start = mid + 1;
    } else {
      beyond = mid;
    }
  }
  return beyond;
}

template <typename P, typename T> void reverse(P start, P beyond) {
  while (start != beyond) {
    --beyond;
    if (start != beyond) {
      T t = *start;
      *start = *beyond;
      *beyond = t;
      ++start;
    }
  }
}

template <typename In, typename Out> Out copy(In start, In beyond, Out dest) {
  while (start != beyond) {
    *dest = *start;
    ++start;
    ++dest;
  }
  return dest;
}

template <typename Iter, typename UnaryFn>
void for_each(Iter start, Iter beyond, UnaryFn f) {
  while (start != beyond) {
    *start = f(*start);
    ++start;
  }
}

template <typename S, typename V> class fcn_obj {
public:
  fcn_obj(S &(*f)(S &, const V &), const V &v) : func(f), val(v) {}
  S &operator()(S &s) const { return (*func)(s, val); }

private:
  S &(*func)(S &, const V &);
  V val;
};

template <typename S, typename V> S &operator<<(S &s, const fcn_obj<S, V> &f) {
  return f(s);
}

#include <iostream>
#include <string>

std::string &append(std::string &s, const std::string &suffix) {
  s += suffix;
  return s;
}

int main() {
  std::string str = "Hello";
  std::string suffix = ", World!";
  fcn_obj<std::string, std::string> f(append, suffix);
  str << f;
  std::cout << str << std::endl; // 输出: Hello, World!
  return 0;
}