#include <iostream>
#include <string>
#include <vector>

static void pad(std::ostream &os, int x, int y);

class Picture_Node;
class Picture {
  friend std::ostream &operator<<(std::ostream &os, const Picture &pic);
  friend Picture frame(const Picture &pic);
  friend Picture operator&(const Picture &pic1, const Picture &pic2);
  friend Picture operator|(const Picture &pic1, const Picture &pic2);
  friend class String_Pic;
  friend class Frame_Pic;
  friend class Vcat_Pic;
  friend class Hcat_Pic;

public:
  Picture(const std::vector<std::string> &v);
  Picture(const Picture &pic);
  ~Picture();
  Picture &operator=(const Picture &pic);

private:
  Picture_Node *p;
  int height() const;
  int width() const;
  void display(std::ostream &os, int row, int width) const;
  Picture(Picture_Node *pn);
};

class Picture_Node {
  friend class Picture;

protected:
  int use;
  Picture_Node() : use(1) {};
  virtual ~Picture_Node() {};
  virtual int height() const = 0;
  virtual int width() const = 0;
  virtual void display(std::ostream &os, int row, int width) const = 0;
};
