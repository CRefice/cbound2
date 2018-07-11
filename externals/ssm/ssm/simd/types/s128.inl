#if SSM_ARCH & SSM_ARCH_SSE2
namespace ssm
{
template<>
struct is_simd<int, 4> { static constexpr bool value = true; };

namespace simd
{
typedef __m128i s128;

template <>
struct make_vector<int, 4>
{
	using type = s128;
};



inline s128 set_vector(int a, int b, int c, int d) {
	return _mm_set_epi32(d, c, b, a);
}

template <>
inline s128 set_wide<int, 4>(int a) {
	return _mm_set1_epi32(a);
}

template <size_t N>
inline s128 set_element_impl<int, 4, N>(s128 vec, int val) {
#if SSM_ARCH & SSM_ARCH_SSE4
	return _mm_insert_epi32(vec, val, N);
#else
	union {
		s128 vec;
		int[4] elems;
	} data;
	data.elems[N] = val;
	return vec;
#endif
}

template <size_t N>
inline int get_element_impl<int, 4, N>(s128 vec) {
#if SSM_ARCH & SSM_ARCH_SSE4
	return _mm_extract_epi32(vec, N);
#else
	const f128 shuf = _mm_shuffle_epi32(vec, vec, _MM_SHUFFLE(N, N, N, N));
	return _mm_cvt_si2ss(shuf);
#endif
}

template <>
inline int get_element_impl<int, 4, 0>(s128 vec) {
	return _mm_cvt_si2ss(vec);
}

inline s128 add(s128 a, s128 b) {
	return _mm_add_epi32(a, b);
}

inline s128 sub(s128 a, s128 b) {
	return _mm_sub_epi32(a, b);
}

inline s128 mul(s128 a, s128 b) {
#if SSM_ARCH & SSM_ARCH_SSE4
    return _mm_mullo_epi32(a, b);
#else
    const s128 tmp1 = _mm_mul_epu32(a, b);
    const s128 tmp2 = _mm_mul_epu32(_mm_srli_si128(a,4), _mm_srli_si128(b,4));
    return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0)));
#endif
}

inline s128 div(s128 a, s128 b) {
	return _mm_div_epi32(a, b);
}

template <size_t N>
inline s128 shuffle(s128 a) {
	static_assert(N < 4, "Shuffle index out of range");
	return _mm_shuffle_epi32(a, a, (N << 6) | (N << 4) | (N << 2) | N);
}

template <size_t X, size_t Y, size_t Z, size_t W>
inline s128 shuffle(s128 a, s128 b) {
	static_assert(X < 4, "Shuffle index out of range");
	static_assert(Y < 4, "Shuffle index out of range");
	static_assert(Z < 4, "Shuffle index out of range");
	static_assert(W < 4, "Shuffle index out of range");
	return _mm_shuffle_epi32(a, b, (W << 6) | (Z << 4) | (Y << 2) | X);
}

inline s128 cmp_eq(s128 a, s128 b) {
	return _mm_cmpeq_epi32(a, b);
}

inline s128 sqrt(s128 a) {

}
}
}
#endif