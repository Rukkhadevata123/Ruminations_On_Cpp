#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <stdexcept>

// Forward declarations
template <typename T> class Array;
template <typename T> class Array_data;
template <typename T> class Ptr_to_const;
template <typename T> class Pointer;

// Ptr_to_const class
template <typename T> class Ptr_to_const {
  // Allow Array and Array_data to access private members of Ptr_to_const
  friend class Array<T>;
  friend class Array_data<T>;
  friend class Pointer<T>;

  friend Ptr_to_const<T> operator+(const Ptr_to_const<T> &p, int n);
  friend Ptr_to_const<T> operator+(int n, const Ptr_to_const<T> &p);
  friend Ptr_to_const<T> operator-(const Ptr_to_const<T> &p, int n);

  friend int operator-(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
    return int(p1.index) - int(p2.index);
  }
  friend int operator-(const Pointer<T> &p1, const Pointer<T> &p2);

  friend bool operator==(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);
  friend bool operator!=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);
  friend bool operator<(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);
  friend bool operator>(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);
  friend bool operator<=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);
  friend bool operator>=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2);

public:
  Ptr_to_const(const Array<T> &a, unsigned i = 0);
  Ptr_to_const();
  Ptr_to_const(const Ptr_to_const &p);
  ~Ptr_to_const();
  Ptr_to_const &operator=(const Ptr_to_const &p);
  const T &operator*() const;

  Ptr_to_const &operator++() {
    ++index;
    return *this;
  }

  Ptr_to_const operator++(int) {
    Ptr_to_const tmp = *this;
    ++index;
    return tmp;
  }

  Ptr_to_const &operator--() {
    --index;
    return *this;
  }

  Ptr_to_const operator--(int) {
    Ptr_to_const tmp = *this;
    --index;
    return tmp;
  }

  Ptr_to_const &operator+=(int n) {
    index += n;
    return *this;
  }

  Ptr_to_const &operator-=(int n) {
    index -= n;
    return *this;
  }

private:
  Array_data<T> *ap;
  unsigned index;
};

// Pointer class
template <typename T> class Pointer : public Ptr_to_const<T> {

  // Declare Pointer as a friend of Ptr_to_const to access private members
  friend class Ptr_to_const<T>;

public:
  Pointer(Array<T> &a, unsigned i = 0);
  Pointer();
  T &operator*() const;

  Pointer &operator++() {
    ++this->index;
    return *this;
  }

  Pointer operator++(int) {
    Pointer tmp = *this;
    ++this->index;
    return tmp;
  }

  Pointer &operator--() {
    --this->index;
    return *this;
  }

  Pointer operator--(int) {
    Pointer tmp = *this;
    --this->index;
    return tmp;
  }

  Pointer &operator+=(int n) {
    this->index += n;
    return *this;
  }

  Pointer &operator-=(int n) {
    this->index -= n;
    return *this;
  }

  friend int operator-(const Pointer<T> &p1, const Pointer<T> &p2) {
    return int(p1.index) - int(p2.index);
  }
};

// Array class
template <typename T> class Array {
  // Friend declarations to allow access to private members
  friend class Pointer<T>;
  friend class Ptr_to_const<T>;

public:
  explicit Array(unsigned size);
  ~Array();
  const T &operator[](unsigned i) const;
  T &operator[](unsigned i);

  void resize(unsigned new_size) { data->resize(new_size); }

  Array(const Array &a) : data(new Array_data<T>(a.data->sz)) {
    data->copy(a.data->data, a.data->sz);
  }

  Array &operator=(const Array &a) {
    if (this != &a) {
      data->clone(a.data);
    }
  }

private:
  Array_data<T> *data;
};

// Array_data class
template <typename T> class Array_data {
  // Friend declarations to allow access to private members
  friend class Array<T>;
  friend class Ptr_to_const<T>;
  friend class Pointer<T>;

  T *data;
  unsigned sz;
  int use;

  Array_data(unsigned size);
  ~Array_data();

  const T &operator[](unsigned i) const;
  T &operator[](unsigned i);

  void copy(T *arr, unsigned size);
  void resize(unsigned new_size);
  void grow(unsigned new_size);
  void clone(const Array_data &);

  Array_data(const Array_data &);
  Array_data &operator=(const Array_data &);
};

// Implementation section

// Ptr_to_const class implementation
template <typename T>
Ptr_to_const<T>::Ptr_to_const(const Array<T> &a, unsigned i)
    : ap(a.data), index(i) {
  ++ap->use;
}

template <typename T> Ptr_to_const<T>::Ptr_to_const() : ap(nullptr), index(0) {}

