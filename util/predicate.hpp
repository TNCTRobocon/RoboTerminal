#pragma once
namespace Util {

template <class T>
class Always {
protected:
    const T value;

public:
    Always(const T& _value) : value(_value) {}
    Always(const Always<T>&) = default;
    virtual ~Always() = default;
    const T& operator()() const { return value; }
};

class AlwaysTrue : public Always<bool> {
public:
    AlwaysTrue() : Always(true) {}
};

class AlwaysFalse : public Always<bool> {
public:
    AlwaysFalse() : Always(false) {}
};

template <>
class Always<void> {
public:
    Always() = default;
    Always(const Always&) = default;
    virtual ~Always() = default;
    void operator()() {}
};

using AlwaysNone = Always<void>;
using AlwaysInt = Always<int>;

}  // namespace Util