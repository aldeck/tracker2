/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include <Alert.h>

#include "App.h"
#include "MainWindow.h"

#include <stdio.h>

App::App()
	:
	BApplication("application/x-vnd.Haiku-Tracker2")
{
	BRect frame(50, 50, 640 + 50, 480 + 50);
	const char *title = "Tracker2";
	fMainWindow = new MainWindow(frame, title);
}


App::~App()
{
}


void
App::AboutRequested()
{
	BAlert* alert;
	alert = new BAlert("About", "Tracker 2", "ok");
	alert->Go(NULL);
}


bool
App::QuitRequested()
{
	printf("app::quitrequested\n");
	return true;
}


void
App::MessageReceived(BMessage *message)
{
	message->PrintToStream();
}


int
main(int argc, char** argv)
{
	App app;
	app.Run();
	return 0;
}
