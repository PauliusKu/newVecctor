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

	// CONSTRUCTORS 9

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

	//desructor

	~newvector() {
		destroy_elements(array_, size_);
		allocator_.deallocate(array_, capacity_);
		array_start_ = nullptr;
		array_end_ = nullptr;
		array_range_end_ = nullptr;
		std::cout << "d - tor" << std::endl;
	}

	// operator =

	newvector& operator=(const newvector& x)
	{
		if (capacity_ >= x.size_) {
			size_ = x.size();
		}
		else {
			destroy_elements(array_, size_);
			allocator_.deallocate(array_, capacity_);
			capacity_ = x.size();
			size_ = x.size();
			array_ = allocator_.allocate(x.size());
		}
		rearrange_pointers();
		construct_elements(x.begin(), x.end(), array_start_);
		std::cout << "operator= 1" << std::endl;
		return *this;
	}

	newvector& operator=(newvector&& x)
	{
		destroy_elements(array_, capacity_);
		allocator_.deallocate(array_, capacity_);
		size_ = x.size_;
		capacity_ = x.capacity_;
		array_ = x.array_;
		rearrange_pointers();
		x.size_ = 0;
		x.capacity_ = 0;
		x.array_ = nullptr;
		x.rearrange_pointers();
		std::cout << "operator= 2" << std::endl;
		return *this;
	}

	newvector& operator=(std::initializer_list<value_type> il)
	{
		if (capacity_ >= il.size()) {
			size_ = il.size();
		}
		else {
			destroy_elements(array_, size_);
			allocator_.deallocate(array_, capacity_);
			capacity_ = il.size();
			size_ = il.size();
			array_ = allocator_.allocate(il.size());
		}
		rearrange_pointers();
		construct_elements(il.begin(), il.end(), array_start_);
		std::cout << "operator= 3" << std::endl;
		return *this;
	}

	// assign

	void assign(size_type n, const_reference val) {
		if (n > capacity_) { increase_array(n); }
		construct_elements(array_, n, val);
		size_ = n;
		rearrange_pointers();
		std::cout << "assign 1" << std::endl;
	}

	void assign(pointer first, pointer last) {
		if (last - first > capacity_) { increase_array(last - first); }
		construct_elements(first, last, array_start_);
		size_ = last - first;
		rearrange_pointers();
		std::cout << "assign 2" << std::endl;
	}

	void assign(std::initializer_list<value_type> il) {
		if (il.size() <= capacity_) {
		}
		else {
			increase_array(il.size());
		}
		construct_elements(il.begin(), il.end(), begin());
		size_ = il.size();
		rearrange_pointers();
		std::cout << "assign 3" << std::endl;
	}

	// get_allocator

	inline allocator_type get_allocator() { return allocator_; }

	// ELEMENT ACCESS

	// at

	reference at(size_type n)
	{
		if (n > 0 && n < capacity_){ return *(array_start_ + n); }
		else throw std::out_of_range("out of vector range.");
	}

	const_reference at(size_type n) const
	{
		if (n > 0 && n < capacity_){ return *(array_start_ + n); }
		else throw std::out_of_range("out of vector range.");
	}

	// operator []

	reference operator[](size_type n) { return *(array_start_ + n); }

	const_reference operator[](size_type n) const { return *(array_start_ + n); }

	// front

	inline reference front() { return *(array_start_); }

	inline const_reference front() const { return *(array_start_); }

	// back

	inline reference back() { return *(array_end_); }

	inline const_reference back() const { return *(array_end_); }

	// data

	inline pointer data() const { return array_; }

	// ITERATORS

	inline iterator begin() { return iterator(array_start_); }

	inline const_iterator begin() const { return const_iterator(array_start_); }

	inline const_iterator cbegin() const { return const_iterator(array_start_); }

	inline iterator end() { return iterator(array_end_); }

	inline const_iterator end() const { return const_iterator(array_end_); }

	inline const_iterator cend() const { return const_iterator(array_end_); }

	inline reverse_iterator rbegin() { return reverse_iterator(end()); }

	inline const_reverse_iterator rbegin() const { return reverse_iterator(cend()); }

	inline const_reverse_iterator crbegin() const { return reverse_iterator(cend()); }

	inline reverse_iterator rend() { return reverse_iterator(begin()); }

	inline const_reverse_iterator rend() const { return reverse_iterator(cbegin()); }

	inline const_reverse_iterator crend() const { return reverse_iterator(cbegin()); }

	//CAPACITY

	inline bool empty() const { return size_ == 0; }

	inline size_type size() const { return size_; }

	inline size_type max_size() const { return size_type(-1); }

	void reserve(size_type n)
	{
		if (n > capacity_){ increase_array(n); }
	}

	inline size_type capacity() const { return capacity_; }

	void shrink_to_fit() { increase_array(size_); }

	//MODIFIERS

	// clear 1

	void clear()
	{
		destroy_elements(array_, size_);
		size_ = 0;
		rearrange_pointers();
	}

	// insert 5

	iterator insert(const_iterator position, const value_type& val)
	{
		const difference_type distance = position - cbegin();
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		size_++;
		iterator it = begin() + distance;
		construct_elements_backward(it, end(), array_start_ + distance + 1);
		allocator_.construct(array_start_ + distance, val);
		rearrange_pointers();
		return it;
	}

	iterator insert(const_iterator position, value_type&& val)
	{
		const difference_type distance = position - cbegin();
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		size_++;
		iterator it = begin() + distance;
		construct_elements_backward(it, end(), array_start_ + distance + 1);
		allocator_.construct(array_start_ + distance, val);
		rearrange_pointers();
		return it;
	}

	iterator insert(const_iterator position, size_type count, const value_type& val)
	{
		const difference_type distance = position - cbegin();
		if (size_ + count > capacity_)
		{
			increase_array(std::max(static_cast<int>(capacity_ * 2), static_cast<int>(size_ + count)));
		}
		size_ += count;
		iterator it = begin() + distance;
		construct_elements_backward(it, end(), array_start_ + distance + count);
		construct_elements(it, count, val);
		rearrange_pointers();
		return it;
	}

	iterator insert(const_iterator position, const_iterator first, const_iterator last)
	{
		const difference_type distance = position - cbegin();
		const difference_type count = last - first;
		if (size_ + count > capacity_)
		{
			increase_array(std::max(static_cast<int>(capacity_ * 2), static_cast<int>(size_ + count)));
		}
		size_ += count;
		iterator it = begin() + distance;
		construct_elements_backward(it, end(), array_start_ + distance + count);
		construct_elements(first, last, array_start_ + distance);
		rearrange_pointers();
		return it;
	}

	iterator insert(const_iterator position, std::initializer_list<value_type> il)
	{
		const difference_type distance = position - cbegin();
		const difference_type count = il.end() - il.begin();
		if (size_ + count > capacity_)
		{
			increase_array(std::max(static_cast<int>(capacity_ * 2), static_cast<int>(size_ + count)));
		}
		size_ += count;
		iterator it = begin() + distance;
		construct_elements_backward(it, end(), array_start_ + distance + count);
		construct_elements(il.begin(), il.end(), array_start_ + distance);
		rearrange_pointers();
		return it;
	}

	// emplace 1

	template <class... Args>
	iterator emplace(const_iterator position, Args&&... args)
	{
		difference_type distance = position - cbegin();
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		construct_elements_backward(begin() + distance, end(), array_start_ + distance + 1);
		allocator_.construct(array_start_ + distance, std::forward<Args>(args)...);
		size_++;
		rearrange_pointers();
		return begin() + distance;
	}

	// erase 2

	iterator erase(const_iterator position)
	{
		const difference_type distance = position - cbegin();
		iterator it = begin() + distance;
		construct_elements(it + 1, end(), array_start_ + distance);
		if (size_ != 0){ size_--; }
		rearrange_pointers();
		return it;

	}

	iterator erase(const_iterator first, const_iterator last)
	{
		const difference_type distance = first - cbegin();
		const difference_type distance_first_last = last - cbegin();
		iterator it_first = begin() + distance;
		iterator it_last = begin() + distance_first_last;
		construct_elements(it_last, end(), array_start_ + distance);
		size_ -= last - first;
		rearrange_pointers();
		return it_first;

	}

	// push_back 2

	void push_back(const value_type& val)
	{
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		allocator_.construct(array_end_, val);
		size_++;
		rearrange_pointers();
	}

	void push_back(value_type&& val)
	{
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		allocator_.construct(array_end_, val);
		size_++;
		rearrange_pointers();
	}

	// emplace_back 1

	template <class... Args>
	void emplace_back(Args&&... args)
	{
		if (array_end_ == array_range_end_)
		{
			increase_array(std::max(1, static_cast<int>(capacity_ * 2)));
		}
		allocator_.construct(array_end_, std::forward<Args>(args)...);
		size_++;
		rearrange_pointers();
	}

	// pop_back 1

	void pop_back()
	{
		allocator_.destroy(array_end_);
		size_--;
		rearrange_pointers();
	}

	// resize 2

	void resize(size_type n)
	{
		if (n <= size_)
		{
			destroy_elements(array_start_ + n, size_ - n);
		}
		else increase_array(std::max(n, 2 * capacity_));
		size_ = n;
	}

	void resize(size_type n, const value_type& val)
	{
		if (n <= size_)
		{
			destroy_elements(array_start_ + n, size_ - n);
		}
		else
		{
			increase_array(std::max(n, 2 * capacity_));
			construct_elements(end(), array_range_end_ - array_end_, val);
			rearrange_pointers();
		}
		size_ = n;
	}

	// swap 1

	void swap(newvector& x)
	{
		std::swap(capacity_, x.capacity_);
		std::swap(size_, x.size_);
		std::swap(array_, x.array_);
		rearrange_pointers();
		x.rearrange_pointers();
	}

	private:
		allocator_type allocator_;
		size_type capacity_;
		size_type size_;
		pointer array_;
		pointer array_start_;
		pointer array_end_;
		pointer array_range_end_;

		void rearrange_pointers()
		{
			array_start_ = array_;
			array_end_ = array_ + size_;
			array_range_end_ = array_ + capacity_;
		}

		void increase_array(size_type new_size)
		{
			pointer new_array = allocator_.allocate(new_size);
			construct_elements(begin(), std::min(end(), begin() + new_size), new_array);

			destroy_elements(array_, size_);
			allocator_.deallocate(array_, capacity_);
			array_ = new_array;
			capacity_ = new_size;
			rearrange_pointers();
		}
		void construct_elements(const_iterator begin, const_iterator end, const_iterator destination)
		{
			const difference_type distance = end - begin;
			std::cout << "c_elem 1 " << std::endl;
			for (difference_type i = 0; i < distance; ++i)
			{
				allocator_.construct(destination +i, *(begin));
				begin++;
			}
		}

		void construct_elements_backward(const_iterator begin, const_iterator end, const_iterator destination)
		{

			const difference_type distance = end - begin;
			std::cout << "c_elem 1 bckw " << std::endl;
			for (difference_type i = distance - 1; i >= 0; --i)
			{
				allocator_.construct(destination + i, *(end - 1));
				end--;
			}
		}

		void construct_elements(pointer destination, size_type count, value_type value)
		{
			std::cout << "c_elem 2 " << std::endl;
			for (size_type i = 0; i < count; ++i)
			{
				allocator_.construct(destination + i, value);
			}
		}

		void destroy_elements(pointer start, size_type n)
		{
			for (size_type i = 0; i < n; ++i)
			{
				allocator_.destroy(start + i);
			}
		}
};

#endif