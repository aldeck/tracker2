/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * 
 */
#ifndef _NATURAL_COMPARE_H
#define _NATURAL_COMPARE_H

#include <stdlib.h>


inline bool
IsDigit(const char c)
{
	if ((c >= 48 && c <= 57) || c == 32)
		return true;
	else
		return false;
}


//! Compares two strings naturally, as opposed to lexicographically
inline int
NaturalCompare(const char *s1, const char *s2)
{
	struct Chunk {
		int32	type;
		union {
			char*	ascii;
				// Type = 0
			int32	num;
				// Type = 1
		};
	};

	Chunk a;
	Chunk b;

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	char bufferA[len1 + 1];
	char bufferB[len2 + 1];

	uint32 i = 0;
	uint32 j = 0;

	while (true) {
		// determine type of next chunks in each string based on first char
		if (i == len1)
			a.type = -1;
		else if (IsDigit(s1[i]))
			a.type = 1;
		else
			a.type = 0;

		if (j == len2)
			b.type = -1;
		else if (IsDigit(s2[j]))
			b.type = 1;
		else
			b.type = 0;

		// check if we reached the end of either string
		if (a.type == b.type && a.type == -1)
			return 0;
		if (a.type == -1)
			return -1;
		if (b.type == -1)
			return 1;

		if (a.type != b.type) {
			// different chunk types, just compare the remaining strings
			return strcasecmp(&s1[i], &s2[j]);
		}

		// fetch the next chunk for a
		if (a.type == 0) {
			// string chunk
			int32 k = i;
			while (!IsDigit(s1[k]) && s1[k] != 0) {
				bufferA[k - i] = s1[k];
				k++;
			}
			bufferA[k - i] = 0;
			a.ascii = bufferA;
			i += k - i;
		} else {
			// number chunk
			int32 k = i;
			while (IsDigit(s1[k]) && s1[k] != 0) {
				bufferA[k - i] = s1[k];
				k++;
			}
			bufferA[k - i] = 0;
			a.ascii = bufferA;
			a.num = atoi(bufferA);
			i += k - i;
		}

		// fetch the next chunk for b
		if (b.type == 0) {
			// string chunk
			int32 k = j;
			while (!IsDigit(s2[k]) && s2[k] != 0) {
				bufferB[k - j] = s2[k];
				k++;
			}
			bufferB[k - j] = 0;
			b.ascii = bufferB;
			j += k - j;
		} else {
			// number chunk
			int32 k = j;
			while (IsDigit(s2[k]) && s2[k] != 0) {
				bufferB[k - j] = s2[k];
				k++;
			}
			bufferB[k - j] = 0;
			b.ascii = bufferB;
			b.num = atoi(bufferB);
			j += k - j;
		}

		// compare the two chunks based on their type
		if (a.type == 0) {
			// string chunks
			int stringCompareResult = strcasecmp(a.ascii, b.ascii);
			// if the chunk strings are the same, keep using natural
			// sorting for the next chunks
			if (stringCompareResult != 0)
				return stringCompareResult;
		} else {
			// number chunks
			if (a.num != b.num) {
				if (a.num < b.num)
					return -1;
				if (a.num > b.num)
					return 1;
			}
		}
	}

	return 0;
}

#endif	// _NATURAL_COMPARE_H
