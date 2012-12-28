#include "test.h"

int main() {

	rangetree<s16, s16> tree;

	tree.add(-1, -1, 0);
	tree.add(1, 1, 0);

	tree.finalize();

	u32 all = tree.count(-50, 50, -50, 50);
	if (all != 2)
		fail("Expected 2, got %u\n", all);

	return 0;
}
