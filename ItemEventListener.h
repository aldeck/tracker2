/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ITEM_EVENT_LISTENER_H
#define _ITEM_EVENT_LISTENER_H


class ItemEventListener {
public:
	virtual				~ItemEventListener() {};

	virtual	void		ItemAdded(Item* item) = 0;
	virtual	void		ItemRemoved(Item* item) = 0;
	virtual	void		ItemChanged(Item* item) = 0;
};

#endif	// _ITEM_EVENT_LISTENER_H
