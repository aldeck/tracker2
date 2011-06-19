/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _TREE_H
#define _TREE_H


#include <vector>


class Node {
public:
						Item(ItemView* parentItemView);
	virtual				~Item ();

			void		AddChild(Item* item, const BPoint& relativePosition);
			void		RemoveChild(Item* item);

protected:
			void		AttachedToParent(Item* item);
	
			Item*		fParentItem;
	typedef std::vector<Item*> ItemVector;
			ItemVector	fChildItems;	
};

#endif /* _TREE_H */
