#include "test.h"

// Test several points at identical coordinates
int main() {

	rangetree<u16, const char> tree(10, 10);

	tree.add(1, 1, "one1");
	tree.add(1, 1, "one2");
	tree.add(1, 1, "one3");
	tree.add(1, 1, "one4");
	tree.add(1, 1, "one5");

	tree.add(5, 7, "tree1");
	tree.add(5, 7, "tree2");
	tree.add(5, 7, "tree3");
	tree.add(5, 7, "tree4");
	tree.add(5, 7, "tree5");
	tree.add(5, 7, "tree6");

	tree.finalize();


	std::vector<const char *> *first = tree.search(1, 1, 1, 1);
	u32 max = first->size();

	if (max != 5)
		fail("Wanted 5, got %u\n", max);

	u32 i;
	for (i = 0; i < max; i++) {
		const char * const s = (*first)[i];
		printf("Pos %u has %s\n", i, s);

		if (strncmp(s, "one", 3) != 0)
			fail("Unexpected string\n");
	}


	std::vector<const char *> *second = tree.search(3, 890, 0, 70);
	max = second->size();

	if (max != 6)
		fail("Wanted 6, got %u\n", max);

	for (i = 0; i < max; i++) {
		const char * const s = (*second)[i];
		printf("Pos %u has %s\n", i, s);

		if (strncmp(s, "tree", 4) != 0)
			fail("Unexpected string\n");
	}


	std::vector<const char *> *all = tree.search(0, 890, 0, 70);
	max = all->size();

	if (max != 11)
		fail("Wanted 11, got %u\n", max);

	delete first;
	delete second;
	delete all;

	return 0;
}
