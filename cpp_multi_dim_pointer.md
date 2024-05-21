In C++ when we need to deal with array with _more than one dimension_, we may need to know how array pointer works with multi-dim array.

```cpp
#include <iostream>

using std::cout, std::endl;

int main(){
    int (*p)[4][5] = new int[3][4][5]();

    cout << p << endl;
    cout << p + 1 << endl;   // will add 80 (1 * 4 * 5 * sizeof(int)) bytes offset

    cout << *p << endl;
    cout << *p + 1 << endl;  // will add 20 (1 * 5 * sizeof(int)) bytes offset

    cout << **p << endl;
    cout << **p + 1 << endl; // will add 4 (1 * sizeof(int)) bytes offset

    cout << ***p << endl;
}
```

Check out this code example. We found that for an array with 3 dim `[x][y][z]`, it's pointer type should be `[y][z]`

# Why Compiler Don't Need First Dim

This is because when calculating the _address offset_ of the element, the first dimemsion will never be used.