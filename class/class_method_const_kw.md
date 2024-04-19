There are several position we can add `const` keyword to the function declaration in class.

```cpp
const ret_type function_name(param_list);
ret_type const function_name(param_list);
ret_type function_name(param_list) const;
```

Actually the **first and second one is equivalent** (we will show it in example below).

We will use an example to explain the difference between them.

```cpp
#include <iostream>
using std::cout, std::endl;

class Base{
    public:
    int mData;

    public:
    Base(int initData) : mData(initData) {}
    
    const int& constIntData(){
        mData++; // ok
        return mData;
    }
    
    int const& intConstData(){
        mData++; // ok
        return mData;
    }
    
    int intDataConst() const {
        // mData++; // error
        return mData;
    }
};

int main(){
    Base ins(1);
    
    int& x = ins.constIntData();       // error
    cons int& x1 = ins.constIntData(); // ok
    
    return 0;
}
```

Based on example above we know:

-----

When `const` appeared besides the return type specifier, for example:

```cpp
const int& func();
int const& func();
```

This means the **return type of the function is `const` value**. In the example we can see that compiler throws an error if we try to use the return type too initialize a non-const int reference.

-----

When `const` appeared behind param list:

```cpp
int& func() const;
```

This means **this function is a `const` class member function**, that means this function will not change any member value of this class (except for the members that have been marked as `mutable`, which means such member could be changed even inside a const member function)

# Refs

[StackOverflow - Function Return Value Const Usecase](https://stackoverflow.com/questions/6299967/what-are-the-use-cases-for-having-a-function-return-by-const-value-for-non-built)

[StackOverflow - const int VS int const](https://stackoverflow.com/questions/3247285/const-int-int-const)