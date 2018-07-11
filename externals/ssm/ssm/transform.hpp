#pragma once

#include "matrix.hpp"
#include "quaternion.hpp"

// Functions for transformations in any number of dimensions.

namespace ssm
{
// Returns an N+1-dimensional vector, with its last component set to 1.
template <typename T, std::size_t N>
inline vector<T, N + 1> homogenize(const vector<T, N>& vec) {
	return detail::unroll<0, N>::template homogenize_vec<vector<T, N+1>, vector<T, N>>(vec);
}

// Returns an N+1-dimensional vector, with its last component set to 1.
template <typename T, std::size_t N>
inline vector<T, N-1> dehomogenize(const vector<T, N>& vec) {
	return detail::unroll<0, N-1>::template dehomogenize_vec<vector<T, N>, vector<T, N-1>>(vec);
}

// Returns an N-dimensional identity matrix.
template <typename T, std::size_t N>
inline matrix<T, N, N> identity() {
	matrix<T, N, N> ret;
	detail::unroll<0, N>::identity_mat(ret);
	return ret;
}

// Note that translation requires a homogenous matrix to be represented as one,
// hence the N+1-sized matrix.
template <typename T, std::size_t N>
inline matrix<T, N + 1, N + 1> translation(const vector<T, N>& pos) {
	auto ret = identity<T, N + 1>();
	ret[N] = homogenize(pos);
	return ret;
}

template <typename T, std::size_t N>
inline matrix<T, N, N> scaling(const vector<T, N>& s) {
	matrix<T, N, N> ret;
	detail::unroll<0, N>::scaling_mat(ret, s);
	return ret;
}

namespace detail
{
template <typename T, typename = void>
struct transform_impl
{
static inline matrix<T, 4, 4> rotation(const unit_quaternion<T>& rot) {
	matrix<T, 4, 4> ret;

	ret[0].x = 1 - 2 * rot.y * rot.y - 2 * rot.z * rot.z;
	ret[0].y = 2 * rot.x * rot.y + 2 * rot.w * rot.z;
	ret[0].z = 2 * rot.x * rot.z - 2 * rot.w * rot.y;

	ret[1].x = 2 * rot.x * rot.y - 2 * rot.w * rot.z;
	ret[1].y = 1 - 2 * rot.x * rot.x - 2 * rot.z * rot.z;
	ret[1].z = 2 * rot.y * rot.z - 2 * rot.w * rot.x;

	ret[2].x = 2 * rot.x * rot.z + 2 * rot.w * rot.y;
	ret[2].y = 2 * rot.y * rot.z + 2 * rot.w * rot.x;
	ret[2].z = 1 - 2 * rot.x * rot.x - 2 * rot.y * rot.y;

	return ret;
}
};

template <typename T>
struct transform_impl<T, enable_if_t<simd::is_simd<T, 4>::value, void>>
{
static inline matrix<T, 4, 4> rotation(const unit_quaternion<T>& rot) {
	const simd::vector<T, 4> wwww = simd::shuffle<3, 3, 3, 3>(rot.data);
	const simd::vector<T, 4> xyzw = rot.data;
	const simd::vector<T, 4> zxyw = simd::shuffle<2, 0, 1, 3>(rot.data);
	const simd::vector<T, 4> yzxw = simd::shuffle<1, 2, 0, 3>(rot.data);

	const simd::vector<T, 4> xyzw2 = simd::add(xyzw, xyzw);
	const simd::vector<T, 4> zxyw2 = simd::shuffle<2, 0, 1, 3>(xyzw2);
	const simd::vector<T, 4> yzxw2 = simd::shuffle<1, 2, 0, 3>(xyzw2);

	simd::vector<T, 4> wide1;
	simd::fill(wide1, T(1));

	simd::vector<T, 4> tmp0 = simd::sub(wide1, simd::mul(yzxw2, yzxw));
	tmp0 = simd::sub(tmp0, simd::mul(zxyw2, zxyw));

	simd::vector<T, 4> tmp1 = simd::mul(yzxw2, xyzw);
	tmp1 = simd::add(tmp1, simd::mul(zxyw2, wwww));

	simd::vector<T, 4> tmp2 = simd::mul(zxyw2, xyzw);
	tmp2 = simd::sub(tmp2, simd::mul(yzxw2, wwww));

	matrix<T, 4, 4> ret = identity<T, 4>();
	ret[0].data = simd::assign(simd::get_element<T, 4, 0>(tmp0),
			simd::get_element<T, 4, 0>(tmp1), simd::get_element<T, 4, 0>(tmp2), T(0));
	ret[1].data = simd::assign(simd::get_element<T, 4, 1>(tmp2),
			simd::get_element<T, 4, 1>(tmp0), simd::get_element<T, 4, 1>(tmp1), T(0));
	ret[2].data = simd::assign(simd::get_element<T, 4, 2>(tmp1),
			simd::get_element<T, 4, 2>(tmp2), simd::get_element<T, 4, 2>(tmp0), T(0));
	return ret;
}
};
}

template <typename T>
inline matrix<T, 4, 4> rotation(const unit_quaternion<T>& rot) {
	return detail::transform_impl<T>::rotation(rot);
}

template <typename T>
inline matrix<T, 4, 4> perspective(T fovy, T aspect, T znear, T zfar) {
	const T halftan = std::tan(fovy / T(2));

	matrix<T, 4, 4> ret;
	ret[0] = vec4(T(1) / aspect * halftan, T(0), T(0), T(0));
	ret[1] = vec4(T(0), T(1) / halftan, T(0), T(0));
	ret[2] = vec4(T(0), T(0), -(zfar + znear) / (zfar - znear), T(0), T(-1));
	ret[3] = vec4(T(0), T(0), - 2 * (zfar * znear) / (zfar - znear), T(0));
	return ret;
}

template <typename T>
inline matrix<T, 4, 4> look_at(const vector<T, 3>& eye, const vector<T, 3>& target, const unit_vector<T, 3>& up) {
	const auto z = normalize(eye - target);
	const auto x = normalize(cross(up, z));
	const auto y = cross(z, x);

	matrix<T, 4, 4> ret;
	ret[0] = vec4(x.x, y.x, z.x, T(0));
	ret[1] = vec4(x.y, y.y, z.y, T(0));
	ret[2] = vec4(x.z, y.z, z.z, T(0));
	ret[3] = vec4(-dot(x, eye), -dot(y,eye), -dot(z, eye), T(1));
	return ret;
}
}
