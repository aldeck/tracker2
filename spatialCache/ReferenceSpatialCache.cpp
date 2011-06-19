/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "ReferenceSpatialCache.h"

#include "Item.h"
#include "ItemView.h"

#include <OS.h>	// system_time

#include <stdio.h>


ReferenceSpatialCache::ReferenceSpatialCache(ItemView* parentItemView)
	:
	fItemView(parentItemView)
{

}


ReferenceSpatialCache::~ReferenceSpatialCache()
{

}


void
ReferenceSpatialCache::AddItem(Item* item)
{
	fItems.insert(item);
}


void
ReferenceSpatialCache::RemoveItem(Item* item)
{
	fItems.erase(item);
}


void
ReferenceSpatialCache::RemoveAllItems()
{
	fItems.clear();
}


void
ReferenceSpatialCache::ItemChanged(Item* item)
{
}


void
ReferenceSpatialCache::AllItemsChanged(Item* item)
{	
}



Item*
ReferenceSpatialCache::FindUpperLeftItem() const
{
	if (fItems.empty())
		return NULL;


	ItemSet::const_iterator it = fItems.begin();
	BPoint min((*it)->Frame().LeftTop());	//todo use item::location
	Item* res = (*it);

	for(; it != fItems.end(); it++) {
		BPoint candidateMin((*it)->Frame().LeftTop());
		if (candidateMin.x <= min.x && candidateMin.y <= min.y) {
			res = (*it);
			min = candidateMin;
		}
	}
	return res;
}


ItemSet
ReferenceSpatialCache::FindIntersectingItems(const BPoint &point) const
{
	ItemSet res;
	ItemSet::const_iterator it = fItems.begin();
	for(; it != fItems.end(); it++) {
		if ((*it)->Frame().Contains(point))
			res.insert(*it);
	}
	return res;
}


ItemSet
ReferenceSpatialCache::FindIntersectingItems(const BRect &rect) const
{
	printf("ReferenceSpatialCache::FindIntersectingItems()");
	bigtime_t startTime = system_time();	
	ItemSet res;
	ItemSet::const_iterator it = fItems.begin();
	for(; it != fItems.end(); it++) {
		if ((*it)->Frame().Intersects(rect))
			res.insert(*it);
	}
		
	bigtime_t time = system_time() - startTime;
	printf(" %lluµs\n", time);
	return res;
}


BRect
ReferenceSpatialCache::ItemsExtent() const
{
	BRect extent(LONG_MAX, LONG_MAX, LONG_MIN, LONG_MIN);
	ItemSet::const_iterator it = fItems.begin();
	for(; it != fItems.end(); it++) {
		extent = extent | (*it)->Frame();
	}
	return extent;
}
