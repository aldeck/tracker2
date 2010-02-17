/*
 * Copyright 2010, Haiku Inc. All Rights Reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Alexandre Deckner, alex@zappotek.com
 */
 
#include "HighLevelQuery.h"

#include <Directory.h>
#include <NodeMonitor.h>
#include <Path.h>

/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <Application.h>

#include <Entry.h>
#include <Node.h>


#include <Query.h>
#include <fs_attr.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <String.h>

#include <ObjectList.h>*/

static bool sVerbose = false;


HighLevelQuery::HighLevelQuery()
{
	fDebug = false;
}


HighLevelQuery::~HighLevelQuery()
{
}


void
HighLevelQuery::_ManageEntry(const entry_ref& entry)
{
	BNode node(&entry);
	status_t err2 = node.InitCheck();
	if (err2 != B_OK) {
		printf("error opening node err=%s\n", strerror(err2));
		return;
	} else {
		node_ref nodeRef;
   		node.GetNodeRef(&nodeRef);
		// debug check
		EntryMap::iterator it = fEntries.find(nodeRef);
		if (it != fEntries.end()) {
			printf("HighLevelQuery::_ManageEntry, already managed node=%lli!!\n", nodeRef.node);
			return;
		}

		fEntries.insert(EntryMap::value_type(nodeRef, entry));

		// needed for the "query entry rename" problem
		status_t err = watch_node(&nodeRef,
			B_WATCH_NAME | B_WATCH_STAT | B_WATCH_ATTR, this);
		fEntryCount++;
		if (sVerbose || err != B_OK) {
			BPath path(&entry);
			printf("%lu HighLevelQuery::_ManageEntry (%lli, %s). Start watching err=%s\n", fEntryCount,
				nodeRef.node, path.Path(), strerror(err));
		}
		
		_NotifyEntryAdded(entry);

		// test, read some attributes

		/*	char buf[B_ATTR_NAME_LENGTH];
			//printf("attrs [ ");
			uint32 count = 0;
			while (node.GetNextAttrName(buf) == B_OK) {
	   			//printf("%s ", buf);
	   			attr_info info;
	   			status_t err3 = node.GetAttrInfo(buf, &info);
	   			if (err3 == B_OK) {
	   				// read all attributes
	   				char attrData[1024];
	   				if (info.size < 1024) {
		   				ssize_t read = node.ReadAttr(buf, (type_code)0, (off_t)0, attrData, info.size);
		   				//printf("read=%lu/%lu ", read, info.size);
		   				count++;
		   				if (count > 2)
		   					break;
		   			} else {
		   				//printf("toobig!=%lu ", info.size);
		   			}
	   			} else {
		   			//printf("noattrinfo '%s' ", strerror(err3));
		   		}
			}
			//printf("]\n");*/

	}
}


void
HighLevelQuery::_UnmanageEntry(const node_ref& nodeRef)
{
	EntryMap::iterator it = fEntries.find(nodeRef);
	if (it != fEntries.end()) {
		fEntries.erase(it);

		// needed for the "query entry rename" problem
		status_t err = B_OK; // watch_node(&nodeRef, B_STOP_WATCHING, this);

		if (sVerbose || err != B_OK) {
			BPath path(&it->second);
			printf("HighLevelQuery::_UnmanageEntry (%lli, %s), Stop watching B_WATCH_NAME err=%s\n",
				nodeRef.node, path.Path(), strerror(err));
		}
	} else
		printf("HighLevelQuery::_UnmanageEntry node %lli is not managed!!!\n", nodeRef.node);
}


void
HighLevelQuery::_UpdateEntry(const node_ref& nodeRef, const entry_ref& entry)
{
	EntryMap::iterator it = fEntries.find(nodeRef);
	if (it != fEntries.end()) {
		BPath fromPath(&it->second);
		it->second = entry;
		BPath toPath(&entry);
		printf("HighLevelQuery::_UpdateEntry (%lli, %s -> %s)\n", nodeRef.node, fromPath.Path(), toPath.Path());
	} else
		printf("HighLevelQuery::_UpdateEntry node %lli is not managed!!!\n", nodeRef.node);
}


void HighLevelQuery::_QueryUpdate(BMessage* message)
{
	int32 opcode;
	message->FindInt32("opcode", &opcode);

	int32 device;
	int64 directory;
	int64 node;
	const char* name;
	message->FindInt32("device", &device);
	message->FindInt64("directory", &directory);
	message->FindInt64("node", &node);
	message->FindString("name", &name);

	char info[B_PATH_NAME_LENGTH];
	sprintf(info, "device=%li directory=%lli node=%lli name=%s ", device, directory, node, name);

	switch (opcode) {
		case B_ENTRY_CREATED:
		{
			printf("%s QUERY_ B_ENTRY_CREATED\n", info);

			entry_ref entryRef;
			entryRef.device = device;
			entryRef.directory = directory;
			entryRef.set_name(name);

			_ManageEntry(entryRef);
			break;
		}
		case B_ENTRY_REMOVED:
		{
			printf("%s QUERY_ B_ENTRY_REMOVED\n", info);

			node_ref nodeRef;
			nodeRef.device = device;
			nodeRef.node = node;

			_UnmanageEntry(nodeRef);
			break;
		}
		default:
			printf("%s QUERY_ UNMANAGED OPCODE\n", info);
			//BApplication::MessageReceived(message);
			break;
	}
}


