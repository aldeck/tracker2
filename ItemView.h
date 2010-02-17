/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _POSEVIEW_H
#define _POSEVIEW_H

#include <View.h>

#include <vector>

#include "ItemList.h"
#include "model/HighLevelQuery.h"

class BScrollBar;
class Item;
class Layouter;

const uint32 kMsgExtentChanged = 'ExCh';

class ItemView : public BView, public HighLevelQueryListener {
public:
					ItemView(BRect frame);
					~ItemView();

	virtual void	AddItem(Item* item);
	virtual void	RemoveItem(Item* item);

	virtual void	Draw(BRect updateRect);
	virtual void	AttachedToWindow();
	virtual void	FrameResized(float width, float height);
	virtual void	KeyUp(const char* bytes, int32 numBytes);
	virtual	void	MouseDown(BPoint point);
	virtual	void	MouseUp(BPoint point);
	virtual void	MouseMoved(BPoint point, uint32 transit,
						const BMessage* message);
	virtual void	MessageReceived(BMessage *message);
	
	virtual BRect	Extent() const;

	int				CurrentLayouterIndex() const;
	
	// HighLevelQueryListener
	virtual	void	EntryAdded(const node_ref& nodeRef, const entry_ref& entryRef);
	virtual	void	EntryRemoved(const node_ref& nodeRef, const entry_ref& entryRef);
	virtual	void	EntryChanged(const node_ref& nodeRef, const entry_ref& entryRef);


protected:
	void			_MouseDrag(BPoint point);
	void			_NotifyExtentChanged();
	//void			_NotifyItemAdded(Item* item);
	//void			_NotifyItemRemoved(Item* item);

	void			_Test();
	void			_MoveTest();

	ItemList		fItems;
	typedef std::vector<Item*> ItemVector;
	ItemVector		fSortedItems;

	typedef std::vector<Layouter*> LayouterList;
	LayouterList	fLayouters;
	uint32			fCurrentLayouterIndex;
	

	Item*			fDraggedItem;
	BPoint			fDraggingPointOffset;

	bool			fDebugDrawing;
	
	HighLevelQuery	fHighLevelQuery;
};

#endif /* _POSEVIEW_H */
