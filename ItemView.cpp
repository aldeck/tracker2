/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "ItemView.h"

#include <stdio.h>
#include <algorithm>

#include <Directory.h>
#include <DragTrackingFilter.h>
#include <Entry.h>
#include <Message.h>
#include <Messenger.h>
#include <ScrollBar.h>
#include <Window.h>

#include "Item.h"
//#include "IconItem.h"
//#include "FileItem.h"
#include "PoseItem.h"
//#include "TextItem.h"
#include "GridLayouter.h"
#include "ListLayouter.h"
#include "SpatialCache.h"

const uint32 kMsgDragStarted = 'Drgs';


ItemView::ItemView(BRect frame)
	:
	BView(frame, "itemView", B_FOLLOW_ALL, B_WILL_DRAW),
	fCurrentLayouterIndex(0),
	fDraggedItem(NULL),
	fDebugDrawing(false)
{
	fLayouters.push_back(new GridLayouter(this, BPoint(50, 50), 0));
	//fLayouters.push_back(new ListLayouter(this, 50, 1));
}


bool less_Item(const Item& i1, const Item& i2)
{
   return i1.Less(&i2);
}


void ItemView::_Test()
{
	/*for (uint32 i = 0; i < 1048; i++) {
		Item* item = new IconItem(this);
		fItems.push_back(item);
		for (uint32 j = 0; j < fLayouters.size(); j++)		// faire un callback pose added
			fLayouters[j]->AddItem(item);
	}*/
	
	/*uint32 count = 0;

	for (int i = 0; i < 1; i++) {
		BDirectory directory("/Data2/mail/Haiku-commits");
			// "/system/apps");//  /Data2/mail/Haiku-bugs
		entry_ref ref;
		
		while (true) {
			status_t status = directory.GetNextRef(&ref);
			if (status != B_OK)
				break;
			
			BEntry entry(&ref);
			char name[256]; //REF_SIZE
			entry.GetName(name);
	
			Item* item = new PoseItem(this, name);
			AddItem(item);
	
			count++;
	
			if (count % 500 == 0)
				printf("%lu \n", count);
		}
	}
	printf("Total added %lu \n", count);*/
	
	fHighLevelQuery.DoIt();
	
	for (uint32 j = 0; j < fLayouters.size(); j++)		// faire un callback pose added
		fLayouters[j]->LayoutAllItems();
			
	_NotifyExtentChanged();	
}


void
ItemView::HighLevelQueryEventReceived(uint32 code, /*const*/ HighLevelQueryResult* result)
{

	switch (code) {
		case HighLevelQuery::HLQ_RESULT_ADDED:
		{
			if (result != NULL) {
				Item* item = new PoseItem(this, result->entryRef);
				AddItem(item);
			}
			break;
		}
		case HighLevelQuery::HLQ_FULL_UPDATE:
		{
			printf("ItemView::HighLevelQueryEventReceived HLQ_FULL_UPDATE event\n");
			HighLevelQuery::ResultVector::iterator it = fHighLevelQuery.Results().begin();
			for (; it != fHighLevelQuery.Results().end(); it++) {
				//printf("sorted: %s\n", (*it).entryRef.name);
				Item* item = new PoseItem(this, (*it).entryRef);
				AddItem(item);
			}			
			break;
		}
		default:
			printf("ItemView::HighLevelQueryEventReceived unhandled event\n");
	}
}


int
ItemView::CurrentLayouterIndex() const
{
	return fCurrentLayouterIndex;
}


ItemView::~ItemView()
{
}


void
ItemView::AddItem(Item* item)
{
	fItems.push_back(item);
	for (uint32 j = 0; j < fLayouters.size(); j++)
		fLayouters[j]->AddItem(item);	// TODO do that in the viewmode (listener)
	
	//_NotifyItemAdded(item);
}


void
ItemView::RemoveItem(Item* item)
{
	
}


/*void
ItemView::_NotifyItemAdded(Item* item)
{
	ListenerList::iterator	it = fListeners.begin();
	for (; it != fListeners.end(); it++)
		(*it)->ItemAdded(item);
}


void
ItemView::_NotifyItemRemoved(Item* item)
{
	ListenerList::iterator	it = fListeners.begin();
	for (; it != fListeners.end(); it++)
		(*it)->ItemRemoveditem);
}*/


void
ItemView::AttachedToWindow()
{
	BView::AttachedToWindow();

	SetDrawingMode(B_OP_ALPHA);
	SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);

	MakeFocus(); //key events

	AddFilter(new DragTrackingFilter(this, kMsgDragStarted));

	Window()->AddHandler(&fHighLevelQuery);
		// temporariliy set here
		// to receive node monitoring message
		
	fHighLevelQuery.AddListener(this);
		
	_Test();
}


void
ItemView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case (kMsgDragStarted):
		{
			BPoint point;
			message->FindPoint("be:view_where", &point);
			_MouseDrag(point);
			break;
		}
		default:
			BView::MessageReceived(message);
	}
}


BRect
ItemView::Extent() const
{
	return fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->ItemsExtent();
}


