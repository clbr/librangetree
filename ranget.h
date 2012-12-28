/*
    libranget - optimized 2d range tree
    Copyright (C) 2012 Lauri Kasanen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBRANGET
#define LIBRANGET

#include "lrtypes.h"

#ifdef __GNUC__
#define fetch(a) __builtin_prefetch(a)
#else
#define fetch(a)
#endif

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>

template <class point, class data> class rangetree {

public:
	rangetree(u32 estimatedTotal = 1000, u32 estimatedResult = 100):
		pool(NULL),
		mainreserve(estimatedTotal), resultreserve(estimatedResult), init(false) {

		xtmparray.reserve(mainreserve);
		ytmparray.reserve(mainreserve);
	}

	~rangetree() {
		nuke();
		delete [] pool;
	}

	int add(point x, point y, data * const ptr) {

		if (init)
			return 1;

		ptx px;
		pty py;

		px.x = py.x = x;
		px.y = py.y = y;
		px.ptr = py.ptr = ptr;

		xtmparray.push_back(px);
		ytmparray.push_back(py);

		return 0;
	}

	void finalize() {

		init = true;

		totalsize = ytmparray.size();

		initpool(totalsize);

		std::sort(xtmparray.begin(), xtmparray.end());
		std::sort(ytmparray.begin(), ytmparray.end());

		build();

		xtmparray.clear();
	}

	u32 count(point xmin, point xmax, point ymin, point ymax) const {

		if (!init)
			return 0;

		// If needed, swap arguments
		if (xmax < xmin)
			pswap(xmax, xmin);
		if (ymax < ymin)
			pswap(ymax, ymin);

		u32 sum = 0;

		std::vector<const node *> list;
		findnodes(&start, xmin, xmax, list);

		const u32 ncount = list.size();

		for (u32 k = 0; k < ncount; k++) {
			const node * const n = list[k];
			const u32 max = n->ypoints.size();
			if (!max)
				continue;

			const u32 lower = binarynext(n->ypoints, ymin);
			const u32 upper = binarynext(n->ypoints, ymax + 1);

			for (u32 i = lower; i < upper; i++) {
					sum++;
			}
		}

		return sum;
	}

	std::vector<data *> *search(point xmin, point xmax, point ymin, point ymax) const {

		if (!init)
			return NULL;

		// If needed, swap arguments
		if (xmax < xmin)
			pswap(xmax, xmin);
		if (ymax < ymin)
			pswap(ymax, ymin);

		std::vector<data *> * const res = new std::vector<data *>;
		res->reserve(resultreserve);

		std::vector<const node *> list;
		findnodes(&start, xmin, xmax, list);

		const u32 ncount = list.size();

		for (u32 k = 0; k < ncount; k++) {
			const node * const n = list[k];
			const u32 max = n->ypoints.size();
			if (!max)
				continue;

			const u32 lower = binarynext(n->ypoints, ymin);
			const u32 upper = binarynext(n->ypoints, ymax + 1);

			for (u32 i = lower; i < upper; i++) {
					res->push_back(n->ypoints[i].ptr);
			}
		}
		return res;
	}

	// Alternative interface for faster searches, avoiding the mem alloc
	void search(data ** const arr, u32 &arrsize,
			point xmin, point xmax, point ymin, point ymax) const {

		if (!init)
			return;

		// If needed, swap arguments
		if (xmax < xmin)
			pswap(xmax, xmin);
		if (ymax < ymin)
			pswap(ymax, ymin);

		std::vector<const node *> list;
		findnodes(&start, xmin, xmax, list);

		const u32 ncount = list.size();

		u32 cur = 0;
		const u32 arrmax = arrsize;

		for (u32 k = 0; k < ncount; k++) {
			const node * const n = list[k];
			const u32 max = n->ypoints.size();
			if (!max)
				continue;

			const u32 lower = binarynext(n->ypoints, ymin);
			const u32 upper = binarynext(n->ypoints, ymax + 1);

			for (u32 i = lower; i < upper; i++) {
					if (cur < arrmax)
						arr[cur] = n->ypoints[i].ptr;

					cur++;
			}
		}

		arrsize = cur;
	}

	static const char *version() {
		return "librangetree 0.1";
	}

private:
	struct ptx {
		point x;
		point y;
		data * ptr;

		inline bool operator < (const ptx &other) const {
			if (x >= other.x)
				return false;
			return true;
		}
	};
	struct pty {
		point x;
		point y;
		data * ptr;

		inline bool operator < (const pty &other) const {
			if (y >= other.y)
				return false;
			return true;
		}
		pty(const ptx &px) {
			x = px.x;
			y = px.y;
			ptr = px.ptr;
		}
		pty() {}
	};
	struct node {
		node * left;
		node * right;

		point min, max;

		std::vector<pty> ypoints;

		node(): left(NULL), right(NULL), min(0), max(0) {}
	};

	// A binary search that returns the index if found, the next index if not
	u32 binarynext(const std::vector<pty> &arr, const point goal) const {

		if (arr.size() == 0)
			return 0;

		const u32 max = arr.size() - 1;

		// These have to be signed to avoid overflow. s64 to contain u32.
		s64 t, left = 0, right = max;

		do {
			t = (left + right) / 2;

			if (goal < arr[t].y)
				right = t - 1;
			else
				left = t + 1;

		} while (left <= right && goal != arr[t].y);

		// We might've landed in the middle. Linearly get the earliest.
		if (arr[t].y == goal) {
			while (t && arr[t-1].y == goal)
				t--;
			return t;
		}

		if (arr[t].y < goal)
			return t + 1;

		return t;
	}

	// Same for X-sorted arrays
	u32 binarynextx(const std::vector<ptx> &arr, const point goal) const {

		if (arr.size() == 0)
			return 0;

		const u32 max = arr.size() - 1;

		// These have to be signed to avoid overflow. s64 to contain u32.
		s64 t, left = 0, right = max;

		do {
			t = (left + right) / 2;

			if (goal < arr[t].x)
				right = t - 1;
			else
				left = t + 1;

		} while (left <= right && goal != arr[t].x);

		// We might've landed in the middle. Linearly get the earliest.
		if (arr[t].x == goal) {
			while (t && arr[t-1].x == goal)
				t--;
			return t;
		}

		if (arr[t].x < goal)
			return t + 1;

		return t;
	}

	void build() {
		if (totalsize < 2) {
			// Trees of a single point aren't supported
			return;
		}

		start.ypoints = ytmparray;
		ytmparray.clear();

		const u32 medianidx = totalsize / 2;
		const point median = xtmparray[medianidx].x;

		start.min = 0;
		start.max = xtmparray[totalsize - 1].x;

		// Ok, divide it between everyone
		start.left = build(0, median);

		// If it's really small, it may not have a right branch..
		if (median != start.max)
			start.right = build(median + 1, start.max);
	}

	node *build(const point min, const point max) {

		node * const n = newnode();

//		if (min > max)
//			abort();

		n->min = min;
		n->max = max;

		const u32 lower = binarynextx(xtmparray, min);
		const u32 upper = binarynextx(xtmparray, max + 1);

		// Quick check: if nothing below me, no need to create anything below
		if (lower == upper) {
			return n;
		}

		// If no kids, create the array; otherwise, recurse
		if (min == max) {

			n->ypoints.reserve(upper - lower);
			n->ypoints.insert(n->ypoints.end(),
				xtmparray.begin() + lower,
				xtmparray.begin() + upper);

			std::sort(n->ypoints.begin(), n->ypoints.end());
		} else {
			const u32 median = (min + max) / 2;

			n->left = build(min, median);
			n->right = build(median + 1, max);

			// For faster builds, we merge our kids' arrays into ours
			mergekids(n->ypoints, n->left, n->right);
		}

		return n;
	}

	void nuke() {
		nuke(start.left);
		nuke(start.right);
	}

	void nuke(node * const n) {
		if (!n)
			return;

		nuke(n->left);
		nuke(n->right);

		if (!ispooled(n))
			delete n;
	}

	void findnodes(const node * const n, const point xmin, const point xmax,
			std::vector<const node *> &list) const {
		if (!n)
			return;

		// Fast outs
		if (xmin > n->max)
			return;
		if (xmax < n->min)
			return;
		if (!n->ypoints.size())
			return;

		fetch(n->left);
		fetch(n->right);

		if (xmin <= n->min && xmax >= n->max) {
			list.push_back(n);
			return;
		}

		findnodes(n->left, xmin, xmax, list);
		findnodes(n->right, xmin, xmax, list);
	}

	void mergekids(std::vector<pty> &arr, node * const __restrict__ left,
			node * const __restrict__ right) const {

		u32 l, r;
		const u32 lmax = left->ypoints.size();
		const u32 rmax = right->ypoints.size();

		arr.reserve(lmax + rmax);

		for (l = 0, r = 0; l < lmax || r < rmax;) {
			// Special cases first: if one array is out
			if (l == lmax) {
				arr.insert(arr.end(), right->ypoints.begin() + r,
					right->ypoints.end());
				break;
			} else if (r == rmax) {
				arr.insert(arr.end(), left->ypoints.begin() + l,
					left->ypoints.end());
				break;
			} else {
				if (left->ypoints[l].y <= right->ypoints[r].y) {
					arr.push_back(left->ypoints[l]);
					l++;
				} else {
					arr.push_back(right->ypoints[r]);
					r++;
				}
			}
		}
	}

	// A memory pool for nodes, to save on the housekeeping overhead,
	// and hopefully gain a bit in cache advantages.
	void initpool(const u32 amount) {
		// If we have N points, the likely amount of nodes is 2N - 1.
		poolcount = amount*2;
		poolgiven = 0;

		pool = new node[poolcount];
	}

	node * newnode() {
		if (poolgiven < poolcount) {
			node * const out = &pool[poolgiven];
			poolgiven++;
			return out;
		} else {
			return new node;
		}
	}

	bool ispooled(const node * const n) const {
		if (n >= &pool[0] && n <= &pool[poolcount - 1])
			return true;
		return false;
	}

	void pswap(point & __restrict__ a, point & __restrict__ b) const {
		point tmp = a;
		a = b;
		b = tmp;
	}

	std::vector<ptx> xtmparray;
	std::vector<pty> ytmparray;
	u32 totalsize;

	node start;
	node *pool;
	u32 poolcount;
	u32 poolgiven;

	u32 mainreserve, resultreserve;
	bool init;
};

#endif
