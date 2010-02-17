/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "MainWindow.h"

#include <Application.h>
#include <ScrollView.h>

#include <stdio.h>

#include "ItemView.h"


MainWindow::MainWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 0)
{
	//fRootView = new BView(Bounds(), "rootView", B_FOLLOW_ALL, 0);
	//AddChild(fRootView);

	BRect viewFrame(Bounds());
	viewFrame.right = viewFrame.right - B_V_SCROLL_BAR_WIDTH;
	viewFrame.bottom = viewFrame.bottom - B_H_SCROLL_BAR_HEIGHT;

	fItemView = new ItemView(viewFrame/*Bounds()*/);
	//fRootView->AddChild(fItemView);

	fRootView = new BScrollView("rootView", fItemView, B_FOLLOW_ALL, 0, true, true);	
	AddChild(fRootView);
	Show();	
}


MainWindow::~MainWindow()
{
}


bool
MainWindow::QuitRequested()
{
	printf("quitrequested\n");
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage *message)
{
	//message->PrintToStream();
	switch (message->what) {
		case kMsgExtentChanged:
			_UpdateScrollBars();
			break;
		default:
			BWindow::MessageReceived(message);
	}
}


void
MainWindow::_UpdateScrollBars()
{
	//printf("MainWindow::_UpdateScrollBars() h=%p v=%p\n", fRootView->ScrollBar(B_HORIZONTAL), fRootView->ScrollBar(B_VERTICAL));
	
	BPoint origin(fItemView->LeftTop());
	BRect bounds(fItemView->Bounds());
	BRect extent(fItemView->Extent());

	BPoint minVal(std::min(extent.left, origin.x), std::min(extent.top, origin.y));

	BPoint maxVal((extent.right - bounds.right) + origin.x,
		(extent.bottom - bounds.bottom) + origin.y);

	maxVal.x = std::max(maxVal.x, origin.x);
	maxVal.y = std::max(maxVal.y, origin.y);
	
	
	
	fRootView->ScrollBar(B_HORIZONTAL)->SetRange(minVal.x, maxVal.x);
	fRootView->ScrollBar(B_VERTICAL)->SetRange(minVal.y, maxVal.y);	
}

