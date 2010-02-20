/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#ifndef _GRID_LAYOUTER_H
#define _GRID_LAYOUTER_H

#include <Point.h>

#include "Layouter.h"
#include "ItemList.h"


class Item;
class ItemView;
class SpatialCache;

class GridLayouter : public Layouter {
public:
						GridLayouter(ItemView* parentItemView,
							const BPoint& spacing, uint32 layouterIndex);
	virtual				~GridLayouter();
	virtual	void		AddItem(Item* item, bool updateNow = false);
	virtual void		RemoveAllItems();
	virtual void		LayoutAllItems();
	virtual SpatialCache* GetSpatialCache() const {return fSpatialCache;};

protected:
	const	uint32		fLayouterIndex;
	BPoint				fSpacing;
	ItemList			fItems;
	
	ItemView*			fItemView;
	SpatialCache*		fSpatialCache;
};

#endif	// _GRID_LAYOUTER_H
