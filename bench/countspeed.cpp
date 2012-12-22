#include "timing.h"

static void check(const u32 max, u32 &create, u32 &search) {
	rangetree<u32, u32> tree(max, max);

	for (u32 i = 0; i < max; i++) {
		u32 x = rand();
		u32 y = rand();
		tree.add(x, y, 0);
	}

	timer t;
	tree.finalize();
	create = t.t();

	for (u32 i = 0; i < max; i++) {
		u32 x = rand();
		u32 y = rand();

		tree.count(x, x+10, y, y+10);
	}

	search = t.t();
}

static u32 ipow(u32 what, u32 where) {

	if (where)
		where--;

	while (where) {
		what *= what;
		where--;
	}
printf("Pow saying %u\n", what);
	return what;
}

int main() {

	srand(time(NULL));

	u32 results[4][2];

	printf("Benchmarking creation and search times...\n");

	u32 i;
	for (i = 0; i < 4; i++) {
		const u32 power = ipow(10, i+1);
		check(power, results[i][0], results[i][1]);
	}

	for (i = 0; i < 4; i++) {
		const u32 power = ipow(10, i+1);
		printf("Using %u points, creation took %u ms\n"
			"and the same amount of searches took %u ms.\n",
			power, results[i][0], results[i][1]);
	}

	printf("n\nProgression:\n");
	for (i = 1; i < 4; i++) {
		const float c = (float) results[i][0] / results[i - 1][0];
		const float s = (float) results[i][0] / results[i - 1][0];

		printf("Creationg %.2fx, search %.2fx\n", c, s);
	}

	return 0;
}
