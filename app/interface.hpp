

#include <memory>

class manual{
protected:
  unsigned double value {0};
public:
  virtual void operator()=0;
  virtual manual* next();
  void setvalue(unsigned double);
}

class move_front:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_delay>();
  };
}

class move_back:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_delay>();
  };
}

class move_left:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_delay>();
  };
}

class move_right:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_delay>();
  };
}

class move_stop:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_stop>();
  };
}

class move_delay:public manual{
public:
  virtual void operator()();
  virtual manual* next(){
    return std::make_unique<move_stop>();
  };
}
