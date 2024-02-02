#include <cassert>
#include <iostream>
#include <memory>
#include <functional>

template <typename T, typename Allocator = std::allocator<T>>
struct _Base_Arr_matrix {

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;

protected:

    allocator_type  m_alloc;
    value_type*     m_data;         //use std::alloc_traits::pointer?
    size_type       m_size;
    size_type       m_capacity;

protected:
    
    _Base_Arr_matrix(allocator_type const& alloc = allocator_type()) : 
        m_alloc(alloc), m_data(nullptr), m_size(0), m_capacity(0)
    {}
    
    _Base_Arr_matrix(size_type const& _elem_count, const_reference value = value_type(), 
        allocator_type const& alloc = allocator_type()) :
            m_alloc(alloc), m_data(nullptr)
    {
        assert(_elem_count > 0);

        m_size = m_capacity = _elem_count;
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

        std::cout << "Matrix is created\n";
    }

    _Base_Arr_matrix(const _Base_Arr_matrix& _that) 
    {
        *this = _that;
    }

/*
    _Base_Arr_matrix(_Base_Arr_matrix&& _other, const allocator_type& _a = allocator_type()) :  
        _Base_Arr_matrix(_a)
    {
        if (_other.get_allocator() == _a) {
            
        }
    }
*/

    ~_Base_Arr_matrix()
    {
    #if __cplusplus >= 201703L
        std::destroy_n(m_data, m_capacity);
    #else
        m_alloc.destroy(m_data);
    #endif
        m_alloc.deallocate(m_data, m_capacity);
    }

public:

    allocator_type 
    get_allocator() const 
    {
        return m_alloc;
    }

};

template <typename T, typename Allocator = std::allocator<T> >
class matrix_no_det : public _Base_Arr_matrix<T, Allocator> {
public:

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;

protected:

    size_type   rows_count;
    size_type   columns_count;

public:

    virtual matrix_no_det(const allocator_type& alloc = allocator_type()) :
        _Base_Arr_matrix<T, Allocator>(alloc), rows_count(0), columns_count(0)
    {}

    virtual matrix_no_det(const size_type& _rows_count, const size_type& _columns_count, 
        const_reference value = value_type(), const allocator_type& alloc = allocator_type()) :
            _Base_Arr_matrix<T, Allocator>(_rows_count * _columns_count, value, alloc), 
            rows_count(_rows_count), columns_count(_columns_count)
    {}

    virtual matrix_no_det(const size_type& count, const_reference value = value_type(), 
        const allocator_type& alloc = allocator_type()) :
            matrix_no_det(count, count, value, alloc)
    {}

    virtual matrix_no_det(matrix_no_det&& other, const allocator_type& alloc = allocator_type()) :
        _Base_Arr_matrix<T, Allocator>(std::move(other), alloc),
        rows_count(other.rows_count), columns_count(other.columns_count)
    {} 

    virtual matrix_no_det(matrix_no_det const& that) 
    {
        *this = that;
    }

	template<typename U, typename Alloc>
    friend void swap(matrix_no_det<U, Alloc>& lhs, matrix_no_det<U, Alloc>& rhs)
    {
        using std::swap;
        swap(lhs.rows_count, rhs.rows_count);
        swap(lhs.columns_count, rhs.columns_count);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_alloc, rhs.m_alloc);
        swap(lhs.m_capacity, rhs.m_capacity);
    }

    matrix_no_det& operator=(matrix_no_det const& that)
    {
        //swap(*this, that); // deprecated
        if (this == &that) 
            return *this;

        if (!check_size(that)) 
        {
            value_type* temp_data = this->m_alloc.allocate(that.rows_count * that.columns_count);

        #if __cplusplus >= 202002L
            temp_data = std::construct_at(temp_data);
            
        #elif __cplusplus >= 201103L
            this->m_alloc.construct(temp_data);    
        #else
            this->m_alloc.construct(temp_data, value_type());
        #endif

        #if __cplusplus >= 201703L
            std::destroy_n(this->m_data, this->m_capacity);
        #else
            this->m_alloc.destroy(this->m_data);
        #endif

            this->m_alloc.deallocate(this->m_data, this->m_capacity);
            this->m_data = temp_data;
        }

        std::copy(that.m_data, that.m_data + that.m_size, this->m_data);
        rows_count = that.rows_count;
        columns_count = that.columns_count;
        this->m_capacity = that.rows_count * that.columns_count;

        return *this;
    }
