#pragma once

#include <cstddef>
#include <functional>
#include <iostream>

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

template< typename T, typename Allocator = std::allocator<T> >
class matrix {
public:

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;

private:

    allocator_type              m_alloc;
    value_type*                 m_data;         //use std::alloc_traits::pointer?
    size_type                   rows_count;
    size_type                   columns_count;
    size_type                   m_size;
    size_type                   m_capacity;     // fill matrix free bytes
    mutable double              determinant;
    mutable bool                det_is_calculated   = false;
    std::function<double()>     det_algorithm       = nullptr;

    static double default_det_algorithm();
    /* WIP Possible algorithm for very large matrices*/
    //static double det_Strassen_algorithm();
    bool check_size(matrix const& that) const;
    bool reverse_check_size(matrix const& that) const;
    void transpose(matrix& that) const;
    void create_minor(matrix& future_minor, size_type row, size_type column) const;
    //void _m_deallocate(value_type* p, size_type n); 

public:

    // Make compile-time constructor for static matrix

    matrix(allocator_type const& alloc = allocator_type(), std::function<double()> _det_algorithm = default_det_algorithm);
    matrix(size_type const& n, allocator_type const& alloc = allocator_type(), 
                                                        std::function<double()> _det_algorithm = default_det_algorithm);
    matrix(size_type const& _rows_count, size_type const& _columns_count, const_reference value = value_type(),
        allocator_type const& alloc = allocator_type() , std::function<double()>  _det_algorithm = default_det_algorithm);
    /* Constructor for square diagonal matrix */
    matrix(size_type const& used_length, const value_type* array, std::function<double()> _det_algorithm = default_det_algorithm);
    ~matrix();

    matrix(matrix const& that);

	template<typename U, typename Alloc>
    friend void swap(matrix<U, Alloc>& lhs, matrix<U, Alloc>& rhs);

    matrix& operator=(matrix const& that);
    
//  Capacity && getters

    #if __cplusplus >= 201703L
    [[nodiscard]] constexpr bool empty() const noexcept;  
    #else
    bool empty() const;
    #endif

    #if __cplusplus >= 201103L
    constexpr size_type size() const noexcept;    
    #else 
    size_type size() const;
    #endif

    #if __cplusplus >= 201703L
    constexpr size_type capacity() const noexcept;
    #else
    size_type capacity() const;
    #endif

    allocator_type get_allocator() const;

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

    double det() const;
    matrix operator+(matrix const& that) const;
    matrix operator- (matrix const& that) const;
    matrix operator*(matrix const& that) const;
    /*  Transpose this matrix */
    matrix operator~() const;
    
    template<typename U, typename Alloc>
    friend std::istream& operator>> (std::istream& is, matrix<U, Alloc>& that);

    template<typename U, typename Alloc>
    friend std::ostream& operator<< (std::ostream& os, matrix<U, Alloc> const& that);
    
    /*  Get a copy of the element(row, column) from const matrix */
    const value_type operator() (size_type const& row, size_type const& column) const;
    /*  Get the non-const reference to the element(row, column) from non-const matrix */
    reference operator() (size_type const& row, size_type const& column);
    /*  Returns column as T* type.
    *   It is highly preferable to wrap it in smart pointer */
    value_type* operator() (size_type const& column);
    /*  Returns row as T* type.
    *   It is highly preferable to wrap it in smart pointer */
    value_type* operator[] (size_type const& row);
    matrix<T, Allocator> get_minor (size_type const& row_to_delete, size_type const& column_to_delete) const;
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
