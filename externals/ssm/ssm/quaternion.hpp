#pragma once

#include "detail/setup.hpp"
#include "detail/vector-manip.hpp"

namespace ssm
{
template <typename T>
struct unit_quaternion : generic_vec<T, 4>
{
	using generic_vec<T, 4>::generic_vec;
};

template <typename T>
struct quaternion : generic_vec<T, 4>
{
	quaternion<T>() = default;
	quaternion<T>(const unit_quaternion<T>& u) : generic_vec<T, 4>(u.data) {}
	using generic_vec<T, 4>::generic_vec;
};

template <template <class> class A,
				 template <class> class B,
				 typename T>
inline T dot(const A<T>& a, const B<T>& b) {
	return detail::vec_impl<T, 4>::dot(a, b);
}

template <template <class> class Quat, typename T>
inline T sqnorm(const Quat<T>& quat) {
	return dot(quat, quat);
}

template <template <class> class Quat, typename T>
inline T norm(const Quat<T>& quat) {
	return static_cast<T>(std::sqrt(sqnorm(quat)));
}

template <template <class> class Quat, typename T>
inline Quat<T> conjugate(Quat<T> quat) {
	detail::vec_impl<T, 4>::quat_conjugate(quat);
	return quat;
}

template <typename T>
inline quaternion<T> inverse(quaternion<T> quat) {
	detail::vec_impl<T, 4>::quat_conjugate(quat);
	return quat / sqnorm(quat);
}

// Specific form for unit quaternions, since it's much simpler
template <typename T>
inline unit_quaternion<T> inverse(unit_quaternion<T> quat) {
	return conjugate(quat);
}

template <template <class> class Quat, typename T>
inline unit_quaternion<T> normalize(const Quat<T>& quat) {
	unit_quaternion<T> norm(quat.data);
	detail::vec_impl<T, 4>::normalize(norm);
	return norm;
}

template <template <class> class Quat,
				 typename T>
inline quaternion<T>& operator+=(quaternion<T>& a, const Quat<T>& b) {
	detail::vec_impl<T, 4>::add(a, b);
	return a;
}

template <template <class> class Quat,
				 typename T>
inline quaternion<T>& operator-=(quaternion<T>& a, const Quat<T>& b) {
	detail::vec_impl<T, 4>::sub(a, b);
	return a;
}

template <template <class> class Quat,
				 typename T>
inline quaternion<T>& operator*=(quaternion<T>& a, const Quat<T>& b) {
	detail::vec_impl<T, 4>::quat_mul(a, b);
	return a;
}

template <typename T>
inline quaternion<T>& operator*=(quaternion<T>& a, T b) {
	detail::vec_impl<T, 4>::mul(a, b);
	return a;
}

template <typename T>
inline quaternion<T>& operator/=(quaternion<T>& a, T b) {
	detail::vec_impl<T, 4>::div(a, b);
	return a;
}

template <template <class> class A,
				 template <class> class B,
				 typename T>
inline quaternion<T> operator+(const A<T>& a, const B<T>& b) {
	quaternion<T> ret(a);
	return ret += b;
}

template <template <class> class A,
				 template <class> class B,
				 typename T>
inline quaternion<T> operator-(const A<T>& a, const B<T>& b) {
	quaternion<T> ret(a);
	return ret -= b;
}

template <template <class> class A,
				 template <class> class B,
				 typename T>
inline quaternion<T> operator*(const A<T>& a, const B<T>& b) {
	quaternion<T> ret(a);
	return ret *= b;
}

template <template <class> class A,
	typename T>
inline quaternion<T> operator*(const A<T>& a, T b) {
	quaternion<T> ret(a);
	return ret *= b;
}

template <template <class> class A,
	typename T>
inline quaternion<T> operator*(T a, const A<T>& b) {
	return b * a;
}

template <template <class> class A,
	typename T>
inline quaternion<T> operator/(const A<T>& a, T b) {
	quaternion<T> ret(a);
	return ret /= b;
}

template <template <class> class Quat, typename T>
inline Quat<T> operator-(Quat<T> quat) {
	detail::vec_impl<T, 4>::negate(quat);
	return quat;
}

template <template <class> class A,
				 template <class> class B,
				 typename T>
inline bool operator==(const A<T>& a, const B<T>& b) {
	return detail::vec_impl<T, 4>::equals(a, b);
}

template <template <class> class A,
	template <class> class B,
	typename T>
	inline bool operator!=(const A<T>& a, const B<T>& b) {
	return !(a == b);
}

//----------------------------------------------
// Utility typedefs
//----------------------------------------------
using quat = quaternion<float>;
using dquat = quaternion<double>;

using unit_quat = unit_quaternion<float>;
using unit_dquat = unit_quaternion<double>;
}
