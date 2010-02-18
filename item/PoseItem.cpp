/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "PoseItem.h"

#include "IconItem.h"
#include "TextItem.h"
#include "ItemView.h"

#include <Entry.h>

PoseItem::PoseItem(ItemView* parentItemView, const entry_ref& ref)
	:
	Item(parentItemView),
	fIconItem(NULL),
	fNameItem(NULL)
{	

	
	fIconItem = new IconItem(parentItemView, ref);
	
	/*BEntry entry(&ref);
	char name[B_FILE_NAME_LENGTH];
	entry.GetName(name);*/
	fNameItem = new TextItem(parentItemView, ref.name);

	//parentItemView->AddItem(fIconItem);
	//parentItemView->AddItem(fNameItem);
}


PoseItem::~PoseItem()
{
}


bool
PoseItem::Less(const Item* other) const
{
	return fNameItem->Text() < dynamic_cast<const TextItem*>(other)->Text();
}


/*uint32
PoseItem::Rank(uint32 sorting)
{
}


void
PoseItem::SetRank(uint32 sorting)
{
}*/


void
PoseItem::SetPosition(const BPoint& position, uint32 space)
{
	//BPoint iconOffset(fIconItem->Position(space) - fPositions[space]);
	//BPoint nameOffset(fNameItem->Position(space) - fPositions[space]);
	Item::SetPosition(position, space);

	BPoint iconOffset;
	BPoint nameOffset(32, 16);
	fIconItem->SetPosition(position + iconOffset, space);
	fNameItem->SetPosition(position + nameOffset, space);
}


BRect
PoseItem::Frame() const
{
	return fIconItem->Frame() | fNameItem->Frame();
}


void
PoseItem::Draw()
{
	fIconItem->Draw();
	fNameItem->Draw();
}
