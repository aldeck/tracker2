/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ITEM_GROUP_H
#define _ITEM_GROUP_H

#include <Point.h>

#include <vector>

class BView;

class ItemView;

class Item {
public:
					Item(ItemView* parentItemView);
	virtual			~Item ();

	virtual	void	Draw() = 0;
	virtual	void	MouseDown(BPoint point);
	virtual	void	ContextDown(BPoint point);
	virtual	BRect	Frame() const = 0;

	//todo choisir entre un simple MessageReceived, MouseDown gauche droit, ou Select, Context, Action, (Drag)

	//virtual BPoint	Position(uint32 spaceIndex) const;
	virtual void	SetPosition(const BPoint& position, int positionId);
	//virtual uint32	Rank(uint32 sortingIndex/*ou sortingId*/) const;
	//virtual void	SetRank(uint32 rank, int sortingIndex);

	//Item::Moved callback/notifier

protected:
	ItemView*		fParentItemView;

	//void*			fUserData;
	//ItemUserData*	fUserData;	//permet d'attacher des donees comme dans ogre ou bullet
								//peut etre un lien vers un object vommun entre plusieurs items

	// one position per layouter
	typedef std::vector<BPoint> PositionList;
	PositionList	fPositions;

	// one ordering per layouter or per sort criterion
	//typedef std::vector<uint32> RankList;
	//RankList		fRanks;

	//ca implique qu'il faut une methode Item::Compare(Item* other, uint32 criterion)
	//ou sinon ce n'est pas l'affaire de item mais de ses heritiers

	// apres pas sur qu'il faille fair le tri dans les layouters, d'ailleurs un tri est toujours valide si
	// on change de layout
	// donc class Orderer
	// les layouters organisent la position, les orderers les ranks , et parfois specialises sur une sous
	// classe the item ex: FileItemOrderer

	// on pourrait trier par couleur, avec un BitmapItemOrderer (dailleurs, IconItem devrait descendre de bitmap

	// il faut pouvoir grouper des items et appliquer un ranking sur le groupe en fonction d'un tri sur
	// un des elements du groupe

};

#endif /* _ITEM_GROUP_H */
