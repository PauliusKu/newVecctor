#ifndef NEWVECTOR_HEADER_H
#define NEWVECTOR_HEADER_H

template<
	class T,
	class Alloc = std::allocator<T>
> class vector {
public:

	typedef T 																value_type;//+
	typedef Alloc 															allocator_type;//+
	typedef size_t															size_type; //+
	typedef std::ptrdiff_t 													difference_type; //+
	typedef value_type& 													reference; //+
	typedef const value_type& 												const_reference; //+
	typedef typename std::allocator_traits<Alloc>::pointer 					pointer; //+
	typedef typename std::allocator_traits<Alloc>::const_pointer 			const_pointer; //+
	typedef T*																iterator; //+
	typedef const T*														const_iterator; //+
	typedef std::reverse_iterator<iterator> 								reverse_iterator;//+
	typedef std::reverse_iterator<const_iterator> 							const_reverse_iterator;//+

private:
	//alloctors and stuff

public:
	//8 constructors

};

#endif