/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <Window.h>


class ItemView;
class BHandler;
class BMenuBar;
class BScrollView;
class BTextControl;

class MainWindow: public BWindow {
public:
					MainWindow(BRect frame, const char* title);
					~MainWindow();

	virtual bool	QuitRequested();
	virtual void	MessageReceived(BMessage* message);
	virtual void	DispatchMessage(BMessage* message, BHandler* target);
	
protected:
	void			_UpdateScrollBars();

	BView*			fToolbarView;
	BScrollView*	fRootView;
	ItemView*		fItemView;
	
	BMenuBar*		fMenuBar;
	BTextControl*	fURITextControl;
};

#endif /* _MAINWINDOW_H */
