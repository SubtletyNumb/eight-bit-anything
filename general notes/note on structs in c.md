User

if i have an array of strutcts in c like this>
struct Car c1 = { 2 };
struct Car c2 = { 3 };
struct Car cars[] = { c1, c2};

if i then update the c1 or c2 here
the cars inside the cars array are not the same anymore?


**You are correct: the cars inside the array are NOT the same anymore.**

In C, when you initialize an array (or assign variables), the data is **copied**.

### What actually happens in memory:

1.  **`struct Car c1 = { 2 };`**
    *   C allocates memory for `c1` at specific address (e.g., 0x100) and writes `2` into it.
2.  **`struct Car cars[] = { c1, c2 };`**
    *   C allocates **new** memory for the array.
    *   It takes the **value** inside `c1` and **copies** it into `cars[0]`.
    *   `cars[0]` is now a completely separate distinct variable from `c1`.

### Code Proof

```c
#include <stdio.h>

struct Car {
    int id;
};

int main() {
    struct Car c1 = { 2 };
    struct Car c2 = { 3 };
    
    // This creates COPIES of c1 and c2 inside the array
    struct Car cars[] = { c1, c2 };

    // Update the original c1
    c1.id = 99;

    printf("Original c1.id: %d\n", c1.id);
    printf("Array cars[0].id: %d\n", cars[0].id);

    return 0;
}
```

**Output:**
```text
Original c1.id: 99
Array cars[0].id: 2
```

---

### How to make them stay "linked"

If you want the array to update automatically when you change `c1`, you must use an **array of pointers** instead of an array of structs. This stores the *address* of `c1` rather than a copy of its data.

```c
struct Car c1 = { 2 };
struct Car c2 = { 3 };

// Array of pointers (store the addresses)
struct Car *cars[] = { &c1, &c2 }; 

// Update c1
c1.id = 99;

// Access via the pointer in the array (using arrow syntax)
// This will print 99
printf("%d", cars[0]->id); 
```