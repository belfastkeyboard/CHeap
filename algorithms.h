#pragma once

#include "iter.h"
#include "internals/comp.h"


void sort(Iter begin,
          Iter end,
          Comp comp);

void shuffle(Iter begin,
             Iter end);
