# Enum Types

[CppReference: Enum Type](https://en.cppreference.com/w/cpp/language/enum)

There are two types of enum: _Scoped Enum_ and _Unscoped Enum_ .

## Enumerator Implicit Value

When declaring and initializing the value of the enumerator, following rules are repected:

- If the first enumerator is implicit-value enumerator, then its value is `0`.
- Except the first enumerator, any implicit-value enumerator has a value bigger than `1` of its previous enumerator.

Check out code example below:

```cpp
// Example 1

#include <iostream>

using std::cin, std::cout, std::endl;

enum ColorEnum : int
{
    red = 1,
    green = 3,
    blue = 0,
    orange,
};

int main()
{
    cout << red << endl;    // 1
    cout << green << endl;  // 3
    cout << blue << endl;   // 0
    cout << orange << endl; // 1
    cout << ((red == orange) ? "True" : "False") << endl;

    return 0;
}
```

## Enumerator Comparison

Enumerator has identical compare operator with its underlying type. In other word, when you compare two enumerator, you are actually do comparison to their underlying value.

# Unscoped Enum

```cpp
enum [enum_name] [: underlying type] {
    enumerator_name_1 = const_expressions;
    // ...
}
```

## Access Enumerator

For unscoped enum, **its enumerator is directly accessable in the enclosing scope where the enum has been declared**. They (those enumerators) are considered _named constants_ .

Check out the _example 1_ again. We declare and define an _unscoped enum_ `ColorEnum` , then inside `main()` we can directly use the enumerator name to access it without any scope operator or something else.

There is another situation when an unscoped enum been declared inside a class.

```cpp
// example 2

struct X
{
    enum direction { left = 'l', right = 'r' };
};
X x;
X* p = &x;
 
int a = X::direction::left; // allowed only in C++11 and later
int b = X::left;
int c = x.left;
int d = p->left;
```

You can sonsider there are two constant declared insdie this class which is `left = 'l'` and `right = 'r'` .

## Type Conversion

```cpp
// example 3

enum ColorEnum : int
{
    red = 1,
    green = 3,
    blue = 0,
    orange,
};

int num = red;                                   // OK enum->int

ColorEnum favColor = 4;                          // Compile time error
ColorEnum favColor2 = static_cast<ColorEnum>(4); // OK int->enum
ColorEnum favColor2 = ColorEnum(4);              // OK int->enum

ColorEnum favColor3 = {4}; // OK (only when explicity specify underlying type to int)
```

# Scoped Enum

## Type Conversion

```cpp
// example 4

enum class ColorEnum
{
    red = 1,
    green = 3,
    blue = 0,
    orange,
};

// enum -> int
int num = ColorEnum::red;                    // Compile time error
int num2 = static_cast<int>(ColorEnum::red); // OK

// int -> enum
ColorEnum favColor = 4;                          // Compile time error
ColorEnum favColor2 = static_cast<ColorEnum>(4); // OK
ColorEnum favColor2 = ColorEnum(4);              // OK

ColorEnum favColor3 = {4}; // OK
// since scoped enum has default underlying type int,
// the above line is ok without explicitly declaring ColorEnum: int
```

Quite similar to _unscope enum_ , but now:
- _ColorEnum_ can't implicitly convert to its underlying type.
- It has implicit `int` underlying type.

For more info about **enum initializating by number without cast**, check the _CppReference_ page. (The link is given at the begining of this post)