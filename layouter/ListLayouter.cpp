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

#include <stdio.h>


ListLayouter::ListLayouter(uint32 layouterIndex, ItemView* parentItemView,
	float verticalSpacing)
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
ListLayouter::AddItem(Item* item, bool updateNow)
{
	fItems.push_back(item);
	fSpatialCache->AddItem(item);
}


void
ListLayouter::RemoveAllItems()
{
	printf("ListLayouter::RemoveAllItems()\n");
	fSpatialCache->RemoveAllItems();
	fItems.clear();
}


void
ListLayouter::LayoutAllItems()
{
	printf("ListLayouter::LayoutAllItems()\n");
	BPoint pin(20, 20);
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		fSpatialCache->RemoveItem(*it);
		(*it)->SetPosition(pin, fLayouterIndex);		
		fSpatialCache->AddItem(*it);
		pin.y += (*it)->Frame().Height() + 5; //fVerticalSpacing;
	}
}
