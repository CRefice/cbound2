#pragma once

#include <cassert>
#include <utility>

#include "../simd/access.hpp"
#include "../simd/types.hpp"

namespace ssm
{
// Forward declarations, useful since array operator has to be a member
template <typename T, std::size_t N>
struct generic_vec;
template <typename T, std::size_t N>
T* data_ptr(generic_vec<T, N>& vec);
template <typename T, std::size_t N>
const T* data_ptr(const generic_vec<T, N>& vec);

// The base for all vector types, has no operations associated with it
// other than general-use array access operators and constructors.
template <typename T, std::size_t N>
struct generic_vec : simd::vector_data<T, N>
{
	using value_type = T;
	// Initialize all elements to zero.
	generic_vec() = default;

	// Initialize all elments to val.
	explicit generic_vec(value_type val) { simd::fill(this->data, val); }

	// Initialize the vector all at once.
	generic_vec(simd::vector<T, N> vec) {
		this->data = vec;
	}

	// Initialize each element to one variadic argument,
	// in the given order.
	template <typename... Args>
	generic_vec(Args... args) { 
		static_assert(sizeof...(args) == N,
				"ssm::generic_vec: wrong number of arguments for variadic constructor");
		simd::assign(this->data, args...);
	}

	// Template accessor functions, useful for compile-time assignments.
	template <std::size_t I>
	value_type get() const { return simd::get_element<T, N, I>(this->data); }

	template <std::size_t I>
	void set(const value_type& val) { this->data = simd::set_element<T, N, I>(this->data, val); }

	T& operator[](std::size_t index) {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
	const T& operator[](std::size_t index) const {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
};

// Data-accessing functions
template <typename T, std::size_t N>
T* data_ptr(generic_vec<T, N>& vec) { return reinterpret_cast<T*>(&(vec.data)); }
template <typename T, std::size_t N>
T* begin(generic_vec<T, N>& vec) { return data_ptr(vec); }
template <typename T, std::size_t N>
T* end(generic_vec<T, N>& vec) { return data_ptr(vec) + N; }

template <typename T, std::size_t N>
const T* data_ptr(const generic_vec<T, N>& vec) {
	return reinterpret_cast<const T*>(&(vec.data));
}
template <typename T, std::size_t N>
const T* begin(const generic_vec<T, N>& vec) { return data_ptr(vec); }
template <typename T, std::size_t N>
const T* end(const generic_vec<T, N>& vec) { return data_ptr(vec) + N; }
}
