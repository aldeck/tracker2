/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "PoseItem.h"

#include "IconItem.h"
#include "TextItem.h"
#include "ItemView.h"

#include <fs_attr.h>
#include <Entry.h>

#include <stdio.h>


PoseItem::PoseItem(ItemView* parentItemView, const entry_ref& entryRef)
	:
	Item(parentItemView),
	fLoaded(false),
	fEntryRef(entryRef),
	fIconItem(NULL)
{	

	// attention on ouvre/ferme deux fois la node
	fIconItem = new IconItem(fParentItemView, fEntryRef);	
	AddChild(fIconItem, BPoint(0, 0));
	//_Load();
}


PoseItem::~PoseItem()
{
	// TODO
	/*delete fIconItem;
	TextItemVector::iterator it = fTextItems.begin();
	for (; it != fTextItems.end(); it++)
		delete (*it);*/
}


//deprecate
bool
PoseItem::Less(const Item* other) const
{
	return true; //fNameItem->Text() < dynamic_cast<const TextItem*>(other)->Text();
}


/*uint32
PoseItem::Rank(uint32 sorting)
{
}


void
PoseItem::SetRank(uint32 sorting)
{
}*/

void
PoseItem::_Load()
{
	if (!fLoaded) {
		//printf("_Load %p\n", this);	
	
		BPoint nameOffset(32, 0);
		TextItem* textItem = new TextItem(fParentItemView, fEntryRef.name);
		AddChild(textItem, nameOffset);		
		nameOffset.y += 16;
	
		// test, read some attributes a faire dans un lazy load
		BNode node(&fEntryRef);
		status_t err2 = node.InitCheck();
		if (err2 != B_OK) {
			printf("error opening node err=%s\n", strerror(err2));
			return;
		}		
		
		char attributeName[B_ATTR_NAME_LENGTH];
		while (node.GetNextAttrName(attributeName) == B_OK) {
			attr_info info;
			if (node.GetAttrInfo(attributeName, &info) == B_OK) {
				BString attribute;			
				status_t err3 = node.ReadAttrString(attributeName, &attribute);
				if (err3 != B_OK) {		
					//printf("can't read attribute %s\n", attributeName);
					attribute = "error";
				}
				
				textItem = new TextItem(fParentItemView, BString(attributeName) << ": " << attribute);
				AddChild(textItem, nameOffset);
				nameOffset.y += 16;	
				/*char attrData[2048];
				if (info.size < 2048) {
	 				ssize_t read = node.ReadAttr(attrName, (type_code)0, (off_t)0, attrData, info.size);
	 				fTextItems.push_back(new TextItem(parentItemView, entryRef.name));
	 			} else {
	 				printf("toobig!=%lu ", info.size);
	 			}*/	   			
			} else {
					printf("noattrinfo");
	   		}
		}
						
		fLoaded = true; 	// TODO error check	
	}
}


/*void
PoseItem::_InitTextItemPositions() {
	
	BPoint nameOffset(32, 0);
	TextItemVector::iterator it = fTextItems.begin();
	for (; it != fTextItems.end(); it++) {
		(*it)->SetPosition(fPositions[fParentItemView->CurrentLayouterIndex()] + nameOffset, fParentItemView->CurrentLayouterIndex());
		nameOffset.y += 16;
	}	
}*/
	

/*void
PoseItem::SetPosition(const BPoint& position, uint32 space)
{
	Item::SetPosition(position, space);

	BPoint iconOffset;
	
	fIconItem->SetPosition(position + iconOffset, space);
	
	BPoint nameOffset(32, 0);
	TextItemVector::iterator it = fTextItems.begin();
	for (; it != fTextItems.end(); it++) {
		(*it)->SetPosition(position + nameOffset, space);
		nameOffset.y += 16;	
	}
	
}*/


BRect
PoseItem::Frame() const
{
	// TODO think: on est obligé de loader pour avoir frame exact donc on ne peux pas mettre dans le 
	// spatial cache sans loader...
	// ca veut dire qu'il faut une taille fixe
	
	/*if (fLoaded) {
		BRect textItemsExtent(LONG_MAX, LONG_MAX, LONG_MIN, LONG_MIN);
		TextItemVector::const_iterator it = fTextItems.begin();
		for (; it != fTextItems.end(); it++)
			textItemsExtent = textItemsExtent | (*it)->Frame();
				
		return fIconItem->Frame() | textItemsExtent;
	}*/
	//return fIconItem->Frame();
	
	BRect bounds(0, 0, 480, 320);	//unnecessary object creation?
	return bounds.OffsetBySelf(Position()); // toto Position() method
	
}

// Hide / Show
// ChildItems


void
PoseItem::Draw()
{
	//printf("Draw %p\n", this);
	_Load();
	Item::Draw();
}
