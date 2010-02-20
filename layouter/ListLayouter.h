/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#ifndef _LIST_LAYOUTER_H
#define _LIST_LAYOUTER_H

#include <Point.h>

#include "Layouter.h"
#include "ItemList.h"


class Item;
class ItemView;

class ListLayouter : public Layouter {
public:
						ListLayouter(ItemView* parentItemView, float verticalSpacing, uint32 layouterIndex);
	virtual				~ListLayouter();
	virtual	void		AddItem(Item* item, bool updateNow = false);
	virtual void		RemoveAllItems();
	virtual void		LayoutAllItems();
	virtual SpatialCache* GetSpatialCache() const {return fSpatialCache;};

protected:
	const	uint32		fLayouterIndex;
	float				fVerticalSpacing;
	ItemList			fItems;
	
	ItemView*			fItemView;
	SpatialCache*		fSpatialCache;
};

#endif	// _LIST_LAYOUTER_H
