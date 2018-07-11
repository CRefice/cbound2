#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ssm/vector.hpp>

bool epsilon_compare(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

TEST_CASE( "constructors", "[vector]" ) {
	ssm::vec4 vec;
	ssm::vec3 vec3;

	SECTION( "default constructor" ) {
		REQUIRE(vec.x == 0.0f);
		REQUIRE(vec.y == 0.0f);
		REQUIRE(vec.z == 0.0f);
		REQUIRE(vec.w == 0.0f);

		REQUIRE(vec3.x == 0.0f);
		REQUIRE(vec3.y == 0.0f);
		REQUIRE(vec3.z == 0.0f);
	}

	SECTION( "one-argument constructor" ) {
		vec = ssm::vec4(5.0f);
		vec3 = ssm::vec3(5.0f);

		REQUIRE(vec.x == 5.0f);
		REQUIRE(vec.y == 5.0f);
		REQUIRE(vec.z == 5.0f);
		REQUIRE(vec.w == 5.0f);

		REQUIRE(vec3.x == 5.0f);
		REQUIRE(vec3.y == 5.0f);
		REQUIRE(vec3.z == 5.0f);
	}

	SECTION( "four-argument constructor" ) {
		vec = ssm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
		REQUIRE(vec.x == 4.0f);
		REQUIRE(vec.y == 3.0f);
		REQUIRE(vec.z == 2.0f);
		REQUIRE(vec.w == 1.0f);
	}
}

TEST_CASE("member getters-setters", "[vector]") {
	ssm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
	vec.x = 11.0f;
	REQUIRE(vec.x == 11.0f);
	vec.y = 12.0f;
	REQUIRE(vec.y == 12.0f);
	vec.z = 13.0f;
	REQUIRE(vec.z == 13.0f);
	vec.w = 14.0f;
	REQUIRE(vec.w == 14.0f);
}

TEST_CASE( "operations", "[vector]" ) {
	ssm::vec4 vec;

	SECTION( "equality comparisons" ) {
		REQUIRE(vec == vec);
		ssm::vec4 vec_2 (10.0f);
		REQUIRE(vec != vec_2);
		REQUIRE(vec_2 == vec_2);

		vec_2 = vec;
		REQUIRE(vec == vec_2);
		vec_2.w = 5.0f;
		REQUIRE(vec != vec_2);
	}

	SECTION( "length" ) {
		ssm::vec4 vec;

		vec = ssm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		REQUIRE(ssm::sqlength(vec) == 1.0f);
		REQUIRE(ssm::length(vec) == 1.0f);
		vec = ssm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
		REQUIRE(epsilon_compare(ssm::sqlength(vec), 2.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::length(vec), std::sqrt(2.0f), 1e-6f));
		vec = ssm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		REQUIRE(epsilon_compare(ssm::sqlength(vec), 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::length(vec), std::sqrt(3.0f), 1e-6f));
		vec = ssm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		REQUIRE(epsilon_compare(ssm::sqlength(vec), 4.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::length(vec), 2.0f, 1e-6f));
		vec = ssm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		REQUIRE(epsilon_compare(ssm::sqlength(vec), 30.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::length(vec), std::sqrt(30.0f), 1e-6f));
	}

	SECTION( "dot product" ) {
		auto vec_1 = ssm::vec4(2.0f, 3.0f, 1.0f, 1.5f);
		auto vec_2 = ssm::vec4(3.0f, 1.5f, 1.0f, 4.0f);
		REQUIRE(epsilon_compare(ssm::dot(vec_1, vec_2), 17.5f, 1e-6f));
	}

	SECTION( "distance" ) {
		auto vec_1 = ssm::vec3(7.0f, 4.0f, 3.0f);
		auto vec_2 = ssm::vec3(17.0f, 6.0f, 2.0f);
		REQUIRE(epsilon_compare(ssm::distance(vec_1, vec_1), 0.0f, 1e-6f));
		REQUIRE(epsilon_compare(ssm::distance(vec_1, vec_2), 10.246951f, 1e-6f));
	}

	SECTION( "cross product" ) {
		auto vec_1 = ssm::vec3(1.0f, 0.0f, 0.0f);
		auto vec_2 = ssm::vec3(0.0f, 1.0f, 0.0f);
		auto cross = ssm::cross(vec_1, vec_2);
		REQUIRE(epsilon_compare(cross.x, 0.0f, 1e-6f));
		REQUIRE(epsilon_compare(cross.y, 0.0f, 1e-6f));
		REQUIRE(epsilon_compare(cross.z, 1.0f, 1e-6f));
		
		vec_1 = ssm::vec3(1.0f, 2.0f, 3.0f);
		vec_2 = ssm::vec3(1.0f, 5.0f, 7.0f);
		cross = ssm::cross(vec_1, vec_2);
		REQUIRE(epsilon_compare(cross.x, -1.0f, 1e-6f));
		REQUIRE(epsilon_compare(cross.y, -4.0f, 1e-6f));
		REQUIRE(epsilon_compare(cross.z, 3.0f, 1e-6f));
	}

	SECTION( "normalization" ) {
		vec = ssm::vec4(1.0f);
		auto norm = ssm::normalize(vec);
		REQUIRE(epsilon_compare(ssm::length(norm), 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(norm.x, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(norm.y, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(norm.z, 0.5f, 1e-6f));
		REQUIRE(epsilon_compare(norm.w, 0.5f, 1e-6f));
	}

	SECTION("arithmetic operators") {
		auto vec3 = ssm::vec3(3.0f, 2.0f, 1.0f);
		vec3 += ssm::vec3(1.0f, 2.0f, 3.0f);
		REQUIRE(vec3 == ssm::vec3(4.0f, 4.0f, 4.0f));
		
		auto vec5 = ssm::vector<float, 5>(3.0f, 2.0f, 1.0f, 2.0f, 3.0f);
		auto vec5_2 = vec5 + ssm::vector<float, 5>(3.0f, 2.0f, 1.0f, 2.0f, 3.0f);
		REQUIRE(vec5_2 == 2.0f * vec5);

		vec = ssm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
		auto vec_2 = ssm::vec4(4.0f, 3.0f, 2.0f, 1.0f);

		REQUIRE(vec + vec_2 == vec_2 + vec);
		auto vec_3 = vec;
		vec_3 += vec_2;
		REQUIRE(vec_3 == vec + vec_2);
		REQUIRE(epsilon_compare(vec_3.x, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 5.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 5.0f, 1e-6f));

		vec_3 = vec;
		vec_3 -= vec_2;
		REQUIRE(vec_3 == vec - vec_2);
		REQUIRE(epsilon_compare(vec_3.x, -3.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, -1.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 3.0f, 1e-6f));

		REQUIRE(vec * vec_2 == vec_2 * vec);
		vec_3 = vec;
		vec_3 *= vec_2;
		REQUIRE(vec_3 == vec * vec_2);
		REQUIRE(epsilon_compare(vec_3.x, 4.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 4.0f, 1e-6f));

		REQUIRE(vec * 3.0f == 3.0f * vec);
		vec_3 = vec;
		vec_3 *= 3.0f;
		REQUIRE(vec_3 == vec * 3.0f);
		REQUIRE(epsilon_compare(vec_3.x, 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 9.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 12.0f, 1e-6f));

		vec_3 = vec;
		vec_3 /= vec_2;
		REQUIRE(vec_3 == vec / vec_2);
		REQUIRE(epsilon_compare(vec_3.x, 0.25f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, 2.0f / 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 1.5f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 4.0f, 1e-6f));

		vec_3 = vec;
		vec_3 /= 3.0f;
		REQUIRE(vec_3 == vec / 3.0f);
		REQUIRE(epsilon_compare(vec_3.x, 1.0f / 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.y, 2.0f / 3.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.z, 1.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_3.w, 4.0f / 3.0f, 1e-6f));

		vec_3 = -vec;
		REQUIRE(vec_3.x == -1.0f);
		REQUIRE(vec_3.y == -2.0f);
		REQUIRE(vec_3.z == -3.0f);
		REQUIRE(vec_3.w == -4.0f);
	}
}
