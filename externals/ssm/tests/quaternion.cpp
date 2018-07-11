#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ssm/quaternion.hpp>

bool epsilon_compare(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

TEST_CASE( "operations", "[quaternion]" ) {
	ssm::quat q;

	SECTION( "equality comparisons" ) {
		REQUIRE(q == q);
		ssm::quat q2(10.0f);
		REQUIRE(q != q2);
		REQUIRE(q2 == q2);

		q2 = q;
		REQUIRE(q == q2);
		q2.w = 5.0f;
		REQUIRE(q != q2);
	}

	SECTION( "norm" ) {
		ssm::quat q;

		q = ssm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		REQUIRE(ssm::sqnorm(q) == 1.0f);
		REQUIRE(ssm::norm(q) == 1.0f);
		q = ssm::quat(1.0f, 1.0f, 0.0f, 0.0f);
		REQUIRE(epsilon_compare(ssm::sqnorm(q), 2.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::norm(q), std::sqrt(2.0f), 1e-6f));
		q = ssm::quat(1.0f, 1.0f, 1.0f, 0.0f);
		REQUIRE(epsilon_compare(ssm::sqnorm(q), 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::norm(q), std::sqrt(3.0f), 1e-6f));
		q = ssm::quat(1.0f, 1.0f, 1.0f, 1.0f);
		REQUIRE(epsilon_compare(ssm::sqnorm(q), 4.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::norm(q), 2.0f, 1e-6f));
		q = ssm::quat(1.0f, 2.0f, 3.0f, 4.0f);
		REQUIRE(epsilon_compare(ssm::sqnorm(q), 30.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::norm(q), std::sqrt(30.0f), 1e-6f));
	}

	SECTION( "conjugate" ) {
		auto q = ssm::quat(5.0f);
		q = ssm::conjugate(q);
		REQUIRE(q.x == -5.0f);
		REQUIRE(q.y == -5.0f);
		REQUIRE(q.z == -5.0f);
		REQUIRE(q.w == 5.0f);
	}

	SECTION( "inverse" ) {
		auto q = ssm::quat(5.0f);
		auto q2 = ssm::inverse(q);
		q = ssm::conjugate(q);
		auto norm = ssm::sqnorm(q);
		REQUIRE(q2 == q / norm);

		auto uq = ssm::normalize(q);
		auto uq2 = ssm::inverse(uq);
		uq = ssm::conjugate(uq);
		auto unorm = ssm::sqnorm(uq); // Should be one
		REQUIRE(uq2 == uq / unorm);
	}

	SECTION( "normalization" ) {
		q = ssm::quat(1.0f);
		auto uq = ssm::normalize(q);
		REQUIRE(epsilon_compare(ssm::norm(uq), 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(uq.x, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(uq.y, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(uq.z, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(uq.w, 0.5f, 1e-6f));
	}

	SECTION( "arithmetic operators" ) {
		q = ssm::quat(1.0f, 2.0f, 3.0f, 4.0f);
		auto q2 = ssm::quat(4.0f, 3.0f, 2.0f, 1.0f);

		REQUIRE(q + q2 == q2 + q);
		auto q3 = q;
		q3 += q2;
		REQUIRE(q3 == q + q2);
		REQUIRE(epsilon_compare(q3.x, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.y, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.z, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.w, 5.0f, 1e-6f));

		q3 = q;
		q3 -= q2;
		REQUIRE(q3 == q - q2);
		REQUIRE(epsilon_compare(q3.x, -3.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.y, -1.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.z, 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.w, 3.0f, 1e-6f));

		REQUIRE(q * 3.0f == 3.0f * q);
		q3 = q;
		q3 *= 3.0f;
		REQUIRE(q3 == q * 3.0f);
		REQUIRE(epsilon_compare(q3.x, 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.y, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.z, 9.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.w, 12.0f, 1e-6f));

		q3 = q;
		q3 /= 3.0f;
		REQUIRE(q3 == q / 3.0f);
		REQUIRE(epsilon_compare(q3.x, 1.0f / 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.y, 2.0f / 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.z, 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.w, 4.0f / 3.0f, 1e-6f));

		q3 = -q;
		REQUIRE(q3.x == -1.0f);
		REQUIRE(q3.y == -2.0f);
		REQUIRE(q3.z == -3.0f);
		REQUIRE(q3.w == -4.0f);
	}

	SECTION( "multiplication" ) {
		ssm::quat q(-2.0f, 3.0f, 2.0f, 5.0f);
		ssm::quat q2(5.0f, 7.0f, 9.0f, 3.0f);
		REQUIRE(q * q2 != q2 * q);
		ssm::quat q3 = q;
		q3 *= q2;
		REQUIRE(q3 == q * q2);
		REQUIRE(epsilon_compare(q3.x, 32.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.y, 72.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.z, 22.0f, 1e-6f));
		REQUIRE(epsilon_compare(q3.w, -14.0f, 1e-6f));
	}
}