# Type of Class Constructor

There are 5 general types of constructor.

- Constructor
- Default Constructor
- Copy Constructor
- Delegate Constructor
- Move Constructor

# Default Constructor

_Default Constructor_ is a class constructor has no parameter.

You could explicitly declare a default constructor for a class like below:

```cpp
class CustomClass
{
public:
    CustomClass() // default constructor
    {
        // do something here
    }
}
```

Compiler will automatically generate a default constructor if all following condition met:

- No any other constructor.
- No explicit `delete` of the default constructor.

## Prevent Default Constructor Auto-generate

If you want to explicitly tell compiler you don't need a default constructor, you can use `delete` keyword.

```cpp
CustomClass() = delete;
```

Then when you try to create a new instance by calling the default constructor, you will receive compliant by compiler.

```cpp
CustomClass instance; // Error: the default constructor of "CustomClass" cannot be referenced -- it is a deleted function
```

# Copy Constructor

_Copy Constructor_ will be called when you try to create a new instance of this class based on the data of an existing instance.

The template of copy constructor of class `A` will be:

```cpp
class A{
    public:
    A(const A&);
};
```

Notice that the parameter should be a `const` reference.

## When will be called

More detailed, copy constructor will be called in following cases:

- Creating new instance from existing one.
- Passing class instance as a function parameter by value.
- Return a class instance. (Not really in morden compiler, we will talk more about this in the _move constructor_ part)

```cpp
CustomClass a;       // default constructor called
CustomClass b(a);    // copy constructor called
CustomClass c = a;   // copy constructor called

void some_func(CustomClass ins);
some_func(c);        // copy constructor called
```

## Copy Constructor Overrides

Generally when we declare a custom copy constructor of a class `T`, we will set the first param `const T &` ,but is it mandatory? Let's find out in rest of this post.

Check out the code below:

```cpp
// g++ std:c++20

#include <iostream>

using std::cout, std::endl;

class TestClass
{
private:
    int mValue = 0;

public:
    // constructor
    TestClass(int value = 0) : mValue(value) {}

    // copy constructor with first param T&
    TestClass(TestClass &ano) : mValue(ano.mValue)
    {
        ++ano.mValue;
        cout << "TestClass(TestClass &)" << endl;
    }

    // copy constructor with first param const T &
    TestClass(const TestClass &ano) : mValue(ano.mValue) { cout << "TestClass(const TestClass &)" << endl; }

    void show() const
    {
        cout << mValue << endl;
    }
};

int main()
{
    TestClass ins;
    const TestClass constIns;
    TestClass ins3{ins};
    TestClass ins4{constIns};
    ins.show();
    constIns.show();

    return 0;
}

/**
Output: 

TestClass(TestClass &)
TestClass(const TestClass &)
1
0
*/
```

This program shows that we could **have more than one copy constructor in a single class with different param list**. _(here `const` and non-const param will be considered different and so could be used to mark two different override of copy constructor)_

Then when the place copy constructor is called, compiler will call the proper copy constructor override based on the instance (If it's `const` etc. ).

For more info, check out: [Cpp References - Class Copy Constructor](https://en.cppreference.com/w/cpp/language/copy_constructor)

# Move Constructor

To be simple, move constructor will be called when construct a new instance from an existing _rValue_ (temporary) instance.

If you don't know what is _lValue_ and _rValue_ , then you need to figure it out first before continue learning about move constructor.

Check example below:

```cpp
class CustomClass;

CustomClass a;
CustomClass b = std::move(a);   // move constructor called
```

The example is simple, `std::move(a)` turns a into a _rValue_ .

# RVO (Return Value Optimization)

Morden C++ compiler has done a great work to try to reduce the times we call copy constructor.

```cpp
CustomClass some_func(CustomClass ins) // ins1 in main has been copyed to local variable ins here
{
    ins.id++;
    CustomClass tmpIns = ins;  // copy constructor called: ins -> tmpIns
    tmpIns.id++;
    return tmpIns;             // copy/move assignment operator called: tmpIns -> ins2 (in main), 
    // you can simply consider this equals ins2(in main) = tmpIns (in some_func)
}

int main()
{
    // CustomClass instance;
    CustomClass ins1(1);     // constructor called
    CustomClass ins2;        // default constructor called
    ins2 = some_func(ins1);  // copy constructor called: ins1 -> ins(local in some_func)
    cout << ins2.id;
    return 0;
}
```

Output of the code above is:

```
Constructor Called
Default Constructor Called
Copy Constructor Called: existId: 1
Copy Constructor Called: existId: 2
Copy/Move Assignment Operator Called: existId: 3
3
```

> `existId` here means the `id` of the instance that being copied. `id` is a `int` type member variable of this class.

The last line:

```
Copy/Move Assignment Operator Called: existId: 3
```

This means if this class has an move assignment operator, then move assign will be used, otherwise, copy assignment will be used.

Let's see another example:

```cpp
CustomClass some_func(CustomClass ins)
{
    ins.id++;
    CustomClass tmp = CustomClass(ins);     // Copy Constructor Called: ins->tmp
    CustomClass tmp2 = tmp;                 // Copy Constructor Called: tmp->tmp2
    return tmp2;                            // do nothing, since tmp2 actually use the addr of ins3
}

int main()
{
    // CustomClass instance;
    CustomClass ins1(1);                    // Constructor Called
    CustomClass ins3 = some_func(ins1);     // Copy Constructor: ins1 -> ins (local in some_func)
    return 0;
}
```

Output:

```
Constructor Called
Copy Constructor Called: existId: 1
Copy Constructor Called: existId: 2
Copy Constructor Called: existId: 2
```

So based on above experiment we know, morden C++ compiler will do more than we expected. 

## Receiving Variable Created Before Function Return

If the receiving variable (for example `ins2 = some_func(ins1);`, then `ins2` is the receiveing variable here) has been created already when function return, then function will try to call _move assignment operator_ first, if no such operator, then compiler will try calling _copy assignment operator_ .

## Receiving Variable Not Created Before Function Return

If the receiving variable not been created, then the return value and the receiving value is actually a same instance! You don't even need to move them. For example in second example, `tmp2` created in `some_func()` is actually use the same address of `ins3` in `main()` function. 

In this situation, the allocation of memory is done by original calling function (In the second example, that is `main()` is responsible to find a place in memory which will be used to store `ins3` in main, a.k.a.: `tmp2` in `some_func()`, they are the same thing in this example). However `main()` is not responsible to initialize this memory, `some_func()` is responsible to initialize a new `CustomClass` instance on this memory. Check the diagram below:

![image.png](https://s2.loli.net/2024/03/16/OjwotdPQnqRWVGc.png)

If you are using TSU Press - C++ Programming as your textbook, you can check out P145 for more info.

# Other Cases

Actually when we create derived classes, or when taking virtual inherited into consideration, there will be more other things to notice when creating constructor/destructor, check out another post about derived class for more info.