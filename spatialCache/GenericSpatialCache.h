/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _GENERIC_SPATIAL_CACHE_H
#define _GENERIC_SPATIAL_CACHE_H

#include "SpatialCache.h"
#include "ItemList.h"

#include <vector>


class Item;
class ItemView;


class GenericSpatialCache : public SpatialCache {
public:
						GenericSpatialCache(ItemView* parentItemView);
	virtual				~GenericSpatialCache();

	virtual	void		AddItem(Item* item);
	virtual	void		RemoveItem(Item* item);
	virtual	void		RemoveAllItems();
	virtual void		_UpdateAllItems(); // test

	virtual	Item*		FindUpperLeftItem() const;
	virtual	ItemSet		FindIntersectingItems(const BPoint& point) const;
	virtual	ItemSet		FindIntersectingItems(const BRect& rect) const;

	virtual	BRect		ItemsExtent() const;

protected:
	inline uint32		_Hash(int x, int y) const;	
	inline int			_Discretize(float x) const;	

private:
	ItemSet				fItems;
	ItemView*			fItemView;
	typedef std::vector<Item*> ItemVector;
	ItemVector**		fTable;
	uint32				fTableSize;
	const float			fCellSize;
	const float			fInverseCellSize;	
};


inline uint32
GenericSpatialCache::_Hash(int x, int y) const
{
	return ((x * 73856093) ^ (y * 19349663)) % fTableSize;
}


inline int				
GenericSpatialCache::_Discretize(float x) const
{
	return (int) floor(x * fInverseCellSize);
}


#endif	// __GENERIC_SPATIAL_CACHE_H
