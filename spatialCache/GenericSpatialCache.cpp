/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "GenericSpatialCache.h"

#include "Item.h"
#include "ItemView.h"

#include <stdio.h>


GenericSpatialCache::GenericSpatialCache(ItemView* parentItemView)
	:
	fItemView(parentItemView),
	fCellSize(512.0),
	fInverseCellSize(1.0 / fCellSize)	
{
	fTableSize = 1024;
	fTable = new (std::nothrow) ItemVector* [fTableSize];
	if (fTable == NULL) {
		printf("GenericSpatialCache::GenericSpatialCache failed creating cell table, no memory\n");	
		// todo, do something
		return;
	}
	
	for (int i = 0; i < fTableSize; i++) {
		fTable[i] = NULL;	
	}
}


GenericSpatialCache::~GenericSpatialCache()
{
	for (int i = 0; i < fTableSize; i++) {
		if (fTable[i] != NULL) {
			delete fTable[i];	
		}	
	}
	delete [] fTable;
}


void
GenericSpatialCache::AddItem(Item* item)
{
	//bigtime_t startTime = system_time();
	std::pair<ItemSet::iterator, bool> res = fItems.insert(item);
	if (!res.second) {
		printf("GenericSpatialCache::AddItem already added %p\n", item);
		return;	
	}
	
	BRect frame(item->Frame());

	for(int x = _Discretize(frame.left); x <= _Discretize(frame.right); x++) {
		for(int y = _Discretize(frame.top); y <= _Discretize(frame.bottom); y++) {
				
			const uint32 h = _Hash(x, y);
			
			if (fTable[h] == NULL) {
				fTable[h] = new (std::nothrow) ItemList();
				if (fTable[h] == NULL) {
					printf("GenericSpatialCache::AddItem failed creating cell for item, no memory %p\n", item);	
					fItems.erase(item);
					// todo, do something
					return;
				}				
			}
			
			fTable[h]->push_back(item);
			//printf("GenericSpatialCache::AddItem ok %p, cell %d (%d, %d)\n", item, h, x, y);			
		}
	}
	
	//float ftime = (float) (system_time() - startTime);
	//printf("GenericSpatialCache::AddItem() %f µs\n", ftime);	
}


void
GenericSpatialCache::RemoveItem(Item* item)
{
	//bigtime_t startTime = system_time();
	size_t erasedCount = fItems.erase(item);
	if (erasedCount == 0) {
		printf("GenericSpatialCache::RemoveItem, already removed or absent %p\n", item);
		return;	
	}
	
	BRect frame(item->Frame());
	
	for(int x = _Discretize(frame.left); x <= _Discretize(frame.right); x++) {
		for(int y = _Discretize(frame.top); y <= _Discretize(frame.bottom); y++) {
				
			const uint32 h = _Hash(x, y);
			
			// empty cell
			if (fTable[h] == NULL)
				continue;
			
			ItemList::iterator it = fTable[h]->begin();
			for (; it != fTable[h]->end(); it++) {
				if ((*it) == item) {
					fTable[h]->erase(it);
					//printf("GenericSpatialCache::RemoveItem ok %p, cell %d (%d, %d)\n", item, h, x, y);	
					break;
				}
			}
			
			if (fTable[h]->empty()) {
				delete fTable[h];
				fTable[h] = NULL;
			}			
		}
	}
	
	//float ftime = (float) (system_time() - startTime);
	//printf("GenericSpatialCache::RemoveItem() %f µs\n", ftime);
}


void
GenericSpatialCache::RemoveAllItems()
{
	printf("GenericSpatialCache::RemoveAllItems()\n");
	
	for (int i = 0; i < fTableSize; i++) {
		if (fTable[i] != NULL) {
			delete fTable[i];
			fTable[i] = NULL;
		}	
	}
	fItems.clear();
}


void
GenericSpatialCache::ItemChanged(Item* item)
{
	//TODO
}


void
GenericSpatialCache::AllItemsChanged(Item* item)
 // TODO ca peut etre aussi la forme de l'item qui change
{
	bigtime_t startTime = system_time();
	
	/*for (int i = 0; i < fTableSize; i++) {
		if (fTable[i] != NULL) {
			ItemList::iterator it = fTable[i]->begin();
			for (; it != fTable[i]->end(); it++) {
				if ((*it) == item) {
					// TODO check if hash has changed.. mmh mais y'en a plusieurs par item
					
					// peut etre utiliser des listes chainées si on veut optimiser
					// les ajout/supressions, surtout qu'on utilise que la lecture 
					// sequentielle
					break;
				}
			}
		}	
	}*/
	
		
	bigtime_t time = system_time() - startTime;
	float ftime = (float)time / 1000.0f;
	printf("GenericSpatialCache::AllItemsMoved() %f ms\n", ftime);
}


void
GenericSpatialCache::_UpdateAllItems()
{
	// todo on pourrait verifier si l'item a bougé avec discretize pour accelerer l'update
	// enfin bon cette fonction ne devrait pas servir (c'est juste du bench)
	bigtime_t startTime = system_time();
	
	for (int i = 0; i < fTableSize; i++) {
		if (fTable[i] != NULL) {
			delete fTable[i];
			fTable[i] = NULL;	
		}	
	}
	
	ItemSet::const_iterator it = fItems.begin();
	/*for(; it != fItems.end(); it++) {
		RemoveItem(*it);		
	}*/
	
	//it = fItems.begin();
	for(; it != fItems.end(); it++) {		
		AddItem(*it);
	}
	
	bigtime_t time = system_time() - startTime;
	float ftime = (float)time / 1000.0f;
	printf("GenericSpatialCache::_UpdateAllItems() %f ms\n", ftime);
}


Item*
GenericSpatialCache::FindUpperLeftItem() const
{
	// todo ask for a search rectangle
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
GenericSpatialCache::FindIntersectingItems(const BPoint &point) const
{
	ItemSet res;
	const uint32 h = _Hash(_Discretize(point.x), _Discretize(point.y));
	
	// empty cell
	if (fTable[h] == NULL)
		return res;
		
	ItemList::const_iterator it = fTable[h]->begin();
	for (; it != fTable[h]->end(); it++) {
		if ((*it)->Frame().Contains(point))
			res.insert(*it);
	}

	return res;
}


ItemSet
GenericSpatialCache::FindIntersectingItems(const BRect &rect) const
{
	//printf("GenericSpatialCache::FindIntersectingItems rect:");	
	//rect.PrintToStream();
	//bigtime_t startTime = system_time();	
	ItemSet res;
	for(int x = _Discretize(rect.left); x <= _Discretize(rect.right); x++) {
		for(int y = _Discretize(rect.top); y <= _Discretize(rect.bottom); y++) {
				
			const uint32 h = _Hash(x, y);
			
			// empty cell
			if (fTable[h] == NULL)
				continue;
			
			ItemList::iterator it = fTable[h]->begin();
			for (; it != fTable[h]->end(); it++) {
				if ((*it)->Frame().Intersects(rect))
					res.insert(*it);
			}			
		}
	}
	//float ftime = (float) (system_time() - startTime);
	//printf("GenericSpatialCache::FindIntersectingItems() found %d items in %f µs\n", res.size(), ftime);
	
	return res;
}


BRect
GenericSpatialCache::ItemsExtent() const
{
	BRect extent(LONG_MAX, LONG_MAX, LONG_MIN, LONG_MIN);
	ItemSet::const_iterator it = fItems.begin();
	for(; it != fItems.end(); it++) {
		extent = extent | (*it)->Frame();
	}
	return extent;
}
