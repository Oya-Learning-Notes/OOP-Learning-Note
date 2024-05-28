# Virtual Function Description

A virtual function is a member function that you expect to be redefined in derived classes. When you **refer to a derived class object using a pointer or a reference** to the base class, you can call a virtual function for that object and **execute the derived class's version of the function**. (From Microsoft Learn C++)

You could check out Microsoft Learn Link (In _Refs_ part below) for more code snippets.

# Virtual Keyword

To declared a keyword, you only need to use `virtual` keyword in Base class method declaration, and virtual keyword in sub-classes is not necessary.

You could consider using `override` in sub-class to tell compiler that you are overriding a method in base class to get more accurate and detailed error info when there are some mistakes.

# Private Override Function

This is a popular design pattern of how we use the properties of virtual function.

Even if we **declared a _private virtual function_ inside base class**, the derived class **could also override this virtual function**. 

```cpp
#include <iostream>

using std::cout, std::endl;

class Base {
private:
    virtual Base& addValue(){
        mValue += 1;
        return *this;
    }
public:
    int mValue = 0;
    Base(){};
    Base& mutate(){
        addValue();
        return *this;
    }
};

class Derived : public Base {
private:
    Base& addValue() override {
        mValue += 2;
        return *this;
    }
};

int main() {
    Base* ins = new Derived();
    ins->mutate();
    cout << ins->mValue << endl;
    return 0;
}
```

In this case, we override `addValue()` inside derived class, and when we call `mutate()` in base class. Even if `mutate()` itself is non-virtual and it's base class version, but `mValue` still be added by 2.

In total, that is:

- Declare _public interface_ in `Base`, do NOT override.
- _public interface_ calls and use _private virtual functions_ .
- `Derived` could override _private virtual functions_ if needed.

For more info check out [this post](http://www.gotw.ca/publications/mill18.htm) discussing virtual function pattern.

# Private Base Class Destructor

Here we recommend if we need to use `delete` to destroy derived class polymorphically, then we should mark the destructor of the base class `virtual`.

```cpp
#include <iostream>

using std::cout, std::endl;

class Base {
    public:
    Base(){}
    ~Base(){
        cout << "Base Destructor" << endl;
    }
};

class Derived : public Base {
    public:
    Derived(){}
    ~Derived(){
        cout << "Derived Destructor" << endl;
    }
};

int main(){
    Base* ins = new Derived();
    delete ins; // only output: Base Destructor
    return 0;
}
```

In example above, when we use `delete` to destroy a derived class instance by using a pointer of the base class type, `~Derived()` has NOT been called which is a big problem.

To solve this problem and let the destruction start from the bottom, we should use `virtual` to mark the destructor in base class.

When we add `virtual ~Base()`, then the derived class constructor will be called as we expected.

# Override Keyword

Notice that `override` keyword could only be used with method that marked `virtual`.

# Ref

[Microsoft Learn - Virtual Function](https://learn.microsoft.com/en-us/cpp/cpp/virtual-functions?view=msvc-170)

[Virtual Function Questions And Discussion](http://www.gotw.ca/publications/mill18.htm)