#include "Cluster.h"
#include <cstdlib>

Cluster::~Cluster() {
  while (head) {
    ClusterItem *tmp = head->next;
    delete head;
    head = tmp;
  }
}

static Cluster *cp;

void *ClusterItem::operator new(size_t t, Cluster &c) {
  cp = &c;
  return ::operator new(t);
}

ClusterItem::ClusterItem() {
  next = cp->head;
  cp->head = this;
}

void *ClusterItem::operator new(size_t t) noexcept {
  abort();
  return nullptr;
}