We are going to discuss 3 different kind of cast in C++.

- `static_cast`
- `reinterpret_cast`
- `const_cast`

# static_cast<> / dynamic_cast<>

These two are introduced in [Microsoft Learn Article](https://learn.microsoft.com/en-us/cpp/cpp/dynamic-cast-operator?view=msvc-170).

Basically these two are used to **moving a class pointer instance upper or lower in it's derivation hierarchy**.

Based on the article, `dynamic_cast` will do runtime check, and will prevent ambiguous convertion, while `static_cast` completely trust programmer's assertion. `static_cast` could used to cast a `enum` instance to its underlying type.

# const_cast<>

This could remove the `const` limit of a value.

# reinterpret_cast

Will directly reinterpret the content in the memory, do NOT use unless if you know what you are doing.


# Refs

[Microsoft Learn - C++ Casting Operator](https://learn.microsoft.com/en-us/cpp/cpp/casting-operators?view=msvc-170)