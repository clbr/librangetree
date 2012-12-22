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

	int add(point x, point y, data * const ptr) {

		if (init)
			return 1;

		ptx px;
		pty py;

		px.x = x;
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
	}

	u32 count(point xmin, point xmax, point ymin, point ymax) {

		if (!init)
			return 0;

		u32 sum = 0;

		return sum;
	}

	std::vector<data *> *search(point xmin, point xmax, point ymin, point ymax) {

		if (!init)
			return NULL;

		std::vector<data *> * const res = new std::vector<data *>;
		res->reserve(resultreserve);

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
		point y;
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

		std::vector<pty> ypoints;
	};


	std::vector<ptx> xtmparray;
	std::vector<pty> ytmparray;
	u32 mainreserve, resultreserve;
	bool init;
};

#endif
