/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ITEM_H
#define _ITEM_H

#include <Point.h>

#include <vector>

class BView;

class ItemView;

class Item {
public:
					Item(ItemView* parentItemView);
	virtual			~Item ();

	virtual	void	Draw()/* = 0*/;
	virtual	void	MouseDown(BPoint point);
	virtual	void	ContextDown(BPoint point);
	virtual	BRect	Frame() const = 0;

	//todo choisir entre un simple MessageReceived, MouseDown gauche droit,
	// ou Select, Context, Action, (Drag)

	virtual BPoint	Position() const;
	//virtual void	SetPosition(const BPoint& position);
	
	virtual BPoint	RelativePosition() const;
	virtual void	SetRelativePosition(const BPoint& position);
	
	/*virtual uint32	Rank() const;
	virtual void	SetRank(uint32 rank);*/

	//ItemMoved callback/notifier

			void	AddChild(Item* item, const BPoint& relativePosition);
			void	RemoveChild(Item* item);

protected:
			void	AttachedToParent(Item* item);

	ItemView*		fParentItemView;	//rename to fRenderView?

	//void*			fUserData;
	//ItemUserData*	fUserData;	//permet d'attacher des donees comme dans ogre ou bullet
								//peut etre un lien vers un object commun entre plusieurs items

	BPoint			fRelativePosition;
	uint32			fRank;
	
	Item*			fParentItem;
	typedef std::vector<Item*> ItemVector;
	ItemVector		fChildItems;	
};

#endif /* _ITEM_H */
