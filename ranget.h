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
#include <iostream>
#include <algorithm>
#include <vector>

template <class point, class data> class rangetree {

public:
	rangetree(u32 estimatedTotal = 1000, u32 estimatedResult = 100):
		mainreserve(estimatedTotal), resultreserve(estimatedResult), init(false) {
		tmparray.reserve(mainreserve);
	}

	int addPoint(point x, point y, data * const ptr) {
		if (init)
			return 1;

		return 0;
	}

	void finalize() {
	}

	u32 count(point xmin, point xmax, point ymin, point ymax) {
	}

	std::vector *search(point xmin, point xmax, point ymin, point ymax) {
		std::vector<data *> * const res = new std::vector<data *>;
	}

private:
	struct pt {
		point x, y;
		data * ptr;
	};


	std::vector<pt> tmparray;
	u32 mainreserve, resultreserve;
	bool init;
};

#endif
