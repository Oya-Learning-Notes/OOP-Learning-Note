There are lots of different types of pointers in C++.

In this post we are NOT going to talking about some STL smart pointer template like `unique_ptr` and `shared_ptr`, we are going to talking about some basic pointer types.

# Function Pointers

To create a _function pointer_ type, use the pattern below:

```
func_ret_type (*func_pointer_type_name)(func_param_type_list)
```

For example, `int (*UniaryFuncPointer)(int, int)` is a function pointer type that could point to a function that receive two `int` type value as parameters and return `int`.

```cpp
#include <iostream>

using std::cout, std::endl;

// a convenient way to create function pointer type alias
typedef int (*UniaryFuncPointer)(int, int);

int add(int x, int y){
    return x + y;
}

int sub(int x, int y){
    return x - y;
}

int main(){
    // declare variables
    int x = 5, y = 3;
    int (*funcPtr)(int, int);
    
    // funcPtr point to add
    funcPtr = add;
    cout << funcPtr(x, y) << endl;
    
    // funcPtr point to sub
    funcPtr = sub;
    cout << funcPtr(x, y) << endl;
    
    return 0;
}
```

## Function Pointer Type Alias

As the example above shows, we could use `typedef` to quickly create the type alias. We just replace the function pointer name with our new type name. In the example above, `UniaryFuncPointer` would be a function pointer type, and we could write something like: 

```cpp
UniaryFuncPointer funcPtr;
funcPtr = add;
```

# Function That Returns Array

Even this is not directly related to pointers, I want to talk about this in this post, that is, how can we return an array type from a function. The correct scheme is as follows:

```
type (*func(param_type_list))[][]...
```

Check example below:

```cpp
#include <iostream>

using std::cout, std::endl;

int (*funcThatRetTwoDimArr(int firstValue, int secondValue))[3]{
    static int arr[3][3] = {0};
    arr[0][0] = firstValue;
    arr[0][1] = secondValue;
    
    return arr;
}

int main(){
    int (*arr)[3] = {0};
    arr = funcThatRetTwoDimArr(1, 2);
    cout << arr[0][0] << endl;
    cout << arr[0][1] << endl;
    
    return 0;
}
```

Here we should notice that:

```cpp
int* arr[3]
int (*arr)[3]
```

These are two completely different declaration.

- `int* arr[3]` Declare an array with 3 elements, which's element type is `int*`.
- `int (*arr)[3]` Declare a pointer `arr`, which is point to a dynamic array which's schema is `[][3]`. This means `arr` actually should be considered a 2-dim array.

Also check out the example below to see how to operate this kind of pointers.

```cpp
#include <iostream>

using std::cout, std::endl;

int main(){
    int arr[5][3][8] = {0};
    int (*anoArr)[3][8] = arr;
    
    arr[2][1][4] = 100;
    
    // find out the value layer by layer, but using pointer operation
    cout << *(*(*(anoArr + 2) + 1) + 4) << endl;
    // directly calculate the element offset from the begin address
    cout << *(**anoArr + (2*3*8 + 1*8 + 4)) << endl;
    
    return 0;
}
```

# Member Function/Variable Pointer

In C++, we could also create pointer which point to the member function or varibles of a class.

```cpp
#include <iostream>

using std::endl, std::cout;

class TestClass {
public:
    int mValue = 0;
    void printValue(){
        cout << mValue << endl;
    }
};

int main(){
    TestClass ins = TestClass();
    void (TestClass::* printValuePtr)(void) = &TestClass::printValue;
    (ins.*printValuePtr)();
    
    return 0;
}
```

Basically, when creating pointers to member of a class, we need to **replace `*` with `<classname>::*`**.

Different from normal pointer which point to a unique single object, **member pointers point to the member in ALL instance of that class**. And when you need to use the pointer, you need to first have an instance, then:

```cpp
Class instance;
instance.*member_variable_pointer;
(instance.*member_func_pointer)(param1, param2, ...);
```

Don't forget the parenthesis when you use _member function pointer_ to call functions.

Check [Microsoft Learn - Pointers to Members](https://learn.microsoft.com/en-us/cpp/cpp/pointers-to-members?view=msvc-170) for more info.

# Pointer To Const Value

Pointer to const value could NOT mutate the value even if it's not a const pointer.

Actually in most of time you can pass the compilation if you don't use `const` pointer when pointing to const value. But below `C++11` there is one exception:

```cpp
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

int main(){
    char* str = "Hello World!";
    cout << *str << endl;
    *str = "1";
    cout << *str << endl; // compile time error
}
```

Although `str` seems like a _non-const_ pointer, but the value `"Hello World!"` is actually a `const char*` type value. **So there will be a compile error when trying mutate the memory of that string**.

Notice in `ISO C++11` and later version, declaration like `char* str = "String";` will be forbidden. You must declare the pointer as a `const`.