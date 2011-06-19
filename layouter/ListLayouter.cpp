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
#include "ReferenceSpatialCache.h"

#include <OS.h> // system_time

#include <stdio.h>


ListLayouter::ListLayouter(uint32 layouterIndex, ItemView* parentItemView,
	float verticalSpacing)
	:
	fLayouterIndex(layouterIndex),
	fVerticalSpacing(verticalSpacing),
	fItemView(parentItemView)
{
	fSpatialCache = new ReferenceSpatialCache(parentItemView);
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
	bigtime_t startTime = system_time();	
	printf("ListLayouter::LayoutAllItems()");
	BPoint pin(20, 20);
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {		
		(*it)->SetRelativePosition(pin);
		fSpatialCache->ItemChanged(*it);
		pin.y += (*it)->Frame().Height() + 5; //fVerticalSpacing;
	}
	
	bigtime_t time = system_time() - startTime;
	printf(" %lluµs\n", time);
}
