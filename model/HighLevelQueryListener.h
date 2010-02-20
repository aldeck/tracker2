/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _HIGH_LEVEL_QUERY_LISTENER_H
#define _HIGH_LEVEL_QUERY_LISTENER_H

#include <SupportDefs.h>

class HighLevelQueryResult;

class HighLevelQueryListener {
public:
	virtual				~HighLevelQueryListener() {};

	virtual	void		HighLevelQueryEventReceived(uint32 code, /*const*/ HighLevelQueryResult* result) = 0;
};

#endif	// _HIGH_LEVEL_QUERY_LISTENER_H
