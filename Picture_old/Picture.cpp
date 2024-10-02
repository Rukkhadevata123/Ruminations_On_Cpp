#include "Picture.h"
#include <cstdio>
#include <cstring>
#include <iostream>

int Picture::max(int a, int b) { return a > b ? a : b; }

void Picture::init(int h, int w) {
  height = h;
  width = w;
  data = new char[height * width];
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      position(i, j) = ' ';
    }
  }
}

void Picture::copyblock(int row, int col, const Picture &p) {
  for (int i = 0; i < p.height; ++i) {
    for (int j = 0; j < p.width; ++j) {
      position(i + row, j + col) = p.position(i, j);
    }
  }
}

void Picture::clear(int r1, int c1, int r2, int c2) {
  for (int i = r1; i <= r2; ++i) {
    for (int j = c1; j <= c2; ++j) {
      position(i, j) = ' ';
    }
  }
}

Picture frame(const Picture &p) {
  Picture r;
  r.init(p.height + 2, p.width + 2);

  for (int i = 1; i < r.height; ++i) {
    r.position(i, 0) = '|';
    r.position(i, r.width - 1) = '|';
  }
  for (int i = 1; i < r.width - 1; ++i) {
    r.position(0, i) = '-';
    r.position(r.height - 1, i) = '-';
  }
  r.position(0, 0) = '+';
  r.position(0, r.width - 1) = '+';
  r.position(r.height - 1, 0) = '+';
  r.position(r.height - 1, r.width - 1) = '+';
  r.copyblock(1, 1, p);
  return r;
}

Picture::Picture() : height(0), width(0), data(0) {}

Picture::Picture(const char *const *array, int n) {
  int w = 0;
  int i;

  for (i = 0; i < n; ++i) {
    w = Picture::max(w, strlen(array[i]));
  }
  init(n, w);
  for (i = 0; i < n; ++i) {
    const char *src = array[i];
    int len = strlen(src);
    int j = 0;
    while (j < len) {
      position(i, j) = src[j];
      ++j;
    }
    while (j < width) {
      position(i, j++) = ' ';
    }
  }
}

Picture::Picture(const Picture &p)
    : height(p.height), width(p.width), data(new char[p.height * p.width]) {
  copyblock(0, 0, p);
}

Picture::~Picture() { delete[] data; }

Picture &Picture::operator=(const Picture &p) {
  if (this != &p) {
    delete[] data;
    init(p.height, p.width);
    copyblock(0, 0, p);
  }
  return *this;
}

Picture operator&(const Picture &p, const Picture &q) {
  Picture r;
  r.init(p.height + q.height, Picture::max(p.width, q.width));
  r.copyblock(0, 0, p);
  r.copyblock(p.height, 0, q);
  return r;
}

Picture operator|(const Picture &p, const Picture &q) {
  Picture r;
  r.init(Picture::max(p.height, q.height), p.width + q.width);
  r.copyblock(0, 0, p);
  r.copyblock(0, p.width, q);
  return r;
}

std::ostream &operator<<(std::ostream &os, const Picture &p) {
  for (int i = 0; i < p.height; ++i) {
    for (int j = 0; j < p.width; ++j) {
      os << p.position(i, j);
    }
    os << std::endl;
  }
  return os;
}

int main() {
  const char *init[] = {"Paris", "in the", "Spring"};
  Picture p(init, 3);
  Picture q = frame(p);
  Picture r = p | q;
  Picture s = q & r;
  Picture t = frame(s);
  std::cout << p << '\n';
  std::cout << q << '\n';
  std::cout << r << '\n';
  std::cout << s << '\n';
  std::cout << t << '\n';
}
