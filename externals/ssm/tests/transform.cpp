#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ssm/transform.hpp>

bool epsilon_compare(float a, float b, float epsilon) {
	return std::abs(a - b) < epsilon;
}

TEST_CASE( "transformations", "[transform]" ) {
	SECTION( "identity" ) {
		ssm::mat4 mat = ssm::identity<float, 4>();
		ssm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

		ssm::vec4 vec_2 = mat * vec;
		REQUIRE(vec == vec_2);
	}

	SECTION( "homogenous coordinates" ) {
		ssm::vec3 vec(1.0f, 2.0f, 3.0f);
		ssm::vec4 hvec = ssm::homogenize(vec);
		REQUIRE(hvec == ssm::vec4(1.0f, 2.0f, 3.0f, 1.0f));
		REQUIRE(ssm::dehomogenize(hvec) == vec);
	}

	SECTION( "scaling" ) {
		ssm::vec4 scale(2.0f, 3.0f, 5.0f, 10.0f);
		ssm::mat4 mat = ssm::scaling(scale);
		ssm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);

		ssm::vec4 vec_2 = mat * vec;
		REQUIRE(epsilon_compare(vec_2.x, 2.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.y, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.z, 15.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.w, 40.0f, 1e-6f));
	}

	SECTION( "translation" ) {
		ssm::vec3 pos(3.0f, 4.0f, 5.0f);
		ssm::mat4 mat = ssm::translation(pos);
		ssm::vec4 vec(1.0f, 2.0f, 3.0f, 1.0f);

		ssm::vec4 vec_2 = mat * vec;
		REQUIRE(epsilon_compare(vec_2.x, 4.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.y, 6.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.z, 8.0f, 1e-6f));
		REQUIRE(epsilon_compare(vec_2.w, 1.0f, 1e-6f));
	}
}
