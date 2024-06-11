#include <iostream>
#include <memory>

using std::move, std::forward;
using std::cout, std::endl;
using std::make_unique, std::make_shared, std::unique_ptr, std::shared_ptr;

class Base1{
    public:

    Base1(){
        cout << "Base1::Base1()" << endl;
    }
};

class VirtualBase{
    public:

    VirtualBase(){
        cout << "VirtualBase::VirtualBase()" << endl;
    }
};

class Mid1 : virtual public VirtualBase{
    public:

    Mid1() : VirtualBase() {
        cout << "Mid1::Mid1()" << endl;
    }
};

class Derived : public Base1, public Mid1{
    public:

    Derived() : VirtualBase() {
        cout << "Derived::Derived()" << endl;
    }
};

int main(){
    Derived ins{};
    return 0;
}