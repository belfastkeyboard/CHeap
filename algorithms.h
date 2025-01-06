#pragma once

#include "internals/iter.h"
#include "internals/comp.h"


void sort(Iter *begin,
          Iter *end,
          Comp comp);
