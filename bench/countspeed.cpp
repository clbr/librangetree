#include "timing.h"

enum {
	maxnum = 1000000,
	powers = 7
};

static void check(const u32 max, u32 &create, u32 &search) {
	rangetree<u32, u32> tree(max, max);

	for (u32 i = 0; i < max; i++) {
		u32 x = rand() % maxnum;
		u32 y = rand() % maxnum;
		tree.add(x, y, 0);
	}

	timer t;
	tree.finalize();
	create = t.t();

	for (u32 i = 0; i < maxnum; i++) {
		u32 x = rand() % maxnum;
		u32 y = rand() % maxnum;

		tree.count(x, x+100, y, y+100);
	}

	search = t.t();
}

static u32 ipow(const u32 what, u32 where) {

	if (where)
		where--;

	u32 out = what;

	while (where) {
		out *= what;
		where--;
	}

	return out;
}

int main() {

	srand(time(NULL));

	u32 results[powers][2];

	printf("Benchmarking creation and search times...\n");

	u32 i;
	for (i = 0; i < powers; i++) {
		printf("10^%u.. ", i+1);
		fflush(stdout);

		const u32 power = ipow(10, i+1);
		check(power, results[i][0], results[i][1]);
	}
	puts("\n");

	for (i = 0; i < powers; i++) {
		const u32 power = ipow(10, i+1);
		printf("Using %u (10^%u) points, creation took %u us (%u ms)\n"
			"\tand 1M searches took %u us. (%.2f us/search)\n",
			power, i+1, results[i][0], results[i][0] / 1000,
			results[i][1],
			(float) results[i][1] / maxnum);
	}

	printf("\n\nProgression:\n");
	for (i = 1; i < powers; i++) {
		const float c = (float) results[i][0] / results[i - 1][0];
		const float s = (float) results[i][1] / results[i - 1][1];

		printf("Creation %.2fx, search %.2fx\n", c, s);
	}

	return 0;
}
