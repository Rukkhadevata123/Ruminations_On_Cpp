#include "Picture.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class String_Pic : public Picture_Node {
  friend class Picture;
  std::vector<std::string> data;
  String_Pic(const std::vector<std::string> &v) : data(v) {};
  ~String_Pic() {};
  int height() const { return data.size(); };
  int width() const;
  void display(std::ostream &os, int row, int width) const;
};

class Frame_Pic : public Picture_Node {
  friend Picture frame(const Picture &pic);
  Picture p;
  Frame_Pic(const Picture &pic) : p(pic) {};
  int height() const { return p.height() + 2; };
  int width() const { return p.width() + 2; };
  void display(std::ostream &os, int row, int width) const;
};

class Vcat_Pic : public Picture_Node {
  friend Picture operator&(const Picture &pic1, const Picture &pic2);
  Picture top, bottom;
  Vcat_Pic(const Picture &pic1, const Picture &pic2)
      : top(pic1), bottom(pic2) {};
  int height() const { return top.height() + bottom.height(); };
  int width() const { return std::max(top.width(), bottom.width()); };
  void display(std::ostream &os, int row, int width) const;
};

class Hcat_Pic : public Picture_Node {
  friend Picture operator|(const Picture &pic1, const Picture &pic2);
  Picture left, right;
  Hcat_Pic(const Picture &pic1, const Picture &pic2)
      : left(pic1), right(pic2) {};
  int height() const { return std::max(left.height(), right.height()); };
  int width() const { return left.width() + right.width(); };
  void display(std::ostream &os, int row, int width) const;
};

Picture::Picture(const Picture &pic) : p(pic.p) { pic.p->use++; };

Picture::Picture(Picture_Node *pn) : p(pn) {};

Picture::Picture(const std::vector<std::string> &v) : p(new String_Pic(v)) {};

Picture::~Picture() {
  if (--p->use == 0) {
    delete p;
  }
};

Picture &Picture::operator=(const Picture &pic) {
  pic.p->use++;
  if (--p->use == 0) {
    delete p;
  }
  p = pic.p;
  return *this;
};

Picture frame(const Picture &pic) { return new Frame_Pic(pic); }

Picture operator&(const Picture &pic1, const Picture &pic2) {
  return new Vcat_Pic(pic1, pic2);
}

Picture operator|(const Picture &pic1, const Picture &pic2) {
  return new Hcat_Pic(pic1, pic2);
}

int Picture::height() const { return p->height(); }

int Picture::width() const { return p->width(); }

void Picture::display(std::ostream &os, int row, int width) const {
  p->display(os, row, width);
}

std::ostream &operator<<(std::ostream &os, const Picture &pic) {
  for (int i = 0; i < pic.height(); i++) {
    pic.display(os, i, pic.width());
    os << std::endl;
  }
  return os;
}

int String_Pic::width() const {
  int w = 0;
  for (int i = 0; i < data.size(); i++) {
    w = std::max(w, int(data[i].size()));
  }
  return w;
}

static void pad(std::ostream &os, int x, int y) {
  for (int i = x; i < y; i++) {
    os << " ";
  }
}

void String_Pic::display(std::ostream &os, int row, int width) const {
  if (row >= height()) {
    pad(os, 0, width);
  } else {
    os << data[row];
    pad(os, data[row].size(), width);
  }
}

void Frame_Pic::display(std::ostream &os, int row, int _width) const {
  if (row < 0 || row >= height()) {
    pad(os, 0, _width);
  } else {
    if (row == 0 || row == height() - 1) {
      os << "+";
      int i = p.width();
      while (--i >= 0) {
        os << "-";
      }
      os << "+";
    } else {
      os << "|";
      p.display(os, row - 1, p.width());
      os << "|";
    }
    pad(os, width(), _width);
  }
}

void Vcat_Pic::display(std::ostream &os, int row, int _width) const {
  if (row >= 0 && row < top.height()) {
    top.display(os, row, _width);
  } else if (row < top.height() + bottom.height()) {
    bottom.display(os, row - top.height(), _width);
  } else {
    pad(os, 0, _width);
  }
}

void Hcat_Pic::display(std::ostream &os, int row, int _width) const {
  left.display(os, row, left.width());
  right.display(os, row, right.width());
  pad(os, width(), _width);
}

int main() {
  std::vector<std::string> v1 = {"Hello", "World", "This", "is", "a", "test"};
  Picture pic1(v1);
  std::cout << "Original Picture:" << std::endl;
  std::cout << pic1 << std::endl;

  Picture framedPic1 = frame(pic1);
  std::cout << "Framed Picture:" << std::endl;
  std::cout << framedPic1 << std::endl;

  std::vector<std::string> v2 = {"Another", "set", "of", "strings"};
  Picture pic2(v2);
  std::cout << "Second Picture:" << std::endl;
  std::cout << pic2 << std::endl;

  Picture hcatPic = pic1 | pic2;
  std::cout << "Horizontally Concatenated Picture:" << std::endl;
  std::cout << hcatPic << std::endl;

  Picture vcatPic = pic1 & pic2;
  std::cout << "Vertically Concatenated Picture:" << std::endl;
  std::cout << vcatPic << std::endl;

  Picture complexPic = frame(hcatPic & framedPic1);
  std::cout << "Complex Picture (Framed Horizontal & Vertical Concatenation):"
            << std::endl;
  std::cout << complexPic << std::endl;

  return 0;
}
