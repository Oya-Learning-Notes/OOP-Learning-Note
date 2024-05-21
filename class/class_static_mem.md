We can declare `const` or `static` member in a class.

# Declare and Define a static member

Generally, C++ requires divide the _declaration_ and _definition_ when declaring `static` member.

```cpp
class A{
    private:
    static int member = 0; // Error!
};

class B{
    private:
    static int member;
};
int B::member = 100; // OK
```

Notice in definition __we don't need (and actually shouldn't) to add `static` keyword again__.

This is because __C++ need to allocate memory__ to this `static` member, but it __only has one copy__ and __should not be store in every single instance of this class__.

# const static Special Case

Notice when a member is `const static`, then you could prefer to directly define the value when declaring.

```cpp
class A {
    const static member = 100; // OK
};

class B {
    const static member;
};
const B::member = 100; // OK
```