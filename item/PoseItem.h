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

#include <Rect.h>
#include <String.h>

class BPoint;

class ItemView;
class IconItem;
class TextItem;
struct entry_ref;

class PoseItem : public Item {
public:
					PoseItem(ItemView* parentItemView, const entry_ref& ref);
	virtual			~PoseItem();
	
	virtual void	SetPosition(const BPoint& position, uint32 positionId);
	virtual	void	Draw();
	virtual	BRect	Frame() const;
	virtual bool	Less(const Item* other) const;

protected:
	IconItem*		fIconItem;
	TextItem*		fNameItem;
	//TextItem*		fTypeItem;
};

#endif /* _POSE_ITEM_H */