void
ItemView::Draw(BRect updateRect)
{
	//ItemList& res = fItems;
	//ItemList::iterator it = res.begin();
	ItemSet res = fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->FindIntersectingItems(updateRect);
	ItemSet::iterator it = res.begin();
	//bigtime_t startTime = system_time();
	//int drawCount = 0;
	for (; it != res.end() ; it++) {
		//if ((*it)->Frame().Intersects(updateRect)) {	// pas necessaire avec spatial cache
			if ((*it) != fDraggedItem) {
				//drawCount++;
				(*it)->Draw();
				if (fDebugDrawing)
					StrokeRect((*it)->Frame()); //debug
			}
		//}
	}

	// draw dragged item last
	if (fDraggedItem != NULL && fDraggedItem->Frame().Intersects(updateRect)) {
		//drawCount++;
		fDraggedItem->Draw();
		if (fDebugDrawing)
			StrokeRect(fDraggedItem->Frame()); //debug
	}
	
	// draw items bounding box
	if (fDebugDrawing)
		StrokeRect(Extent());
		
	//float ftime = (float) (system_time() - startTime);
	//printf("ItemView::Draw drawn %d items from %d in %f µs\n", drawCount, res.size(), ftime);
}


void
ItemView::FrameResized(float width, float height)
{
}


void
ItemView::MouseDown(BPoint point)
{
	//faire un filtrage avec pose spatial cache
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		if ((*it)->Frame().Contains(point)) {
			BMessage* msg = Window()->CurrentMessage();
			uint32 buttons = msg->FindInt32("buttons");

			if (buttons == B_PRIMARY_MOUSE_BUTTON) {
				(*it)->MouseDown(point);
			} else if (buttons == B_SECONDARY_MOUSE_BUTTON) {
				(*it)->ContextDown(point);
				_MoveTest();
				//fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->RemoveAllItems();
				//fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->_UpdateAllItems();
				//fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->FindIntersectingItems(Bounds());
			}
		}
	}
}


void
ItemView::_MoveTest()
{	
	bigtime_t startTime = system_time();
	int count = 0;
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		//fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->RemoveItem(*it);
		BPoint currentPos = (*it)->Position(fCurrentLayouterIndex);
		BPoint nextPos = currentPos + BPoint(0, 5);
		(*it)->SetPosition(nextPos, fCurrentLayouterIndex);
		//fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->AddItem(*it);
		count++;
	}
	float ftime = (float) (system_time() - startTime);
	printf("ItemView::_MoveTest() moved %d items %f µs\n", count, ftime);
	fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->_UpdateAllItems();
	
	Invalidate(Bounds());
}


void
ItemView::MouseUp(BPoint point)
{
	fDraggedItem = NULL;
	fDraggingPointOffset.Set(0, 0);
	
	// do that somewhere else
	_NotifyExtentChanged();
}


void
ItemView::MouseMoved(BPoint point, uint32 transit, const BMessage* message)
{
	BPoint currentPos;
	BPoint nextPos;
	if (fDraggedItem != NULL) {
		currentPos = fDraggedItem->Position(fCurrentLayouterIndex);
		Invalidate(fDraggedItem->Frame());
		fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->RemoveItem(fDraggedItem);
		nextPos = point - fDraggingPointOffset;
		fDraggedItem->SetPosition(nextPos, fCurrentLayouterIndex);
		Invalidate(fDraggedItem->Frame());
		fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->AddItem(fDraggedItem);
		
		BRect frame(fDraggedItem->Frame());
		ItemSet res = fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->FindIntersectingItems(frame);
		ItemSet::iterator it = res.begin();
		for (; it != res.end() ; it++) {
			if ((*it) != fDraggedItem) {
				Invalidate((*it)->Frame());
				fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->RemoveItem((*it));
			}
		}
		it = res.begin();
		for (; it != res.end() ; it++) {
			if ((*it) != fDraggedItem) {
				BPoint pos((*it)->Position(fCurrentLayouterIndex));
				(*it)->SetPosition(pos + (nextPos - currentPos), fCurrentLayouterIndex);
				
				fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->AddItem((*it));
				Invalidate((*it)->Frame());
			}
		}
		
	}
}


void
ItemView::_MouseDrag(BPoint point)
{
	if (fDraggedItem != NULL)
		printf("bizarre\n");

	//todo faire un filtrage avec pose spatial cache
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		if ((*it)->Frame().Contains(point)) {
			fDraggedItem = *it;
			fDraggingPointOffset = point - fDraggedItem->Frame().LeftTop();
			printf("Dragging item=%p\n", fDraggedItem);
			break; //il peut y envoir plusieurs si superposees
		}
	}
}


void
ItemView::_NotifyExtentChanged()
{
	BMessenger messenger(Window());
	messenger.SendMessage(kMsgExtentChanged);
}


void
ItemView::KeyUp(const char* bytes, int32 numBytes)
{
	char key = bytes[0];
	printf("keyup\n");

	switch (key) {
		case B_LEFT_ARROW:
			if (fCurrentLayouterIndex == 0)
				fCurrentLayouterIndex = fLayouters.size() - 1;
			else
				fCurrentLayouterIndex--;

			printf("Next layouter, %lu\n", fCurrentLayouterIndex);
			Invalidate();
			break;

		case B_RIGHT_ARROW:
			if (fCurrentLayouterIndex == fLayouters.size() - 1)
				fCurrentLayouterIndex = 0;
			else
				fCurrentLayouterIndex++;

			printf("Previous layouter, %lu\n", fCurrentLayouterIndex);
			Invalidate();
			break;
		
		case B_UP_ARROW:
		{
			printf("Invert sort\n");			
			fLayouters[fCurrentLayouterIndex]->RemoveAllItems();
			
			ItemList::iterator it = fItems.begin();
			for (; it != fItems.end(); it++) {
				delete (*it);
			}
			
			fHighLevelQuery.InvertSort();
			fLayouters[fCurrentLayouterIndex]->LayoutAllItems();
			Invalidate();
			break;			
		}
		case 'd':
			fDebugDrawing = !fDebugDrawing;
			Invalidate();
			break;

		default:
			break;
	}
}


