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

#include "types.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>

template <class point, class data> class rangetree {

public:
	rangetree(u32 estimatedTotal = 1000, u32 estimatedResult = 100):
		mainreserve(estimatedTotal), resultreserve(estimatedResult), init(false) {

		xtmparray.reserve(mainreserve);
		ytmparray.reserve(mainreserve);
	}

	~rangetree() {
		nuke();
	}

	int add(point x, point y, data * const ptr) {

		if (init)
			return 1;

		ptx px;
		pty py;

		px.x = py.x = x;
		py.y = y;
		px.ptr = py.ptr = ptr;

		xtmparray.push_back(px);
		ytmparray.push_back(py);

		return 0;
	}

	void finalize() {

		init = true;

		std::sort(xtmparray.begin(), xtmparray.end());
		std::sort(ytmparray.begin(), ytmparray.end());

		totalsize = ytmparray.size();

		build();
	}

	u32 count(point xmin, point xmax, point ymin, point ymax) {

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

			for (u32 i = 0; i < max; i++) {
				if (n->ypoints[i].y >= ymin &&
					n->ypoints[i].y <= ymax)
					sum++;
			}
		}

		return sum;
	}

	std::vector<data *> *search(point xmin, point xmax, point ymin, point ymax) {

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

			for (u32 i = 0; i < max; i++) {
				if (n->ypoints[i].y >= ymin &&
					n->ypoints[i].y <= ymax)
					res->push_back(n->ypoints[i].ptr);
			}
		}
		return res;
	}

private:
	struct ptx {
		point x;
		data * ptr;

		inline bool operator < (const ptx &other) const {
			if (x >= other.x)
				return false;
			return true;
		}
	};
	struct pty {
		point y, x;
		data * ptr;

		inline bool operator < (const pty &other) const {
			if (y >= other.y)
				return false;
			return true;
		}
	};
	struct node {
		node * left;
		node * right;

		point min, max;

		std::vector<pty> ypoints;

		node(): left(NULL), right(NULL), min(0), max(0) {}
	};

	void build() {
		if (totalsize < 2) {
			// Trees of a single point aren't supported
			return;
		}

		start.ypoints = ytmparray;

		const u32 medianidx = totalsize / 2;
		const point median = xtmparray[medianidx].x;

		start.min = 0;
		start.max = xtmparray[totalsize - 1].x;

		// Ok, divide it between everyone
		start.left = build(0, median);
		start.right = build(median + 1, totalsize);
	}

	node *build(const u32 min, const u32 max) {

		node * const n = new node;

		u32 i;
		const u32 ymax = totalsize;
		for (i = 0; i < ymax; i++) {
			if (ytmparray[i].x >= min &&
				ytmparray[i].x <= max) {
				n->ypoints.push_back(ytmparray[i]);
			}
		}

		n->min = min;
		n->max = max;

		// If no kids, we're done here; otherwise, recurse
		if (min != max) {
			const u32 median = (min + max) / 2;

			n->left = build(min, median);
			n->right = build(median + 1, max);
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

		delete n;
	}

	void findnodes(node * const n, const point xmin, const point xmax,
			std::vector<const node *> &list) const {
		if (!n)
			return;

		if (xmin <= n->min && xmax >= n->max) {
			list.push_back(n);
			return;
		}

		findnodes(n->left, xmin, xmax, list);
		findnodes(n->right, xmin, xmax, list);
	}

	void pswap(point &a, point &b) const {
		point tmp = a;
		a = b;
		b = tmp;
	}

	std::vector<ptx> xtmparray;
	std::vector<pty> ytmparray;
	u32 totalsize;

	node start;

	u32 mainreserve, resultreserve;
	bool init;
};

#endif
