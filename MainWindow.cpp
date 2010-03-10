/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "MainWindow.h"

#include <Application.h>
#include <GroupLayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <TextControl.h>

#include <stdio.h>

#include "ItemView.h"


/*enum {
	OPEN_LOCATION = 'open',
	GO_BACK = 'goba',
	GO_FORWARD = 'gofo',
	STOP = 'stop',
	GOTO_URI = 'goui',
	RELOAD = 'reld',
	CLEAR_HISTORY = 'clhs',

	TEXT_SIZE_INCREASE = 'tsin',
	TEXT_SIZE_DECREASE = 'tsdc',
	TEXT_SIZE_RESET = 'tsrs',

	TEXT_SHOW_FIND_GROUP = 'sfnd',
	TEXT_HIDE_FIND_GROUP = 'hfnd',
	TEXT_FIND_NEXT = 'fndn',
	TEXT_FIND_PREVIOUS = 'fndp',
};*/


static BLayoutItem*
layoutItemFor(BView* view)
{
	BLayout* layout = view->Parent()->GetLayout();
	int32 index = layout->IndexOfView(view);
	return layout->ItemAt(index);
}


MainWindow::MainWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
		B_AUTO_UPDATE_SIZE_LIMITS | B_ASYNCHRONOUS_CONTROLS),
	fURITextControl(NULL),
	fMenuBar(NULL)
{
	
	fMenuBar = new BMenuBar("Main menu");
	BMenu* menu = new BMenu("Window");
	BMenuItem* quitItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q');
	menu->AddItem(quitItem);
	quitItem->SetTarget(be_app);
	fMenuBar->AddItem(menu);
	
	// URI
	fURITextControl = new BTextControl("uri", "", "", new BMessage(kMsgURIChanged));
	fURITextControl->SetDivider(50.0);
	
	
	//BRect toolbarFrame(Bounds());
	//toolbarFrame.bottom = toolbarFrame.bottom + 24;
	//fToolbarView = new BView(toolbarFrame, "toolbarView", B_FOLLOW_ALL, 0);
	//AddChild(fToolbarView);
	//fRootView = new BView(Bounds(), "rootView", B_FOLLOW_ALL, 0);
	//AddChild(fRootView);

	BRect viewFrame(Bounds());
	viewFrame.right = viewFrame.right - B_V_SCROLL_BAR_WIDTH;
	viewFrame.bottom = viewFrame.bottom - B_H_SCROLL_BAR_HEIGHT;

	fItemView = new ItemView(viewFrame/*Bounds()*/);
	
	fRootView = new BScrollView("rootView", fItemView, B_FOLLOW_ALL, 0, true, true);	
	
	const float kInsetSpacing = 5;
	const float kElementSpacing = 7;
	
	SetLayout(new BGroupLayout(B_HORIZONTAL));
	
	AddChild(BGroupLayoutBuilder(B_VERTICAL)
			.Add(fMenuBar)
			.Add(BGridLayoutBuilder(kElementSpacing, kElementSpacing)
				.Add(fURITextControl, 0, 0)
				.SetInsets(kInsetSpacing, kInsetSpacing, kInsetSpacing, kInsetSpacing)
			)				
			.Add(fRootView)
		);	
	
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
MainWindow::DispatchMessage(BMessage* message, BHandler* target)
{
	/*if (fURITextControl && message->what == B_KEY_DOWN
		&& target == fURITextControl->TextView()) {
		// Handle B_RETURN in the URL text control. This is the easiest
		// way to react *only* when the user presses the return key in the
		// address bar, as opposed to trying to load whatever is in there when
		// the text control just goes out of focus.
		const char* bytes;
		if (message->FindString("bytes", &bytes) == B_OK
			&& bytes[0] == B_RETURN) {
			// Do it in such a way that the user sees the Go-button go down.
			//fGoButton->SetValue(B_CONTROL_ON);
			//UpdateIfNeeded();
			//fGoButton->Invoke();
			//snooze(1000);
			//fGoButton->SetValue(B_CONTROL_OFF);
			BMessage msg(GOTO_URI);
			BMessenger(fItemView).SendMessage(&msg);
		}
	}*/
	BWindow::DispatchMessage(message, target);
}



void
MainWindow::MessageReceived(BMessage *message)
{
	//message->PrintToStream();
	switch (message->what) {
		case kMsgExtentChanged:
			_UpdateScrollBars();
			break;
		case (kMsgURIChanged):
		{			
			BString uri = fURITextControl->Text();			
			message->AddString("uri", uri);
			BMessenger(fItemView).SendMessage(message);			
			break;
		}
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

