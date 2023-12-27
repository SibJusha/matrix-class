# Matrix Class

The Matrix Class is a C++ template class that provides functionality for working with matrices. It allows you to create, manipulate, and perform operations on matrices of different sizes and types.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Public Member Functions](#public-member-functions)
  - [Constructors](#constructors)
  - [Capacity](#capacity)
  - [Modifiers](#modifiers)
  - [Comparison Operators](#comparison-operators)
  - [Other Operators](#other-operators)
  - [Input and Output Operators](#input-and-output-operators)

## Installation

To use the Matrix Class in your C++ project, you need to include the `matrix.h` header file in your source code.

## Usage

To create a matrix object, you can use one of the available constructors. For example:

```cpp
#include "matrix.h"

// Create a 3x3 matrix of integers
matrix<int> m(3, 3);
```

Once you have a matrix object, you can perform various operations on it, such as adding, subtracting, multiplying, and transposing matrices. Here's an example:
```cpp
matrix<int> a(2, 2);
a(0, 0) = 1;
a(0, 1) = 2;
a(1, 0) = 3;
a(1, 1) = 4;

matrix<int> b(2, 2);
b(0, 0) = 5;
b(0, 1) = 6;
b(1, 0) = 7;
b(1, 1) = 8;

matrix<int> c = a + b; // Matrix addition
matrix<int> d = a * b; // Matrix multiplication
matrix<int> e = ~a;    // Matrix transpose
```

For a complete list of available member functions and operators, refer to the Public Member Functions section below.

## Public Member Functions

### Constructors

- `matrix(size_t _rows_count, size_t _columns_count)`: Constructs a matrix with the specified number of rows and columns.
- `matrix()`: Default constructor.
- `matrix(size_t n)`: Constructs a square matrix with the specified size.

### Capacity

- `bool empty() const`: Checks if the matrix is empty.
- `size_t size() const`: Returns the number of elements in the matrix.
- `size_t capacity() const`: Returns the capacity of the matrix.

### Modifiers


### Comparison operators

- `bool operator==(matrix const& that) const`: Checks if two matrices are equal.
- `bool operator!=(matrix const& that) const`: Checks if two matrices are not equal.