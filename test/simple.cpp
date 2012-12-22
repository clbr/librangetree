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

	return 0;
}
