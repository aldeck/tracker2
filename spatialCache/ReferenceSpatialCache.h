/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _REFERENCE_SPATIAL_CACHE_H
#define _REFERENCE_SPATIAL_CACHE_H

#include "SpatialCache.h"
#include "ItemList.h"


class Item;
class ItemView;


class ReferenceSpatialCache : public SpatialCache {
public:
						ReferenceSpatialCache(ItemView *parentItemView);
	virtual				~ReferenceSpatialCache();

	virtual	void		AddItem(Item* item);
	virtual	void		RemoveItem(Item* item);
	virtual	void		RemoveAllItems();

	virtual	Item*		FindUpperLeftItem() const;
	virtual	ItemSet		FindIntersectingItems(const BPoint& point) const;
	virtual	ItemSet		FindIntersectingItems(const BRect& rect) const;

	virtual	BRect		ItemsExtent() const;

private:
	ItemSet				fItems;
	ItemView*			fItemView;
};


#endif	// _REFERENCE_SPATIAL_CACHE_H
