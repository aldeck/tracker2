/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "GridLayouter.h"

#include "Item.h"
//#include "ReferenceSpatialCache.h"
#include "GenericSpatialCache.h"

#include <stdio.h>


GridLayouter::GridLayouter(uint32 layouterIndex, ItemView* parentItemView,
	const BPoint& spacing)
	:
	fLayouterIndex(layouterIndex),
	fSpacing(spacing),
	fItemView(parentItemView)
{
	fSpatialCache = new GenericSpatialCache(parentItemView);
}


GridLayouter::~GridLayouter()
{
	delete fSpatialCache;
}


void
GridLayouter::AddItem(Item* item, bool updateNow)
{
	fItems.push_back(item);
	fSpatialCache->AddItem(item);
}


void
GridLayouter::RemoveAllItems()
{
	printf("GridLayouter::RemoveAllItems()\n");
	fSpatialCache->RemoveAllItems();
	fItems.clear();
}


void
GridLayouter::LayoutAllItems()
{
	printf("GridLayouter::LayoutAllItems()\n");
	BPoint pin(20, 20);
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		fSpatialCache->RemoveItem(*it);
		(*it)->SetRelativePosition(pin);
		fSpatialCache->AddItem(*it);
		pin.x += (*it)->Frame().Width() + fSpacing.x;
		if (pin.x > 800) {
			pin.x = 20;
			pin.y += fSpacing.y;
		}
	}
}
