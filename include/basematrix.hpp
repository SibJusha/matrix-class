#include <cassert>
#include <iostream>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class _Base_matrix {
public:

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef value_type const& const_reference;

private:

    allocator_type  m_alloc;
    value_type*     m_data;         //use std::alloc_traits::pointer?
    size_type       m_size;
    size_type       m_capacity;

public:
    
    _Base_matrix(allocator_type const& alloc = allocator_type()) : 
        m_alloc(alloc), m_data(nullptr), m_size(0), m_capacity(0)
    {}
    
    _Base_matrix(size_type const& _elem_count, const_reference value = value_type(), 
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
        
    _Base_matrix(size_type const& n, allocator_type const& alloc = allocator_type()) :
        _Base_matrix(n, alloc)
    {}

    ~_Base_matrix()
    {
    #if __cplusplus >= 201703L
        std::destroy_n(m_data, m_capacity);
    #else
        m_alloc.destroy(m_data);
    #endif
        m_alloc.deallocate(m_data, m_capacity);
    }

};
