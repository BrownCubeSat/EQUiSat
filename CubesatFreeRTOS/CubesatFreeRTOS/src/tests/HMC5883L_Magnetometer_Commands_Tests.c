#include "HMC5883L_Magnetometer_Commands_Tests.h"

void run_test_HMC5883L_init(const struct test_case *test) {
	set_i2c_mock_test_case(test);
	HMC5883L_init(&i2c_write_mock, &i2c_read_mock);
	uint8_t *init_packets = get_written_bytes();
	test_assert_true(test, init_packets[0] == 0x00, "First HMC5883L initialization byte incorrect.");
	test_assert_true(test, init_packets[1] == 0x70, "Second HMC5883L initialization byte incorrect.");
	test_assert_true(test, init_packets[2] == 0x01, "Third HMC5883L initialization byte incorrect.");
	test_assert_true(test, init_packets[3] == 0xA0, "Fourth HMC5883L initialization byte incorrect.");
}

void run_test_HMC5883L_read(const struct test_case *test) {
	run_test_HMC5883L_init(test);
	uint8_t dummy_read_values[6] = {
		0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};
	i2c_mock_set_next_read_bytes(dummy_read_values);
	uint8_t read_buffer[6] = {0, 0, 0, 0, 0, 0};
	HMC5883L_read(read_buffer);
	for(int i=0; i<6; i++) {
		test_assert_true(test, read_buffer[i] == dummy_read_values[i], "HMC5883L read byte incorrect.");
	}
	uint8_t *init_packets = get_written_bytes();
	// first four bytes were init bytes
	test_assert_true(test, init_packets[4] == 0x02, "Third HMC5883L byte incorrect.");
	test_assert_true(test, init_packets[5] == 0x01, "Fourth HMC5883L byte incorrect.");
}

void run_test_HMC5883L_getXYZ(const struct test_case *test) {
	uint8_t dummy_read_values[6] = {
		0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};
	int16_t xyz_buffer[3] = {0, 0, 0};
	HMC5883L_getXYZ(dummy_read_values, xyz_buffer);
	test_assert_true(test, xyz_buffer[0] == 0x0A0B, "HMC5883L getXYZ x byte incorrect.");
	test_assert_true(test, xyz_buffer[1] == 0x0C0D, "HMC5883L getXYZ y byte incorrect.");
	test_assert_true(test, xyz_buffer[2] == 0x0E0F, "HMC5883L getXYZ z byte incorrect.");
}

void run_test_HMC5883L_computeCompassDir(const struct test_case *test) {
	float heading_degrees = HMC5883L_computeCompassDir(0xAB, 0xCD, 0xEF); // 171, 205, 239
	float epsilon = 0.0000001;
	test_assert_true(test, abs(heading_degrees - 68.41698) < epsilon, "HMC5883L computeCompassDir incorrect.");
}


void run_HMC5883L_tests(void)
{
	/* Define Test Cases */
	DEFINE_TEST_CASE(test_HMC5883L_init,
					NULL,
					run_test_HMC5883L_init,
					NULL,
					"Testing HMC5883L I2C initialization");
	DEFINE_TEST_CASE(test_HMC5883L_read,
					NULL,
					run_test_HMC5883L_read,
					NULL,
					"Testing HMC5883L I2C read functionality");
	DEFINE_TEST_CASE(test_HMC5883L_getXYZ,
					NULL,
					run_test_HMC5883L_getXYZ,
					NULL,
					"Testing HMC5883L I2C getXYZ functionality");
	DEFINE_TEST_CASE(test_HMC5883L_computeCompassDir,
					NULL,
					run_test_HMC5883L_computeCompassDir,
					NULL,
					"Testing HMC5883L I2C computeCompassDir functionality");

	/* Put test case addresses in an array */
	DEFINE_TEST_ARRAY(i2c_tests) = {
		&test_HMC5883L_init,
		&test_HMC5883L_read,
		&test_HMC5883L_getXYZ,
		&test_HMC5883L_computeCompassDir
	};

	/* Define the test suite */
	DEFINE_TEST_SUITE(i2c_test_suite, i2c_tests, "HMC5883L I2C test suite");

	/* Run all tests in the suite*/
	test_suite_run(&i2c_test_suite);
}