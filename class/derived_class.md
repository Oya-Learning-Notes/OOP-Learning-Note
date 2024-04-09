# Derived Classes Type Convert

In _C++_ , if a class is derived from another class in `public` type, then this class type could be implicitly or explicitly converted to its public super class's type.

```cpp
class Base {
    int a;
};

class Derived : public Base {
    int b;
};

int main() {
    Base ins = Derived(); // allowed
    Base *ptr = new Derived(); // allowed
    return 0;
}

// Compile pass
```

Notice this only applyed to public derived class. Try to change the derive type to `private` and see what happen.

# Constructor Of Derived Class

First if you are doing some easy work, which means that all the super class has no parameterized constructor, and the derived class itself has no member that need to call custom constructor, then you can just leave it alone and let compiler to auto generate a constructor.

Otherwise you need to write the constructor for your derived class.

There are several things you need to do in the constructor of your derived class:

- Call super class constructor (order is decided by the places they are in the derivation list)
- Initialize derived class member
- Do other things inside function body (optional)

```cpp
#include <iostream>
using namespace std;

class Base1 {
    public:
    int a = 0;
    Base1(){
        cout << "Base1 Constructor" << endl;
    }
};

class Base2 {
    public:
    int a = 0;
    Base2(){
        cout << "Base2 Constructor" << endl;
    }
};

class Derived : public Base1, public Base2 {
    public:
    int a = 0;
    
    Derived()
    : Base2(), Base1()
    {
        cout << "Derived Constructor" << endl;
    }
};

int main(){
    Derived ins = Derived();
}
```

Output:

```
Base1 Constructor
Base2 Constructor
Derived Constructor
```

# Copy Constructor

Similar to constructor, here we recommend calling super class copy constructor

```cpp
class Derived : public Base {
public:
    Derived(const Derived& anoIns) : Base(anoIns) {
        // do some copy construction work
    }
};
```

Notice that here `anoIns` is a `Derived` class type object, but `Base(const Base&)` actually receives a `Base` type object. Why it still work?

This is because the _derived class type convert_ rules that we talked about above. **Any publicly derived class type could convert to its public super class type.**

# Destructor

The situation in destructor is quite simple, since every class has **only one destructor, and it has no parameter**.

When we define destructor of the derived class, generally **we don't need to particularly consider about the base class**. That means we just need to do the work to clean the derived class, and the compiler will automatically call the destructors of all super classes.

The **calling sequence of destructor is strictly reversed with constructor**.

Check the example below:

```cpp
#include <iostream>
using std::cout, std::endl;

class Base1 {
    int a = 0;
    public:
    Base1() {
        cout << "Base1()" << endl;
    }
    ~Base1() {
        cout << "~Base1()" << endl;
    }
};

class Base2 {
    int b = 0;
    public:
    Base2() {
        cout << "Base2()" << endl;
    }
    ~Base2() {
        cout << "~Base2()" << endl;
    }
};

class Derived : public Base1, public Base2 {
    int c = 0;
    public:
    Derived() {
        cout << "Derived()" << endl;
    }
    ~Derived() {
        cout << "~Derived()" << endl;
    }
};

int main() {
    Derived ins{};
    return 0;
}
```

Output:

```
Base1()
Base2()
Derived()
~Derived()
~Base2()
~Base1()
```