template <typename T>
Ptr_to_const<T>::Ptr_to_const(const Ptr_to_const &p)
    : ap(p.ap), index(p.index) {
  if (ap)
    ++ap->use;
}

template <typename T> Ptr_to_const<T>::~Ptr_to_const() {
  if (ap && --ap->use == 0)
    delete ap;
}

template <typename T>
Ptr_to_const<T> &Ptr_to_const<T>::operator=(const Ptr_to_const &p) {
  if (p.ap)
    ++p.ap->use;
  if (ap && --ap->use == 0)
    delete ap;
  ap = p.ap;
  index = p.index;
  return *this;
}

template <typename T> const T &Ptr_to_const<T>::operator*() const {
  if (ap == nullptr)
    throw std::runtime_error("unbound Ptr_to_const");
  return (*ap)[index];
}

// Pointer class implementation
template <typename T>
Pointer<T>::Pointer(Array<T> &a, unsigned i) : Ptr_to_const<T>(a, i) {}

template <typename T> Pointer<T>::Pointer() : Ptr_to_const<T>() {}

template <typename T> T &Pointer<T>::operator*() const {
  if (this->ap == nullptr)
    throw std::runtime_error("unbound Pointer");
  return const_cast<T &>((*this->ap)[this->index]);
}

// Array class implementation
template <typename T>
Array<T>::Array(unsigned size) : data(new Array_data<T>(size)) {}

template <typename T> Array<T>::~Array() {
  if (--data->use == 0)
    delete data;
}

template <typename T> const T &Array<T>::operator[](unsigned i) const {
  return (*data)[i];
}

template <typename T> T &Array<T>::operator[](unsigned i) {
  return const_cast<T &>((*data)[i]);
}

// Array_data class implementation
template <typename T>
Array_data<T>::Array_data(unsigned size)
    : data(new T[size]), sz(size), use(1) {}

template <typename T> Array_data<T>::~Array_data() { delete[] data; }

template <typename T> const T &Array_data<T>::operator[](unsigned i) const {
  if (i >= sz)
    throw std::out_of_range("Array subscript out of range");
  return data[i];
}

template <typename T> T &Array_data<T>::operator[](unsigned i) {
  if (i >= sz)
    throw std::out_of_range("Array subscript out of range");
  return data[i];
}

template <typename T> void Array_data<T>::copy(T *arr, unsigned size) {
  for (unsigned i = 0; i < size; ++i) {
    data[i] = arr[i];
  }
}

template <typename T> void Array_data<T>::resize(unsigned new_size) {
  if (new_size == sz)
    return;
  T *old_data = data;
  data = new T[new_size];
  copy(old_data, sz > new_size ? new_size : sz);
  delete[] old_data;
  sz = new_size;
}

template <typename T> void Array_data<T>::grow(unsigned new_size) {
  unsigned nsz = sz;
  if (nsz == 0)
    nsz = 1;
  while (nsz < new_size)
    nsz *= 2;
  resize(nsz);
}

template <typename T> void Array_data<T>::clone(const Array_data &a) {
  delete[] data;
  data = new T[sz = a.sz];
  copy(a.data, a.sz);
}

template <typename T> Pointer<T> operator+(const Pointer<T> &p, int n) {
  Pointer<T> tmp = p;
  tmp += n;
  return tmp;
}

template <typename T> Pointer<T> operator+(int n, const Pointer<T> &p) {
  return p + n;
}

template <typename T> Pointer<T> operator-(const Pointer<T> &p, int n) {
  Pointer<T> tmp = p;
  tmp -= n;
  return tmp;
}

template <typename T>
Ptr_to_const<T> operator+(const Ptr_to_const<T> &p, int n) {
  Ptr_to_const<T> tmp = p;
  tmp += n;
  return tmp;
}

template <typename T>
Ptr_to_const<T> operator+(int n, const Ptr_to_const<T> &p) {
  return p + n;
}

template <typename T>
Ptr_to_const<T> operator-(const Ptr_to_const<T> &p, int n) {
  Ptr_to_const<T> tmp = p;
  tmp -= n;
  return tmp;
}

template <typename T>
bool operator==(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  if (p1.ap != p2.ap)
    return false;
  return p1.index == p2.index;
}

template <typename T>
bool operator!=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  return !(p1 == p2);
}

template <typename T>
bool operator<(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  if (p1.ap != p2.ap)
    throw std::runtime_error("comparing unrelated pointers");
  return p1.index < p2.index;
}

template <typename T>
bool operator>(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  return p2 < p1;
}

template <typename T>
bool operator<=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  return !(p2 < p1);
}

template <typename T>
bool operator>=(const Ptr_to_const<T> &p1, const Ptr_to_const<T> &p2) {
  return !(p1 < p2);
}

#endif // ARRAY_H