#include "Expr.h"
#include <iostream>
#include <string>

// Forward declaration of the friend function
class Expr_node;
class Expr;
std::ostream &operator<<(std::ostream &os, const Expr &e);

class Int_node : public Expr_node {
  friend class Expr;
  int n;
  Int_node(int k) : n(k) {};
  void print(std::ostream &os) const { os << n; };
  int eval() const { return n; };
};

class Unary_node : public Expr_node {
  friend class Expr;
  std::string op;
  Expr opnd;
  Unary_node(const std::string &s, Expr e);
  void print(std::ostream &os) const { os << '(' << op << opnd << ')'; };
  int eval() const {
    if (op == "-")
      return -opnd.eval();
    if (op == "+")
      return +opnd.eval();
    throw "error, bad op " + op + " in Unary_node::eval";
  };
};

class Binary_node : public Expr_node {
  friend class Expr;
  std::string op;
  Expr left;
  Expr right;
  Binary_node(const std::string &s, Expr l, Expr r);
  void print(std::ostream &os) const {
    os << '(' << left << op << right << ')';
  };
  int eval() const {
    if (op == "+")
      return left.eval() + right.eval();
    if (op == "-")
      return left.eval() - right.eval();
    if (op == "*")
      return left.eval() * right.eval();
    if (op == "/" && right.eval() != 0)
      return left.eval() / right.eval();
    throw "error, bad op " + op + " in Binary_node::eval";
  };
};

class Ternary_node : public Expr_node {
  friend class Expr;
  std::string op = "?";
  Expr left;
  Expr mid;
  Expr right;
  Ternary_node(const std::string &s, Expr l, Expr m, Expr r);
  void print(std::ostream &os) const {
    os << '(' << left << '?' << mid << ':' << right << ')';
  }

  int eval() const {
    if (left.eval() != 0)
      return mid.eval();
    else
      return right.eval();
  };
};

Expr::Expr(int n) : p(new Int_node(n)) {}
Expr::Expr(const std::string &op, Expr opnd) : p(new Unary_node(op, opnd)) {}
Expr::Expr(const std::string &op, Expr left, Expr right)
    : p(new Binary_node(op, left, right)) {}
Expr::Expr(const std::string &op, Expr left, Expr mid, Expr right)
    : p(new Ternary_node(op, left, mid, right)) {}

Unary_node::Unary_node(const std::string &s, Expr e) : op(s), opnd(e) {}
Binary_node::Binary_node(const std::string &s, Expr l, Expr r)
    : op(s), left(l), right(r) {}
Ternary_node::Ternary_node(const std::string &s, Expr l, Expr m, Expr r)
    : op(s), left(l), mid(m), right(r) {}

std::ostream &operator<<(std::ostream &os, const Expr &e) {
  e.p->print(os);
  return os;
}

int main() {
  // 测试 Int_node
  Expr intExpr(5);
  std::cout << "Int_node: " << intExpr << " = " << intExpr.eval() << std::endl;

  // 测试 Unary_node
  Expr unaryExpr("-", intExpr);
  std::cout << "Unary_node: " << unaryExpr << " = " << unaryExpr.eval()
            << std::endl;

  // 测试 Binary_node
  Expr leftExpr(3);
  Expr rightExpr(4);
  Expr binaryExpr("+", leftExpr, rightExpr);
  std::cout << "Binary_node: " << binaryExpr << " = " << binaryExpr.eval()
            << std::endl;

  // 测试 Ternary_node
  Expr midExpr(10);
  Expr ternaryExpr("?", leftExpr, midExpr, rightExpr);
  std::cout << "Ternary_node: " << ternaryExpr << " = " << ternaryExpr.eval()
            << std::endl;

  // 测试组合使用
  Expr combinedExpr1("+", unaryExpr, binaryExpr); // (-5) + (3 + 4)
  std::cout << "Combined Expr1: " << combinedExpr1 << " = "
            << combinedExpr1.eval() << std::endl;

  Expr combinedExpr2("*", combinedExpr1,
                     ternaryExpr); // ((-5) + (3 + 4)) * (3 ? 10 : 4)
  std::cout << "Combined Expr2: " << combinedExpr2 << " = "
            << combinedExpr2.eval() << std::endl;

  return 0;
}