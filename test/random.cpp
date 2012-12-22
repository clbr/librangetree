#include "test.h"
#include <time.h>
#include <limits.h>

enum {
	MAX = 500
};

struct hoo {
	u16 x, y;
};

static inline u16 smallrand() {
	return rand() % USHRT_MAX;
}

// Linear calculation to check our tree
static u32 calc(hoo *const arr, u16 xmin, u16 xmax, u16 ymin, u16 ymax) {
	u32 sum = 0;
	u32 i;

	for (i = 0; i < MAX; i++) {
		if (arr[i].x >= xmin &&
			arr[i].x <= xmax &&
			arr[i].y >= ymin &&
			arr[i].y <= ymax)
			sum++;
	}

	return sum;
}

int main() {

	srand(time(NULL));

	hoo arr[MAX];
	rangetree<u16, u16> tree(MAX, MAX);

	u32 i;
	for (i = 0; i < MAX; i++) {
		arr[i].x = smallrand();
		arr[i].y = smallrand();

		tree.add(arr[i].x, arr[i].y, 0);
	}

	tree.finalize();

	// Do some random searches
	u32 xmin, xmax, ymin, ymax;

	for (i = 0; i < MAX; i++) {
		xmin = smallrand();
		xmax = smallrand();
		ymin = smallrand();
		ymax = smallrand();

		const u32 check = calc(arr, xmin, xmax, ymin, ymax);
		const u32 count = tree.count(xmin, xmax, ymin, ymax);
		std::vector<u16 *> *he = tree.search(xmin, xmax, ymin, ymax);
		const u32 count2 = he->size();
		delete he;

		if (check != count || check != count2)
			fail("Expected %u, got %u and %u\n", check, count, count2);
	}

	return 0;
}
