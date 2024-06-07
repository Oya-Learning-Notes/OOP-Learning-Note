How does _Copy Constructor_ works in derived class?

```cpp
// Example 1
#include <iostream>
using std::cout, std::endl;

class Base{
    int mValueBase = 0;
    public:

    Base(){
        cout << "Base::Base()" << endl;
    }

    Base(const Base& ins){
        cout << "Base::Base(const Base&)" << endl;
    }
};

class Derived : private Base{
    int mValueDerived = 0;
    public:

    Derived(){
        cout << "Derived::Derived()" << endl;
    }

    Derived(const Derived& ins)
    : Base(ins)
    {
        cout << "Derived::Derived(const Derived&)" << endl;
    }
};

int main(){
    Derived ins1{};
    Derived ins2{ins1};
    // Base& baseRef = ins1;  // error: ‘Base’ is an inaccessible base of ‘Derived’
    return 0;
}
```


# Base Has Default Constructor

If __don't explicitly pass parameter to copy constructor to `Base`, then will call default constructor__ (even if there is explict constructor in base class).

```
// Example 1 Output (Not explicitly call copy cons)
Base::Base()
Derived::Derived()
Base::Base()
Derived::Derived(const Derived&)
```

If explicitly call `Base` copy constructor, default constructor will not called.

```
// Example 1 Output (explicitly call copy cons)
Base::Base()
Derived::Derived()
Base::Base(const Base&)
Derived::Derived(const Derived&)
```

# Base Has No Default Constructor

```cpp
#include <iostream>
using std::cout, std::endl;

class Base{
    int mValueBase = 0;
    public:

    Base() = delete;

    Base(int value) : mValueBase(value){
        cout << "Base::Base()" << endl;
    }

    Base(const Base& ins){
        cout << "Base::Base(const Base&)" << endl;
    }
};

class Derived : private Base{
    int mValueDerived = 0;
    public:

    Derived() : Base(0) {
        cout << "Derived::Derived()" << endl;
    }

    Derived(const Derived& ins)
    {
        cout << "Derived::Derived(const Derived&)" << endl;
    }
};

int main(){
    Derived ins1{};
    Derived ins2{ins1};
    // Base& baseRef = ins1;  // error: ‘Base’ is an inaccessible base of ‘Derived’
    return 0;
}
```

```
error: use of deleted function ‘Base::Base()’
```

This means if you __do NOT explicitly call _Copy Constructor_, and `Base` don't have default constructor, then error occurred.__

Also, even if __`Base` has no explicit _Copy Construcotr___, you __can use it in `Derived` since compiler will generate one for you__.

# `const` Version

Copy Constructor with non-const param in `Derived` could not call Copy Constructor in `Base` with `const` param. (Obviously)