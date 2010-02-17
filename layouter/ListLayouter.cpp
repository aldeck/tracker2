/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "ListLayouter.h"

#include "Item.h"
#include "GenericSpatialCache.h"


ListLayouter::ListLayouter(ItemView* parentItemView, float verticalSpacing, uint32 layouterIndex)
	:
	fLayouterIndex(layouterIndex),
	fVerticalSpacing(verticalSpacing),
	fItemView(parentItemView)
{
	fSpatialCache = new GenericSpatialCache(parentItemView);
}


ListLayouter::~ListLayouter()
{
	delete fSpatialCache;
}


void
ListLayouter::AddItem(Item* item, bool updateNow = false)
{
	fItems.push_back(item);
	fSpatialCache->AddItem(item);
}


void
ListLayouter::LayoutAllItems()
{
	BPoint pin(20, 20);
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		fSpatialCache->RemoveItem(*it);
		(*it)->SetPosition(pin, fLayouterIndex);
		fSpatialCache->AddItem(*it);
		pin.y += fVerticalSpacing;
	}
}
