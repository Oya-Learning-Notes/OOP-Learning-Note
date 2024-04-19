There are two different ways to override an operator for a class:

- Override as non-static member function
- Override as friend function

Notice that **when using non-static member function to override operator, the class instance itself would be the implicit first operand**. That also means if some operator don't take class instance as the first operand (for example `<<`, which takes `std::ostream` as first operand), then this kind of operator could only be override by using the second method. _(you can check the example we gave out below)_

Before override, make sure that the **operator is a standard C++ operator and it's allowed to be override**. Below are some example of some operator that can NOT be override: `::`, `.`, `.*`, `?:` etc.

# Examples

Below is an example of operator override.

```cpp
#include <iostream>

using std::cout, std::endl;
using std::move, std::forward;

class Base {

    private:
    int mData = 0;

    public:
    // parameterized constructor
    Base(int&& data) : mData(data) {}

    Base(const Base& anoIns) 
    : mData(anoIns.mData)
    {
        cout << "Copy Constructor" << endl;
        cout << "Constructed value: ";
        cout << mData << endl;
    }

    // operator+ override
    //
    // This operator has implicit first operand which is 
    // this instance itself (or you can consider it *this)
    Base operator+(const Base& anoIns) {
        // create and return tmp Base instance
        return Base(mData + anoIns.mData);
    }

    // cout override
    friend std::ostream& operator<<(std::ostream& os, const Base& ins);
};

std::ostream& operator<<(std::ostream& os, const Base& ins){
    return os << ins.mData;
}

int main(){
    Base ins1(2);
    Base ins2(3);
    Base ins3 = ins1 + ins2;
    cout << ins3;
    return 0;
}
```