//  Capacity && getters

    #if __cplusplus >= 201703L
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return !rows_count || !columns_count; 
    }  
    #else
    bool empty() const
    {
        return !rows_count || !columns_count; 
    }
    #endif

    #if __cplusplus >= 201103L
    constexpr size_type size() const noexcept
    {
        return this->m_size;
    }    
    #else 
    size_type size() const
    {
        return this->m_size;
    }
    #endif

    #if __cplusplus >= 201703L
    constexpr size_type capacity() const noexcept
    {
        return this->m_capacity;
    }
    #else
    size_type capacity() const
    {
        return this->m_capacity;
    }
    #endif

//  Modifiers

// Compare operators (members)

    bool operator==(matrix_no_det const& that) const
    {
        if (!check_size(that)) {
            return false;
        }
        for (size_type i = 0; i < rows_count; i++) {
            for (size_type j = 0; j < columns_count; j++) {
                if (this->m_data[i * columns_count + j] != that.m_data[i * columns_count + j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(matrix_no_det const& that) const
    {
        return !(*this == that);
    }

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

    matrix_no_det operator+(matrix_no_det const& that) const
    {
        if (!check_size(that)) 
            throw std::runtime_error("Sum of different size matrices");

        matrix_no_det result(rows_count, columns_count); // rewrite
        for (size_type i = 0; i < rows_count; i++) {
            for (size_type j = 0; j < columns_count; j++) {
                result.m_data[i * columns_count + j] = this->m_data[i * columns_count + j] +
                                                    that.m_data[i * columns_count + j];
            }
        }
        return result;
    }

    matrix_no_det operator-(matrix_no_det const& that) const
    {
        if (!check_size(that)) 
            throw std::runtime_error("Substraction of different size matrices");
    
        matrix_no_det result(rows_count, columns_count);
        for (size_type i = 0; i < rows_count; i++) {
            for (size_type j = 0; j < columns_count; j++) {
                result.m_data[i * columns_count + j] = this->m_data[i * columns_count + j] -
                                                    that.m_data[i * columns_count + j];
            }
        }
        return result;
    }

    matrix_no_det operator*(matrix_no_det const& that) const
    {
        if (!reverse_check_size(that)) {
            throw std::runtime_error("Multiplication cannot be done: not fitting sizes");
        }
        matrix_no_det result;
        for (size_type i = 0; i < rows_count; i++) {
            for (size_type j = 0; j < columns_count; j++) {
                for (size_type k = 0; k < columns_count; k++) {            //?    rework done, check is needed
                    result.m_data[i * columns_count + j] += this->m_data[i * columns_count + k] * 
                                                        that.m_data[k * columns_count + j];
                }
            }
        }
        return result;
    }
    
    /*  Transpose this matrix_no_det */
    matrix_no_det operator~() const
    {
        matrix_no_det<T, Allocator> that(rows_count, columns_count);
        transpose(that);
        return that;
    }
    
    template<typename U, typename Alloc>
    friend std::istream& operator>>(std::istream& is, matrix_no_det<U, Alloc>& that)
    {
        for (std::size_t i = 0; i < that.rows_count; i++) {
            for (std::size_t j = 0; j < that.columns_count; j++) {
                is >> that.m_data[i * that.columns_count + j];
            }
        }
        return is;
    }

    template<typename U, typename Alloc>
    friend std::ostream& operator<<(std::ostream& os, matrix_no_det<U, Alloc> const& that)
    {
        for (std::size_t i = 0; i < that.rows_count; i++) {
            for (std::size_t j = 0; j < that.columns_count; j++) {
                os << that.m_data[i * that.columns_count + j] << ' ';
            }
            os << std::endl;
        }
        return os;
    }
    
    /*  Get the non-const reference to the element(row, column) from non-const matrix_no_det */
    reference operator() (size_type const& row, size_type const& column)
    {
        if (row >= 0 && row < rows_count && column >= 0 && column < columns_count) {
            return this->m_data[row * columns_count + column];  
        }
        else {
            throw std::runtime_error("Cannot return element which is out of bounds");
        }
    }

    /*  Get a copy of the element(row, column) from const matrix_no_det */
    const value_type operator() (size_type const& row, size_type const& column) const
    {
        if (row >= 0 && row < rows_count && column >= 0 && column < columns_count) {
            const value_type element = this->m_data[row * columns_count + column];
            return element;
        }
        else {
            throw std::runtime_error("Cannot return element which is out of bounds");
        }
    }
    
    /*  Returns column as T* type.
    *   It is highly preferable to wrap it in smart pointer */
    value_type* operator() (size_type const& column)
    {
        value_type* __col = new value_type[rows_count];
        for (size_type i = 0; i < rows_count; i++) 
            __col[i] = this->m_data[i * columns_count + column];
        return __col;
    }
    
    /*  Returns row as T* type.
    *   It is highly preferable to wrap it in smart pointer */
    value_type* operator[] (size_type const& row)
    {
        value_type* __row = new value_type[columns_count];
        for (size_type i = 0; i < columns_count; i++)
            __row[i] = this->m_data[i * rows_count + row];
        return __row;
    }

    matrix_no_det<T, Allocator> 
    get_minor(size_type const& row_to_delete, size_type const& column_to_delete) const
    {
        matrix_no_det future_minor(rows_count - 1, columns_count - 1);
        for (size_type i = 0, k = 0; k < rows_count - 1; i++, k++) {
            if (i == row_to_delete) {
                i++;
            }
            for (size_type j = 0, l = 0; l < columns_count - 1; j++, l++) {
                if (j == column_to_delete) {
                    j++;
                }
                future_minor.m_data[k * columns_count + l] = this->m_data[i * columns_count + j];
            }
        }
        return future_minor;
    }


protected:

    bool check_size(matrix_no_det const& that) const
    {
        return (that.rows_count != this->rows_count || 
            that.columns_count != this->columns_count);
    }
    
    bool reverse_check_size(matrix_no_det const& that) const
    {
        return (that.rows_count != this->columns_count ||
            that.columns_count != this->rows_count);
    }
    
    void transpose(matrix_no_det& that) const
    {
        for (int i = 0; i < rows_count - 1; i++) {
            for (int j = i + 1; j < columns_count; j++) {
                std::swap(that.m_data[i * columns_count + j], this->m_data[i * columns_count + j]);
            }
        }
    }

};


/*  Wrapper over determinant calculating algorithm to access private m_data of matrix<T>;
    Class is needed, because any random { double func() } doesn't have rights to access private m_data 
    without an usage of getters.
*/
template <typename Det>
class determinant_algorithm;

template <typename T, typename Det = double, typename Allocator = std::allocator<T> >
class matrix : public matrix_no_det<T, Allocator> {
public:
    
    friend class determinant_algorithm<Det>;
    typedef T value_type;
    typedef Det det_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef determinant_algorithm _d_alg;

private:

    mutable det_type    determinant;
    mutable bool        det_is_calculated = false;
    _d_alg<det_type>    m_det_algorithm;

public:

    matrix(const allocator_type& alloc = allocator_type(), const _d_alg& det_alg = _d_alg()) :
        matrix_no_det(alloc), rows_count(0), columns_count(0), m_det_algorithm(det_alg)
    {}

    matrix(const size_type& _rows_count, const size_type& _columns_count, const_reference value = value_type(), 
        const allocator_type& alloc = allocator_type(), const _d_alg& det_alg = _d_alg()) :
            matrix_no_det(_rows_count, _columns_count, value, alloc), 
            rows_count(_rows_count), columns_count(_columns_count), m_det_algorithm(det_alg)
    {}

    matrix(const size_type& count, const_reference value = value_type(), const allocator_type& alloc = allocator_type(),
        const _d_alg& det_alg = _d_alg()) :
            matrix_no_det(count, value, alloc), m_det_algorithm(det_alg)
    {}

    matrix(matrix&& other, const allocator_type& alloc = allocator_type(), const _d_alg& det_alg = _d_alg()) :
        matrix_no_det(std::move(other), alloc), m_det_algorithm(det_alg)
    {} 

    matrix(matrix const& that) 
    {
        *this = that;
    }

    det_type
    det() const
    {
        if (!is_square()) {
            throw std::runtime_error("det() is only for square matrices");
        }
        if (!m_det_algorithm) {
            throw std::runtime_error("det() algorithm is not set for matrix");
        }
        if (det_is_calculated) { 
            return determinant;
        }
        determinant = m_det_algorithm._calculate_det(*this);
        det_is_calculated = true;
        return determinant;
    }

};

template <typename T>
class determinant_algorithm {
    std::function<T (matrix<T>&)> _det_function;

    static double __default_det_alg(matrix<T> Matrix);  

public:
    determinant_algorithm() : determinant_algorithm(__default_det_alg());

    determinant_algorithm(std::function<T (matrix<T>&)> det_calculating_alg) 
    {
        _det_function = std::bind(det_calculating_alg, std::placeholders::_1);
    }

    T _calculate_det(matrix<T>& Matrix)
    {
        return _det_function(Matrix);
    }
};