# Limit Class Member Access

We use three keywords: `public`, `protected` and `private` to control the external and derived class access to the member of a certain class.

**Strongly recommend** to first checkout [Microsoft C++ Docs: Class Access Control](https://learn.microsoft.com/en-us/cpp/cpp/member-access-control-cpp?view=msvc-170), which will describe how to use these three access specifiers.

This post is a conclusion to the docs above.

# For Base Class

`public` means: anyone from outside this class has the right to access these members.

`protected` and `private` means: only this class itself or `friend` can access these members.

# For Derived Class

Checkout [Microsoft C++ Docs - Derived Class Access Control](https://learn.microsoft.com/en-us/cpp/cpp/member-access-control-cpp?view=msvc-170#member-access-in-base-class)

The table in that page introduce how access specifier works when you make derived class from a base class.

![image.png](https://s2.loli.net/2024/03/19/rn2zxRBGWHuT5pF.png)

If we use a more direct diagram to describe this relation, we will get the result below:

![cpp_class_access_diagram.png](https://s2.loli.net/2024/03/19/xaBU72Z5PA9pcDq.png)

## Static Members

The rules introduced above only apply to _non-static_ member in base class. That means: _public static members_ will still accessible from derived class even if you use `private` to declare the base class.

```cpp
#include <iostream>
using std::cout, std::endl;

class BaseClass
{
public:
    static int count;
    int nonStaticCount;
};

// Here BaseClass is declared with private specifier in default, so the private could be omitted
// same to: class DerivedClass : BaseClass
class DerivedClass : private BaseClass
{
    void someMemberFunction()
    {
        this->count;
    }
};

class DerivedClass2 : private DerivedClass
{
    void someMemberFunction();
};

// ------------------------------------------------------

void DerivedClass::someMemberFunction()
{
    cout << ::BaseClass::count << endl; // OK
    this->count;                        // OK
}

void DerivedClass2::someMemberFunction()
{
    cout << ::BaseClass::count << endl; // OK
    this->count;                        // Inaccessible
}
```

Now we fount no matter in what derived class, the express `::BaseClass::count` is valid. You can think we **directly access this public static member by using scope operator**, which is the same thing we do if we want to access this public member outside that class.

However `this->count` works in member function of `DerivedClass`, but not works in `DerivedClass2`.

**Two explanations for me**. First one is from Microsoft docs, and another one is from myself lol.

However, in C++ class, _member functions_ and _friend classes/functions_ has the right to convert a class type to its direct base class even if it's declared in `private` or `protected`.

For me, I would like to directly considered that when we access _static_ member in base classes through instance/pointer/ref, **the static member could be considered still follow the rules of _non-static_ members**.

## Virtual Functions

Still based on the Microsoft docs, which gave us the following example:

```cpp
// example given in Microsoft docs

class VFuncBase
{
public:
    virtual int GetState() { return _state; }

protected:
    int _state;
};

class VFuncDerived : public VFuncBase
{
private:
    int GetState() { return _state; }
};

int main()
{
    VFuncDerived vfd;          // Object of derived type.
    VFuncBase *pvfb = &vfd;    // Pointer to base type.
    VFuncDerived *pvfd = &vfd; // Pointer to derived type.
    int State;

    State = pvfb->GetState(); // GetState is public.
    State = pvfd->GetState(); // C2248 error expected; GetState is private;
}
```

The conclusion is:

Accessiblity of a virtual function of a class is decide by the type of the instance that you call it.

That means if the virtual function is public in the class, then the instance/pointer/ref with this class type would have public access to this virtual function. Same to `protect` and `private`