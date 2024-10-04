#include <cstddef>
class ClusterItem;

class Cluster {
  friend ClusterItem;

public:
  ClusterItem *head;
  Cluster(const Cluster &other);
  Cluster() : head(nullptr) {}
  ~Cluster();
  Cluster &operator=(const Cluster &other);
};

class ClusterItem {
  friend class Cluster;

public:
  void *operator new(size_t t, Cluster &c);
  ClusterItem();

protected:
  virtual ~ClusterItem();

private:
  ClusterItem *next;
  void *operator new(size_t t) noexcept;
  ClusterItem(const ClusterItem &other);
  ClusterItem &operator=(const ClusterItem &other);
};
