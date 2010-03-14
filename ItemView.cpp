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
#include "PoseItem.h"
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
	printf("size of Item %i\n", sizeof(Item));
	

	fLayouters.push_back(new ListLayouter(0, this, 50));
	//fLayouters.push_back(new GridLayouter(1, this, BPoint(50, 50)));
}


void ItemView::_Test()
{
	fHighLevelQuery.ChangeDirectory("/boot/home");
			
	
}


void
ItemView::HighLevelQueryEventReceived(uint32 code, /*const*/ HighLevelQueryResult* result)
{

	switch (code) {
		case HighLevelQuery::HLQ_RESULT_ADDED:
		{
			/*if (result != NULL) {
				Item* item = new PoseItem(this, result->entryRef);
				AddItem(item);
			}*/
			break;
		}
		case HighLevelQuery::HLQ_FULL_UPDATE:
		{
			printf("ItemView::HighLevelQueryEventReceived HLQ_FULL_UPDATE event\n");
			
			RemoveAllItems();
			
			HighLevelQuery::ResultVector::iterator it = fHighLevelQuery.Results().begin();
			for (; it != fHighLevelQuery.Results().end(); it++) {
				//printf("rank %lu: %s\n", (*it).rank, (*it).entryRef.name);
				Item* item = new PoseItem(this, (*it)->entryRef);	// passer le rank
				//printf("add new %p %s\n", item, (*it)->entryRef.name);
				AddItem(item);
			}		
			
			for (uint32 i = 0; i < fLayouters.size(); i++)	// faire le layout au moment de l'ajout
				fLayouters[i]->LayoutAllItems();
			
			_NotifyExtentChanged();
			
			Invalidate(); // ptetre pas necessaire
			
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
	// TODO
}


void
ItemView::RemoveAllItems()
{
	printf("ItemView::RemoveAllItems\n");
	for (uint32 i = 0; i < fLayouters.size(); i++)
		fLayouters[i]->RemoveAllItems();
		
	ItemList::iterator it = fItems.begin();
	for (; it != fItems.end(); it++) {
		//printf("delete %p\n", (*it));
		delete (*it);
	}
	fItems.clear();
	
	//int* foo = new int[100];
	//printf("foo %p\n", foo);
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
		case (kMsgURIChanged):
		{			
			BString uri;
			message->FindString("uri", &uri);			
			fHighLevelQuery.ChangeDirectory(uri);
			MakeFocus(true);
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
				//if (fDebugDrawing)
				//	StrokeRect((*it)->Frame()); //debug
			}
		//}
	}

	// draw dragged item last
	if (fDraggedItem != NULL && fDraggedItem->Frame().Intersects(updateRect)) {
		//drawCount++;
		fDraggedItem->Draw();
		//if (fDebugDrawing)
		//	StrokeRect(fDraggedItem->Frame()); //debug
	}
	
	// draw items bounding box
	//if (fDebugDrawing)
	//	StrokeRect(Extent());
		
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
	// todo double click
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
				//_MoveTest();
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
		BPoint currentPos = (*it)->RelativePosition();
		BPoint nextPos = currentPos + BPoint(0, 5);
		(*it)->SetRelativePosition(nextPos);
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
		currentPos = fDraggedItem->RelativePosition();
		Invalidate(fDraggedItem->Frame());
		fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->RemoveItem(fDraggedItem);
		nextPos = point - fDraggingPointOffset;
		fDraggedItem->SetRelativePosition(nextPos);
		Invalidate(fDraggedItem->Frame());
		fLayouters[fCurrentLayouterIndex]->GetSpatialCache()->AddItem(fDraggedItem); 
			// todo, itemmoved, et autonotify
		
		/* test colision
		
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
		}*/
		
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
	switch (bytes[0]) {
		case B_LEFT_ARROW:
			if (fCurrentLayouterIndex == 0)
				fCurrentLayouterIndex = fLayouters.size() - 1;
			else
				fCurrentLayouterIndex--;

			printf("Layouter %lu\n", fCurrentLayouterIndex);
			Invalidate();
			break;

		case B_RIGHT_ARROW:
			if (fCurrentLayouterIndex == fLayouters.size() - 1)
				fCurrentLayouterIndex = 0;
			else
				fCurrentLayouterIndex++;

			printf("Layouter %lu\n", fCurrentLayouterIndex);
			Invalidate();
			break;
		
		case B_UP_ARROW:
		{
			printf("Invert sort\n");
			printf("Layouter %lu\n", fCurrentLayouterIndex);			
			
			fHighLevelQuery.InvertSort();			
			Invalidate();
			break;			
		}
		
		case B_DOWN_ARROW:
		{
			RemoveAllItems();			
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


