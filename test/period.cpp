#include "test.h"

int main() {

	const u32 MAX = 500;

	rangetree<u16, u16> tree;

	u16 x, y;

	for (x = 0; x < MAX; x++) {
		for (y = 0; y < MAX; y++) {
			tree.add(x, y, 0);
		}
	}

	tree.finalize();

	// With points in every coordinate, we can easily calculate the counts

	for (x = 10; x < MAX; x += 10) {
		for (y = 10; y < MAX; y += 10) {
			const u32 count = tree.count(1, x, 1, y);
			const u32 calc = x*y;

			if (count != calc)
				fail("Got %u, expected %u\n", count, calc);
		}
	}

	return 0;
}
