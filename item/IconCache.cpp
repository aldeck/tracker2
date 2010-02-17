/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "IconCache.h"

#include <Bitmap.h>
#include <IconUtils.h>
#include <MimeType.h>
#include <NodeInfo.h>



IconCache* IconCache::sInstance = NULL;

IconCache::IconCache()
{
}


IconCache::~IconCache()
{
}


IconCache*
IconCache::GetInstance()
{
	if (sInstance == NULL)
		sInstance = new IconCache();
	return sInstance;
}


BBitmap*
IconCache::GetIcon(const entry_ref& ref)
{
	BBitmap* bitmap = new BBitmap(BRect(0, 0, 31, 31), 0, B_RGBA32);
	status_t status = BNodeInfo::GetTrackerIcon(&ref, bitmap, B_LARGE_ICON);
	
	if (status != B_OK) {
		printf("IconCache::GetIcon error, couldn't retrieve icon for ref = '%s'\n", ref.name);
		delete bitmap;
		return NULL;
	}
	
	return bitmap;
}


BBitmap*
IconCache::GetIconFromType(const char* name) //todo const
{
	BitmapMap::iterator it = fBitmaps.find(name);
	if (it != fBitmaps.end())
		return (*it).second;
	else {
		BMimeType type(name);
		uint8* vectorData = NULL;
		size_t size = 0;
		if (type.GetIcon(&vectorData, &size) == B_OK && size != 0) {
			BBitmap* bitmap = new BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);
			BIconUtils::GetVectorIcon(vectorData, size, bitmap);
			fBitmaps.insert(BitmapMap::value_type(name, bitmap));
			return bitmap;
		} else {
			//printf("IconCache::GetIcon error, couldn't retrieve icon for name = '%s'\n", name);
			return NULL;
		}
	}
}
