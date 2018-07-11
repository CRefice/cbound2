namespace ssm
{
namespace simd
{
#if SSM_ARCH & SSM_ARCH_AVX
typedef __m256d d256;

template <>
struct aligned_type<double, 4>
{
	using type = d256;
};

template<>
struct is_simd<double, 4> { static constexpr bool value = true; }

template <size_t N>
inline d256 set_element_impl<double, 4, N>(d256 vec, double val) {
	__m128d temp = _mm256_extractf128_pd(vec, N / 2);
	temp = set_element<N % 2>(temp, val);
	return	_mm256_insertf128_pd(vec, temp, N / 2);
}

template <size_t N>
inline double get_element_impl<double, 4, N>(d256 vec) {
	const d128 extr1 = _mm256_extractf128_pd(vec, N / 2);
	const d128 shuf = _mm_shuffle_pd(extr1, extr1, _MM_SHUFFLE(N % 2, N % 2, N % 2, N % 2));
	return _mm_cvtsd_f64(shuf);
}
#endif
}
}
