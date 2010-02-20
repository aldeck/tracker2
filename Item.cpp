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
	fParentItemView(parentItemView)
{
	//TODO: clean that
	for (int i = 0; i < 10; i++)
		fPositions.push_back(BPoint(0, 0));

	//for (int i = 0; i < 10; i++)
	//	fOrderings.push_back(0);
}


Item::~Item()
{
}


void
Item::MouseDown(BPoint point)
{
	printf("Item::MouseDown this=%p ", this);
	point.PrintToStream();
}


void
Item::ContextDown(BPoint point)
{
	printf("Item::ContextDown this=%p ", this);
	point.PrintToStream();
}


uint32
Item::Rank(uint32 space) const
{
	return fRanks[space];
}


void
Item::SetRank(uint32 position, uint32 space)
{
	fRanks[space] = position;
}


BPoint
Item::Position(uint32 space) const
{
	return fPositions[space];
}


void
Item::SetPosition(const BPoint& position, uint32 space)
{
	fPositions[space] = position;
}


/*void
Item::SetOrdering(uint32 ordering, int layouterIndex)
{
	fOrderings[layouterIndex] = ordering;
}*/
