#include "matrix.hpp"
#include <iomanip>

#define tname template <typename T>

//  Constructors, destructors

tname
matrix<T>::matrix(size_t _rows_count, size_t _columns_count, std::function<double()> _det_algorithm) :
    rows_count(_rows_count),
    columns_count(_columns_count),
    det_algorithm(_det_algorithm)
{
    data = new T*[_rows_count * _columns_count];
}

tname
matrix<T>::matrix(size_t _rows_count, size_t _columns_count) : 
    matrix(_rows_count, _columns_count, default_det_algorithm) 
{}

tname
matrix<T>::matrix() : matrix<T>::matrix(0, 0) {}

tname
matrix<T>::matrix(size_t _size) : matrix<T>::matrix(_size, _size) {}

tname
matrix<T>::~matrix() 
{
    delete[] data;
}

tname 
matrix<T>::matrix(matrix const& that) :
            rows_count(that.rows_count),
            columns_count(that.columns_count),
            det_algorithm(that.det_algorithm) 
{
    data = new T*[that.rows_count * that.columns_count];
    for (int i = 0; i < that.rows_count; i++) {
        for (int j = 0; j < that.columns_count; j++) {
            data[i * that.columns_count + j] = that.data[i * that.columns_count + j];
        }
    }
}

//  Private member functions

tname bool
matrix<T>::check_size(matrix const& that) const
{
    if (that.rows_count != this->rows_count || 
        that.columns_count != this->columns_count) 
    {
        return false;
    }
    return true;
}

tname bool
matrix<T>::reverse_check_size(matrix const& that) const 
{
    if (that.rows_count != this->columns_count ||
        that.columns_count != this->rows_count) 
    {
        return false;
    }
    return true;
}

tname void
matrix<T>::transpose(matrix& that) const 
{
        for (int i = 0; i < rows_count; i++) {
            for (int j = 0; j < columns_count; j++) {
                that.data[i * columns_count + j] = data[i * columns_count + j];
            }
        }
}

tname void
matrix<T>::create_minor(matrix& future_minor, size_t row, size_t column) const 
{
    for (int i = 0, k = 0; k < rows_count - 1; i++, k++) {
        if (i == row) {
            i++;
        }
        for (int j = 0, l = 0; l < columns_count - 1; j++, l++) {
            if (j == column) {
                j++;
            }
            future_minor.data[k * columns_count + l] = data[i * columns_count + j];
        }
    }
}

//  Public member functions

tname void
swap(matrix<T>& a, matrix<T>& b) 
{
        using std::swap;
        swap(a.size, b.size);
        swap(a.data, b.data);
}

tname matrix<T>& 
matrix<T>::operator=(matrix that) 
{
    swap(*this, that);
    return *this;
}

#if __cplusplus >= 201703L
tname [[nodiscard]] constexpr bool 
matrix<T>::empty() const noexcept 
{
    return !rows_count || !columns_count; 
}
#else
tname bool 
matrix<T>::empty() const
{
    return !rows_count || !columns_count;
}
#endif

tname bool
matrix<T>::operator==(matrix const& that) const
{
    if (!check_size(that)) {
        return false;
    }
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            if (data[i * columns_count + j] != that.data[i * columns_count + j]) {
                return false;
            }
        }
    }
    return true;
}

// what about int -> double???

tname bool 
matrix<T>::operator!=(const matrix& that) const
{
    return !(*this == that);
}

tname double
matrix<T>::det()
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
    double det = det_algorithm();
    determinant = det; // to do smth with const matrix, mb calculate det() of const M while creating it
    return det;
}

tname double
matrix<T>::det() const 
{
    if (!is_square()) {
        throw std::runtime_error("det() is only for square matrices");
    }
    if (!det_is_calculated) {
        throw std::runtime_error("DEBUG: det() for const matrix must be calculated on construction");
    }
    return determinant;
}

tname matrix<T> 
matrix<T>::operator+(matrix const& that) const 
{
    if (!check_size(that)) {
        throw std::runtime_error("Sum of different size matrices");
    }
    matrix result(rows_count, columns_count); // rewrite
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            result.data[i * columns_count + j] = data[i * columns_count + j] +
                                                 that.data[i * columns_count + j];
        }
    }
    return result;
}

tname matrix<T>
matrix<T>::operator-(matrix const& that) const 
{
    if (!check_size(that)) {
        throw std::runtime_error("Substraction of different size matrices");
    }
    matrix result(rows_count, columns_count);
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            result.data[i * columns_count + j] = data[i * columns_count + j] -
                                                 that.data[i * columns_count + j];
        }
    }
    return result;
}

tname matrix<T>
matrix<T>::operator*(matrix const& that) const 
{
    if (!reverse_check_size(that)) {
        throw std::runtime_error("Multiplication cannot be done: not fitting sizes");
    }
    matrix result();
    for (int i = 0; i < rows_count; i++) {
        for (int j = 0; j < columns_count; j++) {
            for (int k = 0; k < size; k++) {            //? rework
                result.data[i * columns_count + j] += data[i * columns_count + k] * 
                                                    that.data[k * columns_count + j];
            }
        }
    }
    return result;
}

tname matrix<T>
matrix<T>::operator~() const 
{
    matrix<T> that(rows_count, columns_count);
    transpose(that);
    return that;
}

tname std::istream& 
operator>>(std::istream& is, matrix<T> const& that)
{
    for (int i = 0; i < that.rows_count; i++) {
        for (int j = 0; j < that.columns_count; j++) {
            is >> that.data[i * that.columns_count + j];
        }
    }
    return is;
}

tname std::ostream&
operator<<(std::ostream& os, matrix<T> const& that)
{
    for (int i = 0; i < that.rows_count; i++) {
        for (int j = 0; j < that.columns_count; j++) {
            os << that.data[i * that.columns_count + j] << ' ';
        }
        os << std::endl;
    }
    return os;
}

tname matrix<T>
matrix<T>::operator()(size_t row, size_t column) const 
{
    matrix<T> minor(rows_count - 1, columns_count - 1);
    create_minor(minor, row, column);
    return minor;
}

