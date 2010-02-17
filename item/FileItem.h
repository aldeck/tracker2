/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _FILE_ITEM_H
#define _FILE_ITEM_H

#include "Item.h"

#include <String.h>

class BBitmap;
struct entry_ref;

class ItemView;

class FileItem : public Item {
public:
					FileItem(ItemView* parentItemView, const entry_ref* ref);
	virtual			~FileItem();

	virtual	void	Draw();
	virtual	BRect	Frame() const;

protected:
	BBitmap*		fBitmap;
	BString			fName;
};

#endif /* _FILE_ITEM_H */
