#include "test.h"

int main() {

	rangetree<u16, const char> tree;

	tree.add(1, 1, "one");
	tree.add(1, 2, "two");
	tree.add(1, 3, "three");

	tree.finalize();

	u32 res = tree.count(0, 1, 0, 1);
	printf("The number of points in x (0, 1) and y (0, 1) is %u\n", res);
	if (res != 1)
		fail("Count wrong, got %u, expected 1\n", res);

	res = tree.count(0, 0, 0, 1);
	if (res != 0)
		fail("Count wrong, got %u, expected 1\n", res);

	res = tree.count(0, 800, 0, 1);
	if (res != 1)
		fail("Count wrong, got %u, expected 1\n", res);

	res = tree.count(0, 800, 0, 2);
	if (res != 2)
		fail("Count wrong, got %u, expected 1\n", res);

	res = tree.count(0, 80, 0, 81);
	if (res != 3)
		fail("Count wrong, got %u, expected 1\n", res);

	std::vector<const char *> *joo = tree.search(0,500,2,600);
	printf("(0,500) (2,600) search got %ld results\n", joo->size());
	for (u32 i = 0; i < joo->size(); i++) {
		printf("Node %u name %s\n", i, (*joo)[i]);
	}

	if (joo->size() != 2)
		fail("Search count wrong, got %u expected 2\n", joo->size());

	return 0;
}
