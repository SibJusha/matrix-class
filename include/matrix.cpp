#include "matrix.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

#define tname template <typename T, typename Allocator>

//  struct det_algorithm

template <typename T>
det_algorithm<T>::det_algorithm(std::function<T (matrix<T>)> det_calculating_alg) :
    __det_function(det_calculating_alg)
{}

template <typename T>   
T 
det_algorithm<T>::__calculate_det(matrix<T> Matrix)
{
    return __det_function(Matrix);
}

//  Constructors, destructors

// dummy function
tname double 
matrix<T, Allocator>::default_det_algorithm() {
    return 666;
}

tname
matrix<T, Allocator>::matrix(allocator_type const& alloc, std::function<double()> _det_algorithm) : 
    m_alloc(alloc), m_data(nullptr), rows_count(0), columns_count(0), m_capacity(0), m_size(0)
{
    det_algorithm = std::bind(_det_algorithm);
}

tname
matrix<T, Allocator>::matrix(size_type const& _rows_count, size_type const& _columns_count, const_reference value, 
        const allocator_type& alloc , std::function<double()> _det_algorithm) :
    m_alloc(alloc), m_data(nullptr), rows_count(_rows_count), columns_count(_columns_count)
{
    assert(rows_count > 0 && columns_count > 0);

    m_size = m_capacity = rows_count * columns_count;
    m_data = m_alloc.allocate(m_capacity);

#if __cplusplus >= 202002L
    m_data = std::construct_at(m_data);
#elif __cplusplus >= 201103L
    m_alloc.construct(m_data);    
#else
    m_alloc.construct(m_data);
#endif
	for (size_type i = 0; i < m_capacity; ++i)  
        m_data[i] = std::move(value);

    det_algorithm = std::bind(_det_algorithm);
    std::cout << "Matrix is created\n";
}

tname
matrix<T, Allocator>::matrix(size_type const& _size, const Allocator& alloc, std::function<double()> _det_algorithm) : 
    matrix<T>::matrix(_size, _size, alloc, _det_algorithm) {}

tname
matrix<T, Allocator>::~matrix() 
{
#if __cplusplus >= 201703L
    std::destroy_n(m_data, m_capacity);
#else
    m_alloc.destroy(m_data);
#endif
    m_alloc.deallocate(m_data, m_capacity);
}

tname 
matrix<T, Allocator>::matrix(matrix const& that) 
{
    *this = that;
}

//  Private member functions

tname bool
matrix<T, Allocator>::check_size(matrix const& that) const
{
    if (that.rows_count != this->rows_count || 
        that.columns_count != this->columns_count) 
    {
        return false;
    }
    return true;
}

tname bool
matrix<T, Allocator>::reverse_check_size(matrix const& that) const 
{
    if (that.rows_count != this->columns_count ||
        that.columns_count != this->rows_count) 
    {
        return false;
    }
    return true;
}

tname void
matrix<T, Allocator>::transpose(matrix& that) const 
{
        for (int i = 0; i < rows_count; i++) {
            for (int j = 0; j < columns_count; j++) {
                that.m_data[i * columns_count + j] = m_data[i * columns_count + j];
            }
        }
}

tname void
matrix<T, Allocator>::create_minor(matrix& future_minor, size_type row, size_type column) const 
{
    for (int i = 0, k = 0; k < rows_count - 1; i++, k++) {
        if (i == row) {
            i++;
        }
        for (int j = 0, l = 0; l < columns_count - 1; j++, l++) {
            if (j == column) {
                j++;
            }
            future_minor.m_data[k * columns_count + l] = m_data[i * columns_count + j];
        }
    }
}

//  Public member functions

template<typename U, typename Alloc> void
swap(matrix<U, Alloc>& lhs, matrix<U, Alloc>& rhs) 
{
	using std::swap;
    swap(lhs.rows_count, rhs.rows_count);
    swap(lhs.columns_count, rhs.columns_count);
    swap(lhs.r_reserved, rhs.r_reserved);
    swap(lhs.c_reserved, rhs.c_reserved);
    swap(lhs.det_is_calculated, rhs.det_is_calculated);
    swap(lhs.determinant, rhs.determinant);
    swap(lhs.det_algorithm, rhs.det_algorithm);
   	swap(lhs.m_data, rhs.m_data);
    swap(lhs.m_alloc, rhs.m_alloc);
}

tname matrix<T, Allocator>& 
matrix<T, Allocator>::operator=(matrix const& that) 
{
    //swap(*this, that); // deprecated
    if (this == &that) 
        return *this;

    if (!check_size(that)) 
    {
        value_type* temp_data = m_alloc.allocate(that.rows_count * that.columns_count);
#if __cplusplus >= 202002L
        temp_data = std::construct_at(temp_data);
        
#elif __cplusplus >= 201103L
        m_alloc.construct(temp_data);    
#else
        m_alloc.construct(temp_data, value_type());
#endif
#if __cplusplus >= 201703L
        std::destroy_n(m_data, m_capacity);
#else
        m_alloc.destroy(m_data);
#endif
        m_alloc.deallocate(m_data, m_capacity);
        m_data = temp_data;
    }

    std::copy(that.m_data, that.m_data + that.m_size, m_data);
    rows_count = that.rows_count;
    columns_count = that.columns_count;
    m_capacity = that.rows_count * that.columns_count;
    determinant = that.determinant;
    det_is_calculated = that.det_is_calculated;
    det_algorithm = that.det_algorithm; 

    return *this;
}

