/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "fcb_test.h"

ZTEST(fcb_test_with_2sectors_set, test_fcb_corrupted_sector_header_magic)
{
	int rc;

	struct fcb *fcb = &test_fcb;

	int number_of_free_sectors = fcb_free_sector_cnt(fcb);
	zassert_equal(number_of_free_sectors, 1, "Expected number of free sectors to be 1, was %d", number_of_free_sectors);

	// Inject an error in the magic in second sector, simulating
	// writing the header whilst power is cut and the magic
	// being corrupted.
	const uint32_t wrong_magic = 0xDEADBEEF;
	rc = fcb_flash_write(fcb, &test_fcb_sector[1], 0, &wrong_magic, sizeof(wrong_magic));
	zassert_ok(rc,  "flash_area_write failed (%d)", rc);

	rc = fcb_init(TEST_FCB_FLASH_AREA_ID, fcb);
	zassert_ok(rc,  "fcb_init failed (%d)", rc);

	number_of_free_sectors = fcb_free_sector_cnt(fcb);
	zassert_equal(number_of_free_sectors, 1, "Expected number of free sectors to be 1, was %d", number_of_free_sectors);
}
