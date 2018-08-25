#pragma once

#include <memory>
#include <string>
/**********自動ロボット***********/
class automatic {
public:
    automatic() = default;
    automatic(const automatic&) = default;
    virtual ~automatic() = default;
    virtual void operator()() = 0;
    virtual automatic* next() = 0;
    virtual std::string getname() const = 0;
};

/**********手動ロボット***********/
class manual {
public:
    manual() = default;
    manual(const manual&) = default;
    virtual ~manual() = default;
    virtual void operator()() = 0;
    virtual manual* next() = 0;
    virtual std::string getname() const = 0;
};

class MoveFront : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveFront";
    }
};

class MoveBack : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveBack";
    }
};

class MoveLeft : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveLeft";
    }
};

class MoveRight : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveRight";
    }
};

class MoveStop : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveStop";
    }
};

class MoveDelay : public manual {
public:
    virtual void operator()();
    virtual manual* next();
    virtual std::string getname() const{
      return "MoveDelay";
    }
};
