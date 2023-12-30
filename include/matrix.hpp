#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <algorithm>
#include <stdexcept>
#include <functional>

/*
class vector {
    bool            is_row; //row = true, column = false
    size_t          line_number;
    matrix&  matrix;

    friend class    matrix;

public:

    vector(bool row_or_column, int n, matrix& matrix) :
        is_row(row_or_column),
        line_number(n),
        matrix(matrix)
    {}

    ~vector() {}

    vector(vector const& that) :
        is_row(that.is_row),
        line_number(that.line_number),
        matrix(that.matrix)
    {}

    vector(vector&& that) noexcept :
        is_row(std::move(that.is_row)),
        line_number()
    {}

    vector& operator=(vector& that) {

    }

    friend void swap(vector& lhs, vector& rhs) noexcept {
        std::swap()
    }

}
*/

template<typename T>
class matrix {

    size_t                  rows_count;
    size_t                  columns_count;
    size_t                  r_reserved;
    size_t                  c_reserved;
    T*                      data;
    double                  determinant;
    bool                    det_is_calculated   = false;
    std::function<double()> det_algorithm       = nullptr;
    //friend class  vector; vector is N x 1 matrix

    double default_det_algorithm() const;

    /* WIP Possible algorithm for very large matrices*/
    double det_Strassen_algorithm() const;

    bool check_size(matrix const& that) const;

    bool reverse_check_size(matrix const& that) const;

    void transpose(matrix& that) const;

    void create_minor(matrix& future_minor, size_t row, size_t column) const;

public:

    // Make compile-time constructor for static matrix

    matrix(size_t _rows_count, size_t _columns_count);

    matrix(size_t _rows_count, size_t _columns_count, std::function<double()> _det_algorithm);

    matrix();

/*  Maybe for square matrices 
    matrix(int n, const int * a):
            size(n),
            data(new int*[n]) {

        for (int i = 0; i < n; i++) {
            data[i] = new int[n]();
            data[i][i] = a[i];
        }
    }
 */

    matrix(size_t n);

    ~matrix();

    matrix(matrix const& that);

    friend void swap(matrix& a, matrix& b);

    matrix& operator=(matrix that);

//  Capacity 

    #if __cplusplus >= 201703L
    [[nodiscard]] constexpr bool empty() const noexcept;  
    #else
    bool empty() const;
    #endif

    #if __cplusplus >= 201103L
    constexpr std::size_t size() const noexcept;    
    #else 
    std::size_t size() const;
    #endif

    #if __cplusplus >= 201703L
    constexpr std::size_t capacity() const noexcept;
    #else
    std::size_t capacity() const;
    #endif

//  Modifiers

// Compare operators (members)

    bool operator==(matrix const& that) const;

    bool operator!=(matrix const& that) const; //constexpr?

    bool is_reversible() const;

    #if __cplusplus >= 201703L
    constexpr bool is_square() const {
        return rows_count == columns_count;
    }
    #else
    bool is_square() const {
        return rows_count == columns_count;
    }
    #endif

//  Operators (?)

    double det();

    double det() const;

    matrix operator+(matrix const& that) const;

    matrix operator- (matrix const& that) const;

    matrix operator*(matrix const& that) const;

    matrix operator~() const;

    friend std::istream& operator>> (std::istream& is, matrix const& that);

    friend std::ostream& operator<< (std::ostream& os, matrix const& that);

    matrix operator() (size_t row, size_t column) const;
    
    vector operator() (size_t column);

    vector operator[] (size_t row);

};
/*
class vector {

    public:


        int& operator[] (int i) const {
            if (i >= matrix.size) {
                std::cout << "i >= size of matrix";
                exit(-1);
            }
            if (is_row) {
                return matrix.data[line_number][i];
            } else {
                return matrix.data[i][line_number];
            }
        }
};

vector matrix::operator() (int column) {
    if (column >= size) {
        std::cout << "i >= size of matrix";
        exit(-1);
    }
    return vector(false, column, *this);
}

vector matrix::operator[] (int row) {
    if (row >= size) {
        std::cout << "i >= size of matrix";
        exit(-1);
    }
    return vector(true, row, *this);
}*/
#endif // __MATRIX_H__