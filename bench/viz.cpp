#define LR_VISUALIZE
#include <ranget.h>

int main() {

	rangetree<u16,u16> tree(10, 10);

	tree.add(1, 0, 0);
	tree.add(10, 0, 0);
	tree.add(100, 0, 0);

	tree.finalize();

	tree.visualize();
	tree.print();

	return 0;
}
