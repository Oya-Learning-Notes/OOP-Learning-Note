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

# Constructor

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

## With Virtual Base Classes

If there are virtual classes in the class hierarchy, the rules become:

- Initialize `virtual` super class __(still first thing even if it's indirect super class)__.
- Initialize __non-`virtual` super class__.
- Initialize __class member__ by init list.
- Execute constructor __function body__.

Here is an [Code Example](./derived_class/code_eg_constructor_order.cpp)

# Copy Constructor

Similar to constructor, here we recommend calling super class copy constructor

```cpp
class Derived : Base {
public:
    Derived(const Derived& anoIns) : Base(anoIns) {
        // do some copy construction work
    }
};
```

Notice that here `anoIns` is a `Derived` class type object, but `Base(const Base&)` actually receives a `Base` type object. Why it still work?

This is because the _derived class type convert_ rules that we talked about above. **Any publicly derived class type could convert to its public super class type.** Moreover, __as a special case, here the `public` condition is not needed anymore__. This means even if `Derived` is privitely derived from `Base`, this code will still work.

Checkout [Derived Class Copy Constructor Notes](./derived_class/copy_constructor.md) for more info.

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

# Derivation Conflict

## Multi-Base Conflict

If there are members with same names in different base classes, then conflict occured.

```cpp
#include <iostream>

using std::endl, std::cout;

class Base1{
public:
    int var;
};

class Base2{
public:
    int var;
};

class Derived : public Base1, public Base2{
public:
    int varDerived;
};

int main(){
    Derived ins;
    cout << ins.var << endl; // error: request for member ‘var’ is ambiguous
    
    return 0;
}
```

Check out diagram below:

![image](https://github.com/Oya-Learning-Notes/OOP-Learning-Note/assets/61616918/256a5802-6453-4e00-9d5b-c2b784fc9a7e)

If there is any function with the same name then same things will happen.

But what if functions in two base class has same name but different param list?

```cpp
#include <iostream>

using std::endl, std::cout;

class Base1{
public:
    int add(int x){
        return x + 1;
    }
};

class Base2{
public:
    int add(int x, int y){
        return x + y;
    }
};

class Derived : public Base1, public Base2{
public:
    int varDerived;
};

int main(){
    Derived ins;
    cout << ins.add(1) << endl;     // error: request for member ‘add’ is ambiguous
    cout << ins.add(1, 2) << endl;  // error: request for member ‘add’ is ambiguous
    
    return 0;
}
```

That means **even if param list is different, there would still have ambiguous error**.

## Base And Derived Conflict

```cpp
#include <iostream>

using std::endl, std::cout;

class Base1{
public:
    int var1 = 0;
};

class Base2{
public:
    int var2 = 0;
    int add(int x, int y){
        return x + y;
    }
};

class Derived : public Base1, public Base2{
public:
    int add(int x){
        return x + 1;
    }
};

int main(){
    Derived ins;
    cout << ins.add(1) << endl;    // OK Output: 2
    cout << ins.add(1, 2) << endl; // error: no matching function for call to ‘Derived::add(int, int)
    
    return 0;
}
```

![image](https://github.com/Oya-Learning-Notes/OOP-Learning-Note/assets/61616918/8eb6dada-5f85-4960-89e8-ccfb452fe0cf)

In fact, as long as there is a same-name function in derived class, **ALL overrides in base classes will be hidden despite of whether the param list is the same**.

One method to let compiler only hide the conflict one is add `using` statement.

```cpp
#include <iostream>

using std::endl, std::cout;

class Base1{
public:
    int var1 = 0;
};

class Base2{
public:
    int var2 = 0;
    int add(int x, int y){
        return x + y;
    }
    int add(int x){
        return x + 2;
    }
};

class Derived : public Base1, public Base2{
public:
    using Base2::add;
    int add(int x){
        return x + 1;
    }
};

int main(){
    Derived ins;
    cout << ins.add(1) << endl;     // Derived::add Output: 2
    cout << ins.add(1, 2) << endl;  // Base2::add   Output: 3
    
    return 0;
}
```

![image](https://github.com/Oya-Learning-Notes/OOP-Learning-Note/assets/61616918/3c1d7594-8527-44c0-b549-d39f6f7ce735)


## Diamond Inherit

Consider the situation below:

```cpp
#include <iostream>

using std::endl, std::cout;

class Base {
    public:
    int var;
};

class Mid1 : public Base {
    ;
};

class Mid2 : public Base {
    ;
};

class Derived : public Mid1, public Mid2 {
    ;
};

int main(){
    Derived ins = Derived();
    ins.var;
    
    return 0;
}
```

In this case, seems that there is only one `var` in `Base`, but actually both `Mid1` and `Mid2` is derived from `Base`, means there would be two different copy of `var` inside memory of class instance of `Derived`.

![image](https://github.com/Oya-Learning-Notes/OOP-Learning-Note/assets/61616918/42a7329f-d3d2-4761-b0b4-2d6cd3f2a16f)

One solution is to directly specify which `var` you want to access.

```cpp
int main(){
    Derived ins = Derived();
    cout << ins.Mid1::var << endl; // 0
    ins.Mid2::var = 100;
    cout << ins.Mid2::var << endl; // 100
    
    return 0;
}
```

Another approach is to use `virtual public` inherit when declaring `Mid1` and `Mid2`.

```cpp
#include <iostream>

using std::endl, std::cout;

class Base {
    public:
    int var;
};

class Mid1 : virtual public Base {
    ;
};

class Mid2 : virtual public Base {
    ;
};

class Derived : public Mid1, public Mid2 {
    ;
};

int main(){
    Derived ins = Derived();
    cout << ins.var << endl; // 0
    
    return 0;
}
```

Using `virtual public` inherit will tell compiler only use one piece of memory in subclasses to store info of the `Base` class.

When using `virtual public` to derive from `Base`, all direct and indirect class should take the responesiblity to call _constructor_ of this `Base` class. Check example below:

```cpp
#include <iostream>

using std::endl, std::cout;

class Base {
    public:
    int var;
    Base(int initVar) : var(initVar) {
        cout << "Base::Base(int)" << endl;
    }
};

class Mid1 : virtual public Base {
    public:
    Mid1() : Base(1) {
        cout << "Mid1::Mid1()" << endl;
    }
};

class Mid2 : virtual public Base {
    public:
    Mid2() : Base(2) {
        cout << "Mid2::Mid2()" << endl;
    }
};

class Derived : public Mid1, public Mid2 {
    public:
    Derived() : Base(3), Mid1(), Mid2() {
        cout << "Derived::Derived()" << endl;
    }
};

int main(){
    Derived ins = Derived();
    cout << ins.var << endl;
    
    return 0;
}
```

Notice that **even in indirect sub class `Derived`, we need to call `Base::Base` in constructor** when we use virtual inherit.

Here comes the questions, we directly call `Base::Base` in derived class, and we also call two mid class constructor, which will also call base class constructor. **Will `Base::Base()` be called repeatly more than one time?**

No, actually compiler will only allow the farest sub-class, in this example is `Derived`, to call `Base::Base(int)`, other calls in the middle of inherit tree will be ignored, in this example is the call from `Mid1` and `Mid2`. Check code example below:

```cpp
#include <iostream>

using std::endl, std::cout;

class Base {
    public:
    int var;
    Base(int initVar) : var(initVar) {
        cout << "Base::Base(int)" << endl;
    }
};

class Mid1 : virtual public Base {
    public:
    Mid1() : Base(1) {
        cout << "Mid1::Mid1()" << endl;
    }
};

class Mid2 : virtual public Base {
    public:
    Mid2() : Base(2) {
        cout << "Mid2::Mid2()" << endl;
    }
};

class Derived : public Mid1, public Mid2 {
    public:
    Derived() : Base(3), Mid1(), Mid2() {
        cout << "Derived::Derived()" << endl;
    }
};

int main(){
    Derived ins = Derived();
    cout << ins.var << endl;
    
    return 0;
}
```

Output:

```
Base::Base(int)
Mid1::Mid1()
Mid2::Mid2()
Derived::Derived()
3
```

![image](https://github.com/Oya-Learning-Notes/OOP-Learning-Note/assets/61616918/dc175704-b790-4356-af26-1e25bc93c2b1)