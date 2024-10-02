#include <cstring>
#include <iostream>

class Picture {
  friend std::ostream &operator<<(std::ostream &, const Picture &);
  friend Picture frame(const Picture &);
  friend Picture operator&(const Picture &, const Picture &);
  friend Picture operator|(const Picture &, const Picture &);

public:
  Picture();
  Picture(const char *const *, int);
  Picture(const Picture &);
  ~Picture();

  Picture &operator=(const Picture &);

private:
  int height;
  int width;
  char *data;
  void copyblock(int, int, const Picture &);
  void clear(int r1, int c1, int r2, int c2);
  void init(int h, int w);
  static int max(int, int);
  char position(int row, int col) const { return data[row * width + col]; }
  char &position(int row, int col) { return data[row * width + col]; }
};
