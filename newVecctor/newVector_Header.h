#ifndef NEWVECTOR_HEADER_H
#define NEWVECTOR_HEADER_H

#include <initializer_list>
#include <algorithm>
#include <iterator>
#include <exception>
#include <memory>
#include <cstddef>
#include <utility>

template<
	class T,
	class Alloc = std::allocator<T>
> class newvector {
public:

	typedef T 																value_type;//+
	typedef Alloc 															allocator_type;//+
	typedef size_t															size_type; //+
	typedef std::ptrdiff_t 													difference_type; //+
	typedef value_type& 													reference; //+
	typedef const value_type& 												const_reference; //+
	typedef typename std::allocator_traits<allocator_type>::pointer 		pointer;
	typedef typename std::allocator_traits<allocator_type>::const_pointer 	const_pointer;
	typedef T*																iterator; //+
	typedef const T*														const_iterator; //+
	typedef std::reverse_iterator<iterator> 								reverse_iterator;//+
	typedef std::reverse_iterator<const_iterator> 							const_reverse_iterator;//+

	//8 constructors

	explicit newvector(const allocator_type& alloc = allocator_type()) :
		allocator_(alloc),
		capacity_(0),
		size_(0),
		array_(nullptr)
	{
		rearrange_pointers();
		std::cout << "c-tor 1" << std::endl;
	};

	newvector(size_type n, value_type val, const allocator_type& alloc = allocator_type()) :
		allocator_(alloc),
		capacity_(n),
		size_(n),
		array_(allocator_.allocate(n))
	{
		rearrange_pointers();
		construct_elements(array_, n, val);
		std::cout << "c-tor 2" << std::endl;
	};

	explicit newvector(size_type n) :
		allocator_(),
		capacity_(n),
		size_(n),
		array_(allocator_.allocate(n))
	{
		rearrange_pointers();
		std::cout << "c-tor 3" << std::endl;
	};

	newvector(pointer first, pointer last, const allocator_type& alloc = allocator_type()) :
		allocator_(alloc),
		capacity_(static_cast<size_type>(last - first)),
		size_(static_cast<size_type>(last - first)),
		array_(allocator_.allocate(static_cast<size_type>(last - first)))
	{
		rearrange_pointers();
		construct_elements(first, last, array_start_);
		std::cout << "c-tor 4" << std::endl;
	};

	newvector(const newvector& x) :
		allocator_(),
		capacity_(x.capacity()),
		size_(x.size()),
		array_(allocator_.allocate(x.capacity()))
	{
		rearrange_pointers();
		construct_elements(x.begin(), x.end(), array_start_);
		std::cout << "c-tor 5.0" << std::endl;
	};

	newvector(const newvector& x, const allocator_type& alloc) :
		allocator_(alloc),
		capacity_(x.capacity()),
		size_(x.size()),
		array_(allocator_.allocate(x.capacity()))
	{
		rearrange_pointers();
		construct_elements(x.begin(), x.end(), array_start_);
		std::cout << "c-tor 5.1" << std::endl;
	};

	newvector(newvector&& x) :
		allocator_(),
		capacity_(x.capacity_),
		size_(x.size_),
		array_(x.array_)
	{
		rearrange_pointers();
		x.array_ = nullptr;
		x.size_ = 0;
		x.capacity_ = 0;
		x.rearrange_pointers();
		std::cout << "c-tor 6" << std::endl;
	};

	newvector(newvector&& x, const allocator_type& alloc) :
		allocator_(alloc),
		capacity_(x.capacity_),
		size_(x.size_),
		array_(x.array_)
	{
		rearrange_pointers();
		x.array_ = nullptr;
		x.size_ = 0;
		x.capacity_ = 0;
		x.rearrange_pointers();
		std::cout << "c-tor 7" << std::endl;
	};

	newvector(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type()) :
		allocator_(alloc),
		capacity_(il.size()),
		size_(il.size()),
		array_(allocator_.allocate(il.size()))
	{
		rearrange_pointers();
		construct_elements(il.begin(), il.end(), array_start_);
		std::cout << "c-tor 8" << std::endl;
	};

	~newvector() {
		destroy_elements(array_, size_);
		allocator_.deallocate(array_, capacity_);
		array_start_ = nullptr;
		array_end_ = nullptr;
		array_range_end_ = nullptr;
	}

	// Element access

	reference operator[](size_type n) { return *(array_start_ + n); }

	const_reference operator[](size_type n) const { return *(array_start_ + n); }

	reference at(size_type n) {
		if (n > 0 && n < capacity_)
			return *(array_start_ + n);
		else throw std::out_of_range("out of vector range.");
	}

	const_reference at(size_type n) const {
		if (n > 0 && n < capacity_)
			return *(array_start_ + n);
		else throw std::out_of_range("out of vector range.");
	}

	inline reference front() {
		return *(array_start_);
	}

	inline const_reference front() const {
		return *(array_start_);
	}

	inline reference back() {
		return *(array_end_);
	}

	inline const_reference back() const {
		return *(array_end_);
	}

	inline pointer data() const { return array_; }

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
		size_++;
		rearrange_pointers();
	}




	void pop_back() {
		allocator_.destroy(array_end_);
		size_--;
		rearrange_pointers();
	}

	private:
		allocator_type allocator_;
		size_type capacity_;
		size_type size_;
		pointer array_;
		pointer array_start_;
		pointer array_end_;
		pointer array_range_end_;

		void rearrange_pointers() {
			array_start_ = array_;
			array_end_ = array_ + size_;
			array_range_end_ = array_ + capacity_;
		}

		void increase_array(size_type new_size) {
			pointer new_array = allocator_.allocate(new_size);
			construct_elements(begin(), std::min(end(), begin() + new_size), new_array);

			destroy_elements(array_, size_);
			allocator_.deallocate(array_, capacity_);
			array_ = new_array;
			capacity_ = new_size;
			rearrange_pointers();
		}

		void construct_elements(pointer begin, pointer end, pointer destination) {
			// std::cout << "constructor 1" << std::endl;
			const difference_type distance = end - begin;
			for (unsigned int i = 0; i < distance; ++i) {
				allocator_.construct(destination + i, *(begin));
				begin++;
			}
		}

		void construct_elements(pointer destination, size_type count, value_type value) {
			// std::cout << "constructor 2" << std::endl;
			for (size_type i = 0; i < count; ++i)
				allocator_.construct(destination + i, value);
		}

		void destroy_elements(pointer start, size_type n) {
			for (size_type i = 0; i < n; ++i) {
				allocator_.destroy(start + i);

			}
		}
};

#endif