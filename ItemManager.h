/*
 * Copyright 2010, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ITEM_MANAGER_H
#define _ITEM_MANAGER_H


#include "ItemEventListener.h"

#include <set>

class BPoint;
class Item;
class SpatialCache;


class ItemManager : public ItemEventListener {
public:
	virtual				~ItemManager();

	virtual	void		ItemAdded(Item* item) = 0;
	virtual	void		ItemRemoved(Item* item) = 0;
	virtual	void		ItemChanged(Item* item) = 0;

};

#endif	// _ITEM_MANAGER_H
