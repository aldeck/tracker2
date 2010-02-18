/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _HIGH_LEVEL_QUERY_EVENT_H
#define _HIGH_LEVEL_QUERY_EVENT_H


class HighLevelQueryEvent {
public:
					HighLevelQueryEvent(uint32 what, node_ref& nodeRef, entry_ref& entryRef)
						:
						what(_what),
						nodeRef(_nodeRef),
						entryRef(_entryRef)
					{};

	uint32			what;
	node_ref&		nodeRef;
	entry_ref&		entryRef;
};

#endif	// _HIGH_LEVEL_QUERY_EVENT_H
