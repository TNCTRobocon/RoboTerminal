#pragma once

#include <memory>

class move_stop;

class manual{
public:
  virtual void operator()()=0;
  virtual manual* next()=0;
};

class move_front:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};

class move_back:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};

class move_left:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};

class move_right:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};

class move_stop:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};

class move_delay:public manual{
public:
  virtual void operator()();
  virtual manual* next();
};