#if __cplusplus >= 201703L
tname [[nodiscard]] constexpr bool 
matrix<T, Allocator>::empty() const noexcept 
{
    return !rows_count || !columns_count; 
}
#else
tname bool 
matrix<T, Allocator>::empty() const
{
    return !rows_count || !columns_count;
}
#endif

tname bool
matrix<T, Allocator>::operator==(matrix const& that) const
{
    if (!check_size(that)) {
        return false;
    }
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            if (m_data[i * columns_count + j] != that.m_data[i * columns_count + j]) {
                return false;
            }
        }
    }
    return true;
}

// what about int -> double???

tname bool 
matrix<T, Allocator>::operator!=(const matrix& that) const
{
    return !(*this == that);
}

 tname double
matrix<T, Allocator>::det() const
{
    if (!is_square()) {
        throw std::runtime_error("det() is only for square matrices");
    }
    if (!det_algorithm) {
        throw std::runtime_error("det() algorithm is not set for matrix");
    }
    if (det_is_calculated) { 
        return determinant;
    }
    determinant = det_algorithm();
    det_is_calculated = true;
    return determinant;
}

tname matrix<T, Allocator> 
matrix<T, Allocator>::operator+(matrix const& that) const 
{
    if (!check_size(that)) {
        throw std::runtime_error("Sum of different size matrices");
    }
    matrix result(rows_count, columns_count); // rewrite
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            result.m_data[i * columns_count + j] = m_data[i * columns_count + j] +
                                                 that.m_data[i * columns_count + j];
        }
    }
    return result;
}

tname matrix<T, Allocator>
matrix<T, Allocator>::operator-(matrix const& that) const 
{
    if (!check_size(that)) {
        throw std::runtime_error("Substraction of different size matrices");
    }
    matrix result(rows_count, columns_count);
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            result.m_data[i * columns_count + j] = m_data[i * columns_count + j] -
                                                 that.m_data[i * columns_count + j];
        }
    }
    return result;
}

tname matrix<T, Allocator>
matrix<T, Allocator>::operator*(matrix const& that) const 
{
    if (!reverse_check_size(that)) {
        throw std::runtime_error("Multiplication cannot be done: not fitting sizes");
    }
    matrix result;
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            for (int k = 0; k < columns_count; k++) {            //?    rework done, check is needed
                result.m_data[i * columns_count + j] += m_data[i * columns_count + k] * 
                                                    that.m_data[k * columns_count + j];
            }
        }
    }
    return result;
}

tname matrix<T, Allocator>
matrix<T, Allocator>::operator~() const 
{
    matrix<T, Allocator> that(rows_count, columns_count);
    transpose(that);
    return that;
}

template<typename U, typename Alloc> std::istream& 
operator>>(std::istream& is, matrix<U, Alloc>& that)
{
    for (int i = 0; i < that.rows_count; i++) {
        for (int j = 0; j < that.columns_count; j++) {
            is >> that.m_data[i * that.columns_count + j];
        }
    }
    return is;
}

template<typename U, typename Alloc> std::ostream&
operator<<(std::ostream& os, matrix<U, Alloc> const& that)
{
    for (int i = 0; i < that.rows_count; i++) {
        for (int j = 0; j < that.columns_count; j++) {
            os << that.m_data[i * that.columns_count + j] << ' ';
        }
        os << std::endl;
    }
    return os;
}
 
#if 0
int main() {
    matrix<int> A(2, 2);
    std::string data = "1 1 2 5";
    std::istringstream sstream(data);
	sstream >> A;
    sstream.str("2 2 3 8");
    return 0;
} 
#endif
/*
tname const T
matrix<T, Allocator>::operator()(size_type row, size_type column) const
{
    if (row >= 0 && row < rows_count && column >= 0 && column < columns_count) {
        const T element = m_data[row * columns_count + column];
        return element;
    }
    else {
        throw std::runtime_error("Cannot return element which is out of bounds");
    }
}

tname T&
matrix<T, Allocator>::operator()(size_type row, size_type column)
{
    if (row >= 0 && row < rows_count && column >= 0 && column < columns_count) {
        return m_data[row * columns_count + column];  
    }
    else {
        throw std::runtime_error("Cannot return element which is out of bounds");
    }
}

tname matrix<T, Allocator>
matrix<T, Allocator>::get_minor(size_type row_to_delete, size_type column_to_delete) const 
{
    matrix future_minor(rows_count - 1, columns_count - 1);
    for (int i = 0, k = 0; k < rows_count - 1; i++, k++) {
        if (i == row_to_delete) {
            i++;
        }
        for (int j = 0, l = 0; l < columns_count - 1; j++, l++) {
            if (j == column_to_delete) {
                j++;
            }
            future_minor.m_data[k * columns_count + l] = m_data[i * columns_count + j];
        }
    }
    return future_minor;
}


tname value_type*
matrix<T, Allocator>::operator()(size_type column) 
{
    if (column >= 0 && column < columns_count) {
        T* result_col = new T[rows_count];
        for (int i = 0; i < rows_count; i++) {
            result_col[i] = m_data[i * columns_count + column];
        }
        return result_col;
    }
    else {
        throw std::runtime_error("Cannot return column which is out of bounds");
    }
}

tname T*
matrix<T, Allocator>::operator[](size_type row) 
{
    if (row >= 0 && row < rows_count) {
        return &m_data[row * columns_count];
    }
    else {
        throw std::runtime_error("Cannot return row which is out of bounds");
    }
}*/
