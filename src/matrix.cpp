#include "matrix.hpp"

#define tname template <typename T>

//  Constructors, destructors

tname
matrix<T>::matrix(size_t _rows_count, size_t _columns_count) : 
    rows_count(_rows_count),
    columns_count(_columns_count)
{
    data = new T*[_rows_count];
    for (size_t i = 0; i < _rows_count; i++) {
        data[i] = new T[_columns_count];
    }
}

tname
matrix<T>::matrix() : matrix<T>::matrix(0, 0) {}

tname
matrix<T>::matrix(size_t _size) : matrix<T>::matrix(_size, _size) {}

tname
matrix<T>::~matrix() 
{
    for (int i = 0; i < rows_count; i++) {
        delete[] data[i];
    }
    delete[] data;
}

tname 
matrix<T>::matrix(matrix const& that) :
            rows_count(that.rows_count),
            columns_count(that.columns_count),
            data(new int*[size]) 
{
    for (int i = 0; i < size; i++) {
        data[i] = new int[that.size];
        for (int j = 0; j < size; j++) {
            data[i][j] = that.data[i][j];
        }
    }
}

//  Private member functions

tname void
matrix<T>::transpose(matrix& that) const 
{
        for (int i = 0; i < rows_count; i++) {
            for (int j = 0; j < columns_count; j++) {
                that.data[i][j] = data[j][i];
            }
        }
}

tname void
matrix<T>::create_minor(matrix& future_minor, size_t row, size_t column) const 
{
    for (int i = 0, k = 0; k < size - 1; i++, k++) {
        if (i == row) {
            i++;
        }
        for (int j = 0, l = 0; l < size - 1; j++, l++) {
            if (j == column) {
                j++;
            }
            future_minor.data[k][l] = data[i][j];
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
matrix<T>::operator==(const matrix& that) const //  to remade
{
    /* if (size != that.size) {
        return false;
    } */
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (data[i][j] != that.data[i][j]) {
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

#if __cplusplus >= 201703L
tname constexpr int32_t 
matrix<T>::det() const 
{

}
#elif __cplusplus >= 201103L
tname int32_t 
matrix<T>::det() const 
{

}
#endif

tname matrix<T> 
matrix<T>::operator+(matrix const& that) const 
{
    //check_size(that.size);
    matrix result(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result.data[i][j] = data[i][j] + that.data[i][j];
        }
    }
    return result;
}

tname matrix<T>
matrix<T>::operator-(matrix const& that) const 
{
    //check_size(that.size);
    matrix result(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result.data[i][j] = data[i][j] - that.data[i][j];
        }
    }
    return result;
}

tname matrix<T>
matrix<T>::operator*(const matrix& that) const 
{
    //check_size(that.size);
    matrix result(size);
    for (int i = 0; i < size; i++) {
        result.data[i][i] = 0;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                result.data[i][j] += data[i][k] * that.data[k][j];
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
    for (int i = 0; i < that.size; i++) {
        for (int j = 0; j < that.size; j++) {
            is >> that.data[i][j];
        }
    }
    return is;
}

tname std::ostream&
operator<<(std::ostream& os, matrix<T> const& that)
{
    for (int i = 0; i < that.size; i++) {
        for (int j = 0; j < that.size; j++) {
            os << that.data[i][j] << ' ';
        }
        os << std::endl;
    }
    return os;
}

tname matrix<T>
matrix<T>::operator()(int row, int column) const 
{
    matrix<T> minor(size - 1);
    create_minor(minor, row, column);
    return minor;
}

