#include "timing.h"

enum {
	highnum = 1000000
};

static u32 subrand() {
	return rand() % highnum;
}

int main(int argc, char **argv) {

	u32 mils = 1;
	if (argc >= 2)
		mils = atoi(argv[1]);
	printf("Creating a tree with %u million points, to measure the RAM consumption.\n",
		mils);

	rangetree<u32, u32> tree;

	srand(time(NULL));

	u32 i;
	const u32 max = mils * 1000 * 1000;
	for (i = 0; i < max; i++) {
		tree.add(subrand(), subrand(), 0);
	}
	tree.finalize();

	return 0;
}
