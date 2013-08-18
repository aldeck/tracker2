/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "IconItem.h"

#include <Bitmap.h>
#include <MimeType.h>
#include <IconUtils.h>
#include <TranslationUtils.h>

#include "ItemView.h"
#include "IconCache.h"


//int IconItem::sIconIndex = 0;

IconItem::IconItem(ItemView* parentItemView, const entry_ref& ref)
	:
	Item(parentItemView),
	fEntryRef(ref)
{
	
	// either a type icon, disk icon or custom exe icon...
	
	fBitmap = NULL; //new BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);
	fThumbnailBitmap = NULL;
	/*BMessage types;
	BMimeType::GetInstalledTypes(&types);

	for (int32 index = sIconIndex; ; index++) {
	//int32 index = sIconIndex ;
		const char* type;
		if (types.FindString("types", index % 256, &type) != B_OK)
			break;

		BMimeType mimeType(type);
		uint8* vectorData = NULL;
		size_t size = 0;

		if (mimeType.GetIcon(&vectorData, &size) != B_OK || size == 0)
			continue;

		BIconUtils::GetVectorIcon(vectorData, size, fBitmap);
		break;
	}
	sIconIndex++;*/
	//_Load();
}


IconItem::~IconItem()
{
	delete fBitmap;
}


void
IconItem::_Load()
{
	if (fBitmap == NULL) {	
		//printf("IconItem::_Load()\n");	
		/*BMessage types;
		BMimeType::GetInstalledTypes(&types);
	
		sIconIndex++;
		
		const char* type;
		if (types.FindString("types", sIconIndex % 256, &type) != B_OK) {
			printf("can't find type %d\n", sIconIndex);
			return;
		}
			
		fBitmap = IconCache::GetInstance()->GetIconFromType(type);*/
		
		fBitmap = IconCache::GetInstance()->GetIcon(fEntryRef);		//use BIconutils
		//fBitmap = IconCache::GetInstance()->GetIconFromType("application/octet-stream");
	}

	if (fThumbnailBitmap == NULL) {
	    BBitmap* bigBitmap = BTranslationUtils::GetBitmap(&fEntryRef);
		if(bigBitmap == NULL || !bigBitmap->IsValid()) {
			return;
		}

		BRect thumbFrame = (0, 0, 32, 32);
		fThumbnailBitmap = new BBitmap(thumbFrame,
			B_BITMAP_ACCEPTS_VIEWS, B_RGB32);
		BView* thumbView = new BView(thumbFrame, "thumbnail",
			B_FOLLOW_NONE, B_WILL_DRAW);
		fThumbnailBitmap->AddChild(thumbView);

		thumbView->LockLooper();
		thumbView->DrawBitmap(bigBitmap, thumbFrame);
		thumbView->Sync();
		thumbView->UnlockLooper();

		fThumbnailBitmap->RemoveChild(thumbView);
		delete thumbView;
		delete bigBitmap;
	}
}


BRect
IconItem::Frame() const
{
	if (fBitmap == NULL)
		return BRect(0, 0, 32, 32).OffsetToSelf(Position());
	else
		return BRect(fBitmap->Bounds()).OffsetToSelf(Position());
}


void
IconItem::Draw()
{
	//lazy load
	if (fBitmap == NULL) {		
		_Load();
	}

	if(fThumbnailBitmap != NULL && fThumbnailBitmap->IsValid()) {
		fParentItemView->DrawBitmap(fThumbnailBitmap, Position());
	} else {
	    fParentItemView->DrawBitmap(fBitmap, Position());
	}
	Item::Draw();
}
