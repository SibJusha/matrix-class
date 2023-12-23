#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <algorithm>
#include <stdexcept>

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

    size_t          rows_count;
    size_t          columns_count;
    size_t          r_reserved;
    size_t          c_reserved;
    T**             data;
    //friend class  vector; vector is N x 1 matrix

/*  Maybe make two funcs?

    void check_size(const size_t& that_size) const {
        if (size != that_size) {
            throw std::length_error("Matrices are of different sizes");
            exit(-1);
        }
    }
 */
    void transpose(matrix& that) const;

    void create_minor(matrix& future_minor, size_t row, size_t column) const;

public:

    matrix(size_t _rows_count, size_t _columns_count);

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

    bool operator==(const matrix& that) const;

    bool operator!=(const matrix& that) const; //constexpr?

//  Operators (?)

    #if __cplusplus >= 201703L
    constexpr int32_t det() const;
    #elif __cplusplus >= 201103L
    int32_t det() const;
    #endif

    matrix operator+(matrix const& that) const;

    matrix operator- (const matrix& that) const;

    matrix operator*(const matrix& that) const;

    matrix operator~() const;

    friend std::istream& operator>> (std::istream& is, matrix const& that);

    friend std::ostream& operator<< (std::ostream& os, matrix const& that);

    matrix operator() (int row, int column) const;
    
    vector operator() (int column);

    vector operator[] (int row);

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