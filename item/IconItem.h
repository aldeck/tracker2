/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ICON_ITEM_H
#define _ICON_ITEM_H

#include "Item.h"

class BBitmap;

class ItemView;

class IconItem : public Item {
public:
					IconItem(ItemView* parentItemView);
	virtual			~IconItem ();

	virtual	void	Draw();
	virtual	BRect	Frame() const;

protected:
	void			_Load();

	BBitmap*		fBitmap;
	static int		sIconIndex;
};

#endif /* _ICON_ITEM_H */
