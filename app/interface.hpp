#pragma once

#include <memory>

class move_stop;

class manual{
public:
  virtual void operator()=0;
  virtual std::unique_ptr<move_stop> next();
};

class move_front:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};

class move_back:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};

class move_left:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};

class move_right:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};

class move_stop:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};

class move_delay:public manual{
public:
  virtual void operator()();
  virtual std::unique_ptr<move_stop> next(){
    return std::make_unique<move_stop>();
  };
};
