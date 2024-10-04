template <typename T> class Seq;

template <typename T> class Seq_item {
  friend class Seq<T>;

  int use;
  const T data;
  Seq_item *next;

  Seq_item(const T &val) : use(1), data(val), next(nullptr) {}
  Seq_item(const T &val, Seq_item *item) : use(1), data(val), next(item) {
    if (item)
      item->use++;
  }

  bool operator==(const Seq_item &other) const {
    return data == other.data;
  }
};

template <typename T> class Seq {
private:
  Seq_item<T> *item;
  Seq(Seq_item<T> *other) : item(other) {
    if (other)
      other->use++;
  }
  void destroy(Seq_item<T> *other) {
    while (other && --other->use == 0) {
      Seq_item<T> *next = other->next;
      delete other;
      other = next;
    }
  }

public:
  Seq() : item(nullptr) {};
  Seq(const T &val, const Seq &seq) : item(new Seq_item<T>(val, seq.item)) {};
  Seq(const Seq &seq) : item(seq.item) {
    if (item)
      item->use++;
  }
  ~Seq() { destroy(item); }

  int length() const {
    int len = 0;
    Seq_item<T> *current = item;
    while (current) {
      len++;
      current = current->next;
    }
    return len;
  }

  friend bool operator==(const Seq &s1, const Seq &s2) {
    if (s1.length() != s2.length()) {
      return false;
    }
    Seq_item<T> *item1 = s1.item;
    Seq_item<T> *item2 = s2.item;
    while (item1 && item2) {
      if (!(item1->data == item2->data)) {
        return false;
      }
      item1 = item1->next;
      item2 = item2->next;
    }
    return true;
  }

  Seq &operator=(const Seq &seq) {
    if (seq.item)
      seq.item->use++;
    destroy(item);
    item = seq.item;
    return *this;
  }
  T hd() const {
    if (item)
      return item->data;
    throw "empty sequence";
  }
  Seq tl() const {
    if (item)
      return Seq(item->next);
    throw "empty sequence";
  }
  operator bool() const { return item != nullptr; }

  Seq<T> &operator++() {
    if (item) {
      Seq_item<T> *tmp = item->next;
      if (tmp) {
        tmp->use++;
      }
      if (--item->use == 0) {
        delete item;
      }
      item = tmp;
    }
    return *this;
  }

  Seq<T> operator++(int) {
    Seq<T> tmp = *this;
    if (item) {
      --item->use;
      item = item->next;
      if (item) {
        item->use++;
      }
    }
    return tmp;
  }

  T operator*() const { return hd(); }

  Seq<T> &insert(const T &val) {
    item = new Seq_item<T>(val, item);
    return *this;
  }

  Seq_item<T> *owntail() {
    if (item == nullptr) {
      return nullptr;
    }
    Seq_item<T> *tmp = item;
    Seq_item<T> **ptmp = &item;
    while (tmp->use == 1) {
      if (tmp->next == nullptr) {
        return *ptmp;
      }
      ptmp = &tmp->next;
      tmp = tmp->next;
    }
    *ptmp = new Seq_item<T>(tmp->data);
    --tmp->use;
    tmp = tmp->next;
    Seq_item<T> *j = *ptmp;
    while (tmp) {
      j->next = new Seq_item<T>(tmp->data);
      j = j->next;
      tmp = tmp->next;
    }
    return j;
  }

  Seq<T> &flip() {
    if (item) {
      Seq_item<T> *tmp = owntail();
      Seq_item<T> *curr = item;
      Seq_item<T> *prev = nullptr;
      do {
        Seq_item<T> *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
      } while (curr);
      item = tmp;
    }
    return *this;
  }
};

template <typename T> Seq<T> cons(const T &val, const Seq<T> &seq) {
  return Seq<T>(val, seq);
}

template <typename T> Seq<T> merge(Seq<T> s1, Seq<T> s2) {
  if (!s1)
    return s2;
  if (!s2)
    return s1;
  if (s1.hd() < s2.hd()) {
    return cons(s1.hd(), merge(s1.tl(), s2));
  } else {
    return cons(s2.hd(), merge(s1, s2.tl()));
  }
}

template <typename T> void split(Seq<T> s, Seq<T> &s1, Seq<T> &s2) {
  while (s) {
    s1.insert(s.hd());
    if (++s) {
      s2.insert(s.hd());
      ++s;
    }
  }
}

template <typename T> Seq<T> merge_sort(Seq<T> s) {
  if (!s || !s.tl()) {
    return s;
  }
  Seq<T> s1, s2;
  split(s, s1, s2);
  return merge(merge_sort(s1), merge_sort(s2));
}