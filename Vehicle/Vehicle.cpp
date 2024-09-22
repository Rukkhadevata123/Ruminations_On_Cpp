class Vehicle {
public:
  virtual double weight() const = 0;
  virtual void start() = 0;
  virtual Vehicle *copy() const = 0;
  virtual ~Vehicle() {};
};

class VehicleSurrogate {
public:
  VehicleSurrogate() : vp(0) {};
  VehicleSurrogate(const Vehicle &v) : vp(v.copy()) {};
  ~VehicleSurrogate() { delete vp; };
  VehicleSurrogate(const VehicleSurrogate &v) : vp(v.vp ? v.vp->copy() : 0) {};
  VehicleSurrogate &operator=(const VehicleSurrogate &v) {
    if (this != &v) {
      delete vp;
      vp = v.vp ? v.vp->copy() : 0;
    }
    return *this;
  };
  double weight() const {
    if (vp)
      return vp->weight();
    else
      throw "No Vehicle";
  };
  void start() {
    if (vp)
      vp->start();
    else
      throw "No Vehicle";
  };

private:
  Vehicle *vp;
};

class Automobile : public Vehicle {
public:
  double weight() const { return 3000; };
  void start() {};
  Vehicle *copy() const { return new Automobile(*this); };
};

int main() {

  VehicleSurrogate parkingLot[100];
  Automobile a;
  parkingLot[0] = a;
}
