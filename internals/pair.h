#pragma once

#ifndef CHEAP_KEY_VALUE_PAIR_DEFINED
typedef struct PairKV
{
	const void *key;
	void       *value;
} PairKV;
#define CHEAP_KEY_VALUE_PAIR_DEFINED
#endif
