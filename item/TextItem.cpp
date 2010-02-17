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
	fText(text)
{
	BString truncatedString;
	if (truncate) {
		truncatedString = fText;
		float width = 128;//rect.Width() - 2 * kTextMargin + 2
		parentItemView->TruncateString(&truncatedString, B_TRUNCATE_END,
			width);
		fText = truncatedString.String();
	}
	fCachedBounds.Set(0, 0, 8 + fParentItemView->StringWidth(fText), 16);
}


TextItem::~TextItem()
{
}


BRect
TextItem::Frame() const
{
	BRect bounds(fCachedBounds);	//unnecessary object creation
	return bounds.OffsetBySelf(fPositions[fParentItemView->CurrentLayouterIndex()]);
}


void
TextItem::Draw()
{
	BPoint position(fPositions[fParentItemView->CurrentLayouterIndex()]);
	position.x += 4;
	position.y += 12;
	fParentItemView->DrawString(fText.String(), position);
}
