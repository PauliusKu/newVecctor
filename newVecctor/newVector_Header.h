#ifndef NEWVECTOR_HEADER_H
#define NEWVECTOR_HEADER_H

#include <initializer_list>
#include <algorithm>
#include <iterator>
//#include <exception>
//#include <memory>
//#include <cstddef>
//#include <utility>

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
	/**
	* Allocator associated to the vector.
	*/
	allocator_type allocator_;

	/**
	* Capacity of the vector.
	*/
	size_type capacity_;
	/**
	* Size of the vector.
	*/
	size_type size_;
	/**
	* Pointer to the array.
	*/
	pointer array_;

	/**
	* Range pointer to the start of the array.
	*/
	pointer array_start_;
	/**
	* Range pointer to the end of size of the array.
	*/
	pointer array_end_;
	/**
	* Range pointer to the end of capacity of the array.
	*/
	pointer array_range_end_;

	/**
	* @brief      Reassigns range pointers to their correct values.
	*/

	void rearrange_pointers() {
		array_start_ = array_;
		array_end_ = array_ + size_;
		array_range_end_ = array_ + capacity_;
	}

	/**
	* @brief      Creates a pointer to a new array. Allocates new_size worth of memory and constructs the elements from the old array.
	* The old array elements are destroyed and deallocated.
	*
	* @param[in]  new_size  new array size
	*/
	void increase_array(size_type new_size) {
		pointer new_array = allocator_.allocate(new_size);
		construct_elements(begin(), std::min(end(), begin() + new_size), new_array);

		destroy_elements(array_, size_);
		allocator_.deallocate(array_, capacity_);
		array_ = new_array;
		capacity_ = new_size;
		rearrange_pointers();
	}

	/**
	* @brief      Constructs elements from range [begin, end) to a pointer.
	*
	* @param[in]  begin          Iterator to begin of range
	* @param[in]  end            Iterator to end of range
	* @param[in]  destination    Pointer to destination
	*
	* @tparam     InputIterator  Class template type for iterator
	* @tparam     <unnamed>      { description }
	*/
	template <class InputIterator, typename = std::_RequireInputIter<InputIterator>>
	void construct_elements(InputIterator begin, InputIterator end, pointer destination) {
		// std::cout << "cons 1" << std::endl;
		const difference_type distance = end - begin;
		for (unsigned int i = 0; i < distance; ++i) {
			allocator_.construct(destination + i, *(begin));
			begin++;
		}
	}

	/**
	* @brief      Constructs count number of elements with value_type value to a pointer.
	*
	* @param[in]  destination  Pointer to destination
	* @param[in]  count        Number of elements
	* @param[in]  value        Value
	*/
	void construct_elements(pointer destination, size_type count, value_type value) {
		// std::cout << "cons 2" << std::endl;
		for (size_type i = 0; i < count; ++i)
			allocator_.construct(destination + i, value);
	}

	/**
	* @brief      Destroys the first n elements of array start.
	*
	* @param[in]  start  Pointer to the start of range
	* @param[in]  n      Number of elements
	*/
	void destroy_elements(pointer start, size_type n) {
		for (int i = 0; i < n; ++i) {
			allocator_.destroy(start + i);
		}
	}
public:
	//8 constructors






	// Iterators

	inline iterator begin() { return iterator(array_start_); }

	inline iterator end() { return iterator(array_end_); }

	inline const_iterator begin() const { return const_iterator(array_start_); }

	inline const_iterator end() const { return const_iterator(array_end_); }

	inline const_iterator cbegin() const { return const_iterator(array_start_); }

	inline const_iterator cend() const { return const_iterator(array_end_); }

	inline reverse_iterator rbegin() { return reverse_iterator(end()); }

	inline reverse_iterator rend() { return reverse_iterator(begin()); }

	inline const_reverse_iterator rbegin() const { return reverse_iterator(cend()); }

	inline const_reverse_iterator rend() const { return reverse_iterator(cbegin()); }

	inline const_reverse_iterator crbegin() const { return reverse_iterator(cend()); }

	inline const_reverse_iterator crend() const { return reverse_iterator(cbegin()); }


	//Modifiers

	void push_back(const value_type& val) {
		if (array_end_ == array_range_end_) {
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		allocator_.construct(array_end_, val);
		size_++;
		rearrange_pointers();
	}

	void push_back(value_type&& val) {
		if (array_end_ == array_range_end_) {
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		allocator_.construct(array_end_, val);
		rearrange_pointers();
		size_++;
	}




	void pop_back() {
		allocator_.destroy(array_end_);
		size_--;
		rearrange_pointers();
	}


};

#endif