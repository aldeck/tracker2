/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ICON_CACHE_H
#define _ICON_CACHE_H

#include <map>

class BBitmap;
class BFile;
struct entry_ref;

// TODO maybe use some refcounted Icon class that owns vector data / draws the bitmap


class IconCache {
public:
						IconCache();
	virtual				~IconCache();

	virtual	BBitmap*	GetIconFromType(const char* name);
	virtual	BBitmap*	GetIcon(const entry_ref& ref);

	static IconCache*	GetInstance();

protected:
	static IconCache*	sInstance;

	typedef std::map<const char *, BBitmap*> BitmapMap;
	BitmapMap			fBitmaps;
};

#endif /* _ICON_CACHE_H */
