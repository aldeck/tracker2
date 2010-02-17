/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "IconCache.h"

#include <Bitmap.h>
#include <MimeType.h>
#include <IconUtils.h>

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
IconCache::GetIcon(const char* name) //todo const
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
			printf("IconCache::GetIcon error, couldn't retrieve icon for name = '%s'\n", name);
			return NULL;
		}
	}
}
