/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "FileItem.h"

#include <Bitmap.h>
#include <Entry.h>
#include <MimeType.h>
#include <IconUtils.h>

#include "IconCache.h"
#include "ItemView.h"


FileItem::FileItem(ItemView* parentItemView, const entry_ref* ref)
	:
	Item(parentItemView)
{
	fBitmap = new BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);

	BNode node(ref);
	if (node.InitCheck() == B_OK) {
		char mimeString[256]; //MIME8TYPE8SIZE
		ssize_t size = node.ReadAttr("BEOS:TYPE", B_MIME_STRING_TYPE, 0, mimeString, 256);
		if (size > 0)
			fBitmap = IconCache::GetInstance()->GetIconFromType(mimeString);
		else
			printf("FileItem::FileItem error reading mimetype, ref = '%s'", ref->name);

		BEntry entry(ref);
		char name[256]; //REF8SIZE
		entry.GetName(name);
		fName.SetTo(name);
	} else {
		printf("FileItem::FileItem error opening node, ref = '%s'", ref->name);
	}
}


FileItem::~FileItem()
{
}


BRect
FileItem::Frame() const
{
	return BRect(fBitmap->Bounds()).OffsetToSelf(fPositions[fParentItemView->CurrentLayouterIndex()]);
}


void
FileItem::Draw()
{
	fParentItemView->DrawBitmap(fBitmap, fPositions[fParentItemView->CurrentLayouterIndex()]);
}
