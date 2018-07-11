#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ssm/matrix.hpp>

bool epsilon_compare(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

bool epsilon_compare(const ssm::mat4& a, const ssm::mat4& b, float epsilon) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (!epsilon_compare(a[i][j], b[i][j], epsilon))
				return false;
		}
	}
	return true;
}

TEST_CASE("matrix constructors", "[matrix]") {
	ssm::mat4 mat;

	SECTION("default constructor") {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; i < 4; ++i)
				REQUIRE(mat[i][j] == 0.0f);
		}
	}

	SECTION("one-argument constructor") {
		mat = ssm::mat4(5.0f);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; i < 4; ++i)
				REQUIRE(mat[i][j] == 5.0f);
		}
	}
}

TEST_CASE("equality comparisons", "[matrix]") {
	ssm::mat4 mat_1(1.0f);
	ssm::mat4 mat_2(2.0f);

	REQUIRE(mat_1 == mat_1);
	REQUIRE(mat_2 == mat_2);
	REQUIRE(mat_1 != mat_2);
	mat_2 = mat_1;
	REQUIRE(mat_1 == mat_2);
	mat_2[2][2] = 10.0f;
	REQUIRE(mat_1 != mat_2);
}

TEST_CASE("operations", "[matrix]") {
	ssm::mat4 mat(3.0f);

	SECTION("arithmetic operators") {
		ssm::mat4 mat_2(7.0f);

		REQUIRE(mat + mat_2 == mat_2 + mat);
		auto mat_3 = mat;
		mat_3 += mat_2;
		REQUIRE(mat_3 == mat + mat_2);
		REQUIRE(mat_3 == ssm::mat4(3.0f + 7.0f));

		mat_3 = mat;
		mat_3 -= mat_2;
		REQUIRE(mat_3 == mat - mat_2);
		REQUIRE(mat_3 == ssm::mat4(3.0f - 7.0f));

		REQUIRE(mat * 3.0f == 3.0f * mat);
		mat_3 = mat;
		mat_3 *= 3.0f;
		REQUIRE(mat_3 == mat * 3.0f);

		mat_3 = mat;
		mat_3 /= 3.0f;
		REQUIRE(mat_3 == mat / 3.0f);

		mat_3 = -mat;
		REQUIRE(mat_3 == ssm::mat4(-3.0f));
	}

	SECTION("multiplication") {
		mat[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
		mat[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
		mat[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
		mat[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

		ssm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
		REQUIRE((vec * mat) == (mat * vec));
		auto vec_2 = vec;
		vec_2 *= mat;
		REQUIRE(vec_2 == mat * vec);
		REQUIRE(vec_2 == vec);

		mat = ssm::mat4(2.0f);
		vec_2 = vec;
		vec_2 *= mat;
		REQUIRE(vec_2 == mat * vec);
		REQUIRE(epsilon_compare(vec_2.x, 20.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.y, 20.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.z, 20.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.w, 20.0f, 1e-6f));

		ssm::mat4 mat_2;
		mat_2[0] = { 1.0f, 2.0f, 3.0f, 4.0f };
		mat_2[1] = { 5.0f, 6.0f, 7.0f, 8.0f };
		mat_2[2] = { 9.0f, 10.0f, 11.0f, 12.0f };
		mat_2[3] = { 13.0f, 14.0f, 15.0f, 16.0f };

		REQUIRE(mat * mat_2 != mat_2 * mat);
		auto mat_3 = mat_2;
		mat_3 *= mat;
		REQUIRE(mat_3 == mat_2 * mat);

		ssm::mat4 result;
		result[0] = { 20.0f, 20.0f, 20.0f, 20.0f };
		result[1] = { 52.0f, 52.0f, 52.0f, 52.0f };
		result[2] = { 84.0f, 84.0f, 84.0f, 84.0f };
		result[3] = { 116.0f, 116.0f, 116.0f, 116.0f };

		REQUIRE(epsilon_compare(mat_3, result, 1e-6f));
	}
}
