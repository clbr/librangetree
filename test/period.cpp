#include "test.h"
#include <limits.h>

int main() {

	rangetree<u16, u16> tree;

	u16 x, y;

	for (x = 0; x < USHRT_MAX; x++) {
		for (y = 0; y < USHRT_MAX; y++) {
			tree.add(x, y, 0);
		}
	}

	tree.finalize();

	// With points in every coordinate, we can easily calculate the counts

	for (x = 10; x < USHRT_MAX; x += 10) {
		for (y = 10; y < USHRT_MAX; y += 10) {
			const u32 count = tree.count(1, x, 1, y);
			const u32 calc = x*y;

			if (count != calc)
				fail("Got %u, expected %u\n", count, calc);
		}
	}

	return 0;
}
