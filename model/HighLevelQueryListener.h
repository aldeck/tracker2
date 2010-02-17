/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _HIGH_LEVEL_QUERY_LISTENER_H
#define _HIGH_LEVEL_QUERY_LISTENER_H

struct entry_ref;
struct node_ref;

class HighLevelQueryListener {
public:
	virtual				~HighLevelQueryListener() {};

	virtual	void		EntryAdded(const node_ref& nodeRef, const entry_ref& entryRef) = 0;
	virtual	void		EntryRemoved(const node_ref& nodeRef, const entry_ref& entryRef) = 0;
	virtual	void		EntryChanged(const node_ref& nodeRef, const entry_ref& entryRef) = 0;
};

#endif	// _HIGH_LEVEL_QUERY_LISTENER_H
