#include "test.h"

enum {
	max = 1000
};

// This tests mainly corruption, ie is the correct data returned
int main() {

	rangetree<u16, u16> tree(max, 1);

	u32 i;
	for (i = 0; i < max; i++) {
		tree.add(i, i, (u16 *) i);
	}
	tree.finalize();

	for (i = 0; i < max; i++) {
		std::vector<u16 *> *vec = tree.search(i, i, i, i);

		if (vec->size() != 1)
			fail("Size wrong, expected 1, got %u\n", vec->size());
		if ((u64) (*vec)[0] != i)
			fail("Corrupted content, expected %u, got %u\n", i, (*vec)[0]);

		delete vec;
	}

	return 0;
}
