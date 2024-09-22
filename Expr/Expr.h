#include <string>
#include <iostream>

class Expr;

class Expr_node {
  friend std::ostream &operator<<(std::ostream &os, const Expr &e);
  friend class Expr;

  int use;

protected:
  Expr_node() : use(1) {};
  virtual void print(std::ostream &os) const = 0;
  virtual ~Expr_node() {};
  virtual int eval() const = 0;
};

class Expr {
  friend class Expr_node;
  Expr_node *p;

  friend std::ostream &operator<<(std::ostream &os, const Expr &e);

public:
  Expr(int n);
  Expr(const std::string &op, Expr opnd);
  Expr(const std::string &op, Expr left, Expr right);
  Expr(const std::string &op, Expr left, Expr mid, Expr right);
  Expr(const Expr &e) : p(e.p) { ++p->use; };
  ~Expr() {
    if (--p->use == 0)
      delete p;
  };
  Expr &operator=(const Expr &rhs) {
    ++rhs.p->use;
    if (--p->use == 0)
      delete p;
    p = rhs.p;
    return *this;
  };
  int eval() const { return p->eval(); };
};