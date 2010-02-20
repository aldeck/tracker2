/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _SPATIAL_CACHE_H
#define _SPATIAL_CACHE_H

#include <Rect.h>
#include <Point.h>

#include "ItemList.h"

class BPose;
class BPoseView;


class SpatialCache {
public:
	virtual				~SpatialCache();

	virtual	void		AddItem(Item* item) = 0;
	virtual	void		RemoveItem(Item* item) = 0;
	virtual	void		RemoveAllItems() = 0;
	virtual void		_UpdateAllItems() {}; // test

	virtual	Item*		FindUpperLeftItem() const = 0;
	virtual	ItemSet		FindIntersectingItems(const BPoint& point) const = 0;
	virtual	ItemSet		FindIntersectingItems(const BRect& rect) const = 0;

	virtual	BRect		ItemsExtent() const = 0;
};


#endif	// _SPATIAL_CACHE_H
