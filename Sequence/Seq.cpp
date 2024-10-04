#include <cassert>
#include <iostream>
#include "Seq.h"

int main() {
  // 创建一个空的Seq<int>
  Seq<int> seq1;
  std::cout << "seq1: " << (seq1 ? "not empty" : "empty") << std::endl;

  // 插入元素
  seq1.insert(3).insert(2).insert(1);
  std::cout << "seq1 after insertions: ";
  for (Seq<int> s = seq1; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试hd和tl
  std::cout << "Head of seq1: " << seq1.hd() << std::endl;
  Seq<int> seq1_tail = seq1.tl();
  std::cout << "Tail of seq1: ";
  for (Seq<int> s = seq1_tail; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试复制构造函数
  Seq<int> seq2 = seq1;
  std::cout << "seq2 (copy of seq1): ";
  for (Seq<int> s = seq2; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试赋值操作符
  Seq<int> seq3;
  seq3 = seq1;
  std::cout << "seq3 (assigned from seq1): ";
  for (Seq<int> s = seq3; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试flip
  seq1.flip();
  std::cout << "seq1 after flip: ";
  for (Seq<int> s = seq1; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试merge_sort
  Seq<int> seq4;
  seq4.insert(5).insert(3).insert(8).insert(1).insert(4);
  std::cout << "seq4 before merge_sort: ";
  for (Seq<int> s = seq4; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  Seq<int> sorted_seq4 = merge_sort(seq4);
  std::cout << "seq4 after merge_sort: ";
  for (Seq<int> s = sorted_seq4; s; ++s) {
    std::cout << *s << " ";
  }
  std::cout << std::endl;

  // 测试operator==
  std::cout << "seq1 == seq2: " << (seq1 == seq2 ? "true" : "false")
            << std::endl;
  std::cout << "seq1 == seq3: " << (seq1 == seq3 ? "true" : "false")
            << std::endl;

  return 0;
}