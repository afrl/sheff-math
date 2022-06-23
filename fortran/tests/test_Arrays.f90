    program test_Arrays
    ! Tests C++/Fortran array wrappers

    use iso_c_binding
    use simple_math

    real(C_DOUBLE), dimension(6) :: fixed_array
    real(C_DOUBLE), dimension(6,6) :: fixed_matrix
    real(C_DOUBLE), dimension(:), allocatable :: dynamic_array
    real(C_DOUBLE), dimension(:,:), allocatable :: dynamic_matrix
    type(SWIGTYPE_p_simple_math__DynamicVectorRefT_double_t) :: v_ref
    type(SWIGTYPE_p_simple_math__DynamicMatrixColMajorRefT_double_t) :: m_ref

    v_ref = as_vector(fixed_array)
    
    allocate(dynamic_array(4))
    v_ref = as_vector(dynamic_array)
    deallocate(dynamic_array)
    
    m_ref = as_matrix(fixed_matrix)
    
    allocate(dynamic_matrix(4, 4))
    m_ref = as_matrix(dynamic_matrix)
    deallocate(dynamic_matrix)

    end program test_Arrays