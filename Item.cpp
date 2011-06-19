/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "Item.h"

#include "ItemView.h"

#include <stdio.h>


Item::Item(ItemView* parentItemView)
	:
	fParentItemView(parentItemView),
	fRelativePosition(0, 0),
	fParentItem(NULL)
{
}


Item::~Item()
{
}


void
Item::AttachedToParent(Item *item)
{
	fParentItem = item;
}


void
Item::AddChild(Item *item, const BPoint& relativePosition)
{
	fChildItems.push_back(item);
	item->SetRelativePosition(relativePosition);
	item->fParentItem = this;
}


void
Item::RemoveChild(Item *item)
{
	ItemVector::iterator it = fChildItems.begin();
	for (; it != fChildItems.end(); it++) {
		if (*it == item) {
			 fChildItems.erase(it);
			 (*it)->fParentItem = NULL;
			 break;
		}
	}
}


void
Item::MouseDown(BPoint point)
{
	//printf("Item::MouseDown this=%p ", this);
	//point.PrintToStream();
}


void
Item::ContextDown(BPoint point)
{
	//printf("Item::ContextDown this=%p ", this);
	//point.PrintToStream();
}


/*uint32
Item::Rank() const
{
	return fRanks[space];
}


void
Item::SetRank(uint32 rank)
{
	fRank = rank;
}*/


void
Item::Draw()
{
	ItemVector::iterator it = fChildItems.begin();
	for (; it != fChildItems.end(); it++) {
		(*it)->Draw();
	}
	//fParentItemView->StrokeRect(Frame());
}


BPoint
Item::Position() const
{
	// TODO cache this
	if (fParentItem == NULL)
		return fRelativePosition;
	else
		return fParentItem->Position() + fRelativePosition;
}


BPoint
Item::RelativePosition() const
{
	return fRelativePosition;
}


void
Item::SetRelativePosition(const BPoint& position)
{
	//printf("SetRelativePosition x%f y%f %p\n", position.x, position.y, this);
	fRelativePosition = position;
	// cache propagate to children
	/*ItemVector::iterator it = fChildItems.begin();
	for (; it != fChildItems.end(); ++i) {
		(*it)->SetPosition(fPosition
	}*/
}
