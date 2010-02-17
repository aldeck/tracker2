/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _TEXT_ITEM_H
#define _TEXT_ITEM_H

#include "Item.h"

#include <Rect.h>
#include <String.h>

class ItemView;

class TextItem : public Item {
public:
					TextItem(ItemView* parentItemView, const BString& text);
	virtual			~TextItem();

	virtual	void	Draw();
	virtual	BRect	Frame() const;

	const BString&	Text() const {return fText;};

	virtual bool	Less(const Item* other) const { return false; };

protected:
	BString			fText;
	BRect			fCachedBounds;
};

#endif /* _TEXT_ITEM_H */