void HighLevelQuery::_NodeMonitorUpdate(BMessage* message)
{
	int32 opcode;
	message->FindInt32("opcode", &opcode);

	int32 device;
	int64 directory;
	int64 node;
	const char* name;
	message->FindInt32("device", &device);
	message->FindInt64("directory", &directory);
	message->FindInt64("node", &node);
	message->FindString("name", &name);

	char info[B_PATH_NAME_LENGTH];
	sprintf(info, "device=%li directory=%lli node=%lli name=%s ", device, directory, node, name);

	switch (opcode) {
		case B_ENTRY_CREATED:
			printf("%s MONITOR_ B_ENTRY_CREATED\n", info);
			break;

		case B_ENTRY_REMOVED:
			printf("%s MONITOR_ B_ENTRY_REMOVED\n", info);
			break;

		case B_ENTRY_MOVED:
		{
			// can be about:
			// a query node that we're monitoring and which name has changed
			// a file in a directory that we monitor that moved in

			int64 fromDirectory;
			int64 toDirectory;
			message->FindInt64("from directory", &fromDirectory);
			message->FindInt64("to directory", &toDirectory);
			char info2[B_PATH_NAME_LENGTH];
			sprintf(info2, "device=%li fromdir=%lli todir=%lli node=%lli name=%s ", device, fromDirectory, toDirectory, node, name);
			printf("%s MONITOR_ B_ENTRY_MOVED\n", info2);

			node_ref nodeRef;
			nodeRef.device = device;
			nodeRef.node = node;

			entry_ref entryRef;
			entryRef.device = device;
			entryRef.directory = toDirectory;
			entryRef.set_name(name);

			_UpdateEntry(nodeRef, entryRef);
			break;
		}

		case B_STAT_CHANGED:
			printf("%s MONITOR_ B_STAT_CHANGED\n", info);
			break;

		case B_ATTR_CHANGED:
			printf("%s MONITOR_ B_ATTR_CHANGED\n", info);
			break;

		case B_DEVICE_MOUNTED:
			printf("%s MONITOR_ B_DEVICE_MOUNTED\n", info);
			break;

		case B_DEVICE_UNMOUNTED:
			printf("%s MONITOR_ B_DEVICE_UNMOUNTED\n", info);
			break;

		default:
			printf("%s MONITOR_ UNMANAGED OPCODE\n", info);
			//BApplication::MessageReceived(message);
			break;
	}
}


void
HighLevelQuery::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case B_QUERY_UPDATE:
			_QueryUpdate(message);
			break;

		case B_NODE_MONITOR:
			_NodeMonitorUpdate(message);
			break;

		default:
			printf("HighLevelQuery::MessageReceived unknown message\n");
			//BApplication::MessageReceived(message);
			break;
	}
}


void
HighLevelQuery::AddListener(HighLevelQueryListener* listener)
{
	// TODO some checking
	fListeners.push_back(listener);
}



void
HighLevelQuery::_NotifyEntryAdded(const entry_ref& entry)
{
	ListenerList::iterator it = fListeners.begin();
	for(; it != fListeners.end(); it++)
		(*it)->EntryAdded(entry);
}


void
HighLevelQuery::_NotifyEntryRemoved(const entry_ref& entry)
{
	ListenerList::iterator it = fListeners.begin();
	for(; it != fListeners.end(); it++)
		(*it)->EntryRemoved(entry);
}


void
HighLevelQuery::_NotifyEntryChanged(const entry_ref& entry)
{
	ListenerList::iterator it = fListeners.begin();
	for(; it != fListeners.end(); it++)
		(*it)->EntryChanged(entry);
}


void
HighLevelQuery::Perform()
{
	/*status_t status = query.Fetch();
	if (status != B_OK) {
		fprintf(stderr, "%s: bad query expression\n", kProgramName);
		return;
	}*/

	int32 count = 0;
	bigtime_t startTime = system_time();

	/*BEntry entry;
	BPath path;
	while (query.GetNextEntry(&entry) == B_OK) {
		if (sFilesOnly && !entry.IsFile())
			continue;

		if (entry.GetPath(&path) != B_OK) {
			fprintf(stderr, "%s: could not get path for entry\n", kProgramName);
			continue;
		}

		entry_ref ref;
		entry.GetRef(&ref); //TODO check error
		_ManageEntry(ref);

		count++;
	}*/

	BDirectory directory("/boot/home/Desktop/tracker2test");// "/system/apps");//  /Data2/mail/Haiku-bugs
	status_t error = directory.InitCheck();

	if (error != B_OK)
		printf("directory.InitCheck error=%s\n", strerror(error));

	while (true) {
		entry_ref ref;
		status_t status = directory.GetNextRef(&ref);
		if (status != B_OK)
			break;

		_ManageEntry(ref);

		count++;
	}

	node_ref ref;
	directory.GetNodeRef(&ref);
	status_t status = watch_node(&ref, B_WATCH_ALL, this);
	printf("Watching directory, status=%s\n", strerror(status));

	bigtime_t deltaTime = system_time() - startTime;
	printf("FOUND %ld entries, time %llims (%fms/kEntry)\n", count, deltaTime / 1000, (float)deltaTime / (float)count);
}
