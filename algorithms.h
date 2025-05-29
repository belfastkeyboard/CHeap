#pragma once

#include "iter.h"

typedef int (*Comp)(const void *a, const void *b);

void sort(Iter begin, Iter end, Comp comp);

void shuffle(Iter begin, Iter end);
