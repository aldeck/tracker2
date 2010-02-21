/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _POSE_ITEM_H
#define _POSE_ITEM_H

#include "Item.h"

#include <Entry.h>
#include <Rect.h>
#include <String.h>

#include <vector>

class BPoint;

class ItemView;
class IconItem;
class TextItem;

class PoseItem : public Item {
public:
					PoseItem(ItemView* parentItemView, const entry_ref& ref);
	virtual			~PoseItem();
	
	virtual void	SetPosition(const BPoint& position, uint32 positionId);
	virtual	void	Draw();
	virtual	BRect	Frame() const;
	virtual bool	Less(const Item* other) const;

protected:
	bool			fLoaded;
	void			_Load();
		
	entry_ref		fEntryRef;
	IconItem*		fIconItem;
	
	typedef std::vector<TextItem*> TextItemVector;
	TextItemVector	fTextItems;	
};

#endif /* _POSE_ITEM_H */
