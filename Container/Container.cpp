#include "Container.h"
#include <iostream>

int main() {
  try {
    // 创建一个包含5个整数元素的数组
    Array<int> arr(5);

    // 使用Pointer类访问和修改元素
    Pointer<int> p1(arr, 0); // 指向第一个元素的指针
    Pointer<int> p2(arr, 1); // 指向第二个元素的指针

    // 通过Pointer对象修改数组元素
    *p1 = 10;
    *p2 = 20;

    std::cout << "修改后的数组内容:" << std::endl;
    for (unsigned i = 0; i < 5; ++i) {
      std::cout << arr[i] << " "; // 使用Array的operator[]访问数组元素
    }
    std::cout << std::endl;

    // 测试Ptr_to_const类的只读访问
    Ptr_to_const<int> const_p(arr, 0); // 指向第一个元素的只读指针

    std::cout << "通过Ptr_to_const访问第一个元素: " << *const_p << std::endl;

    // 测试数组的克隆拷贝
    Array<int> arr_copy = arr; // 使用拷贝构造函数
    std::cout << "克隆后的数组内容:" << std::endl;
    for (unsigned i = 0; i < 5; ++i) {
      std::cout << arr_copy[i] << " ";
    }
    std::cout << std::endl;

    // 修改原数组，验证克隆数组是否独立
    *p1 = 30;
    std::cout << "修改原数组后的内容:" << std::endl;
    for (unsigned i = 0; i < 5; ++i) {
      std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "克隆数组的内容应保持不变:" << std::endl;
    for (unsigned i = 0; i < 5; ++i) {
      std::cout << arr_copy[i] << " ";
    }
    std::cout << std::endl;

    // 测试数组的大小调整
    arr.resize(10);
    std::cout << "调整大小后的数组内容:" << std::endl;
    for (unsigned i = 0; i < 10; ++i) {
      std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    // 测试越界访问
    std::cout << "尝试访问越界元素（将抛出异常）:" << std::endl;
    std::cout << arr[15] << std::endl; // 应抛出std::out_of_range异常

  } catch (const std::exception &e) {
    std::cerr << "捕获异常: " << e.what() << std::endl;
  }

  return 0;
}