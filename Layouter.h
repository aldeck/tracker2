/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _LAYOUTER_H
#define _LAYOUTER_H

#include <set>

class BPoint;
class Item;
class SpatialCache;

class Layouter {
public:
	virtual				~Layouter();

	virtual	void		AddItem(Item* item, bool updateNow = false) = 0;
	//virtual	void		AddPoses(PoseList* list, bool updateNow = false) = 0;
	//virtual	void		RemovePose(Pose* pose) = 0;
	//virtual	void		RemovePoses(PoseList* list) = 0;
	virtual	void		RemoveAllItems() = 0;

	virtual void		LayoutAllItems() = 0;

	//					UpdateAllPoses(); // change la position de chaque pose, si differente de celle stockée dans pose->position
	//					UpdatePosesInRect(const Brect& rect); // si c'est rapide pas besoin d'updater ce qu'on ne voit pas
						// ca veut dire que les positions sont dans un spatial cache
	virtual SpatialCache* GetSpatialCache() const = 0;
};

#endif	// _LAYOUTER_H
