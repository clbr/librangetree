#include "test.h"

int main() {

	rangetree<u16, u16> tree;

	tree.add(0,0,0);
	tree.add(1,0,0);
	tree.add(2,0,0);

	tree.finalize();


	u16 *ptr[1];
	u32 num = 1;

	tree.search(ptr, num, 0, 5, 0, 5);

	printf("Found %u, fit one (%p)\n", num, ptr[0]);

	return 0;
}
