/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "TextItem.h"

#include "ItemView.h"


TextItem::TextItem(ItemView* parentItemView, const BString& text)
	:
	Item(parentItemView),	
	fLoaded(false),
	fText(text)
{
	fCachedBounds.Set(0, 0, 8 + fParentItemView->StringWidth(fText), 16);	
}


TextItem::~TextItem()
{
}


void
TextItem::_Load()
{
	/*if (!fLoaded) {	
		BString truncatedString;	
		truncatedString = fText;
		float width = 128;//rect.Width() - 2 * kTextMargin + 2
		parentItemView->TruncateString(&truncatedString, B_TRUNCATE_END,
			width);
		fText = truncatedString.String(); // uhu?	
		fCachedBounds.Set(0, 0, 8 + fParentItemView->StringWidth(fText), 16);
	}*/
}


BRect
TextItem::Frame() const
{
	BRect bounds(fCachedBounds);	//unnecessary object creation?
	return bounds.OffsetBySelf(Position());
}


void
TextItem::Draw()
{
	BPoint position(Position());
	position.x += 4;
	position.y += 12;
	fParentItemView->DrawString(fText.String(), position);
	Item::Draw();
}
