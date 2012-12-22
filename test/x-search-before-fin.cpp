#include "test.h"

int main() {

	rangetree<u16, u16> tree;
	std::vector<u16 *> *p = tree.search(0,0,0,0);

	if (!p)
		return 1;

	return 0;
}
