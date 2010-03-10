/*
 * Copyright 2010, Haiku Inc. All Rights Reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Alexandre Deckner, alex@zappotek.com
 */
 
#include "HighLevelQuery.h"

#include <Directory.h>
#include <fs_attr.h>
#include <NodeMonitor.h>
#include <Path.h>
#include <String.h>

#include <algorithm>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/stat.h>


static bool sVerbose = false;

//#define NDEBUG

// test
bool _BiggerThan(const HighLevelQueryResult* a, const HighLevelQueryResult* b)
{
	assert(a != NULL && b != NULL);
	return !(*a < *b);
}

bool _LesserThan(const HighLevelQueryResult* a, const HighLevelQueryResult* b)
{
	assert(a != NULL && b != NULL);
	return *a < *b;
}


HighLevelQuery::HighLevelQuery()
{
	fDebug = false;
	
	
	// bump node monitor limit
	//int32 DEFAULT_MON_NUM = 4096;	// copied from fsil.c
	struct rlimit rl;
	rl.rlim_cur = 16000;
	rl.rlim_max = RLIM_SAVED_MAX;
	if (setrlimit(RLIMIT_NOVMON, &rl) < 0)
		printf("error bumping node monitor limit err=%s\n", strerror(errno));
	else
		printf("ok bumped node monitor to 16000\n");
		
	fInvertSortToggle = false;
	fSortFunction = _LesserThan;
}


HighLevelQuery::~HighLevelQuery()
{
}

static int foo = 0;

void
HighLevelQuery::_ManageEntry(const entry_ref& entryRef)
{
	
	
	BNode node(&entryRef);
	status_t err2 = node.InitCheck();
	if (err2 != B_OK) {
		printf("error opening node err=%s\n", strerror(err2));
		return;
	} else {
		node_ref nodeRef;
   		//node.GetNodeRef(&nodeRef);
   			// instead get node_ref directly via stat below, since we will probably need later it anyway
   		
		struct stat nodeStat;		
		status_t err4 = node.GetStat(&nodeStat);
		if (err4 == B_OK) {
			nodeRef.device  = nodeStat.st_dev;
			nodeRef.node = nodeStat.st_ino;
		} else
			printf("error cant get stat for node err=%s\n", strerror(err4));
		
   		
		// debug check
		EntryMap::iterator it = fEntries.find(nodeRef);
		if (it != fEntries.end()) {
			printf("HighLevelQuery::_ManageEntry, already managed node=%lli!!\n", nodeRef.node);
			return;
		}

		fEntries.insert(EntryMap::value_type(nodeRef, entryRef));

		// needed for the "query entry rename" problem
		// attention on recoit deux notifs sur un rename dans un directory, probablement
		// le watch sur le fichier et sur le repertoire
		uint32 flags = /*B_WATCH_NAME |*/ B_WATCH_STAT | B_WATCH_ATTR;
		status_t err = watch_node(&nodeRef, flags, this);
		fEntryCount++;
		if (sVerbose || err != B_OK) {
			BPath path(&entryRef);
			printf("%lu HighLevelQuery::_ManageEntry (%lli, %s). Start watching err=%s\n", fEntryCount,
				nodeRef.node, path.Path(), strerror(err));
		}
		
		// test, read some attributes
		
		//node.RewindAttrs();
		/*char attrName[B_ATTR_NAME_LENGTH];
		//printf("%s [ ", entryRef.name);
		uint32 totalSize = 0;
		while (node.GetNextAttrName(attrName) == B_OK) {
   			//printf("%s ", attrName);
   			attr_info info;
   			if (node.GetAttrInfo(attrName, &info) == B_OK) {
   				// read all attributes
   				char attrData[2048];
   				if (info.size < 2048) {
	   				ssize_t read = node.ReadAttr(attrName, (type_code)0, (off_t)0, attrData, info.size);
	   				//printf("%lu/%lu ", read, info.size);
	   				//totalSize += info.size;
	   				foo += read;
	   			} else {
	   				printf("toobig!=%lu ", info.size);
	   			}	   			
   			} else {
	   			printf("noattrinfo");
	   		}
		}
		//printf(" ] total %lu\n", totalSize);*/
		
		// test, read one attribute
		
		//printf("%s [ ", entryRef.name);
		//printf("[ ");		
		BString attribute;
		/*char *attributeName = "MAIL:from";
		status_t err3 = node.ReadAttrString(attributeName, &attribute);
		if (err3 != B_OK)  				
			printf("can't read attribute %s\n", attributeName);*/
		
		attribute = entryRef.name;
			
		HighLevelQueryResult* result = new HighLevelQueryResult(); //todo nothrow
		result->nodeRef = nodeRef;
		result->entryRef = entryRef;
		result->stringAttribute = attribute;
		//fSortedResults.insert(StringSortedResultMap::value_type(attribute.ToLower(), nodeModel));

		fResults.push_back(result);
		//_NotifyEvent(HLQ_RESULT_ADDED, &result);
		
		//printf(" ]\n");	
		
		// test, read icon
		/*attr_info info;
		if (node.GetAttrInfo("BEOS:ICON", &info) == B_OK) {			
			char attrData[2048];
			if (info.size < 2048) {
				ssize_t read = node.ReadAttr("BEOS:ICON", (type_code)0, (off_t)0, attrData, info.size);
				//printf("%lu/%lu \n", read, info.size);
				foo += read;				
			} else {
				printf("toobig!=%lu ", info.size);
			}	   			
   		}*/
		
		
		// le but final est de ne lire que les meta infos necessesaires au tri 
		// si on ne veut pas de tri alors pas besoin de lire quoi que ce soit
		// dans tous les cas le reste sera lu au moment de l'affichage
		
		// on peut ne pas vouloir de tri mais connaitre les positions custom pour
		// savoir ce qu'on doit afficher en premier
		
		// dans les deux cas c'est une information d'affichage
	
	}
}


void
HighLevelQuery::_UnmanageEntry(const node_ref& nodeRef)
{
	EntryMap::iterator it = fEntries.find(nodeRef);
	if (it != fEntries.end()) {		

		// needed for the "query entry rename" problem
		status_t err = watch_node(&nodeRef, B_STOP_WATCHING, this);

		if (sVerbose || err != B_OK) {
			BPath path(&it->second);
			printf("HighLevelQuery::_UnmanageEntry (%lli, %s), Stop watching B_WATCH_NAME err=%s\n",
				nodeRef.node, path.Path(), strerror(err));
		}
		
		//_NotifyEntryRemoved(nodeRef, (*it).second);		
		fEntries.erase(it);
				
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
		{
			printf("%s MONITOR_ B_ENTRY_CREATED\n", info);

			entry_ref entryRef;
			entryRef.device = device;
			entryRef.directory = directory;
			entryRef.set_name(name);

			_ManageEntry(entryRef);
			break;
		}
		case B_ENTRY_REMOVED:
		{
			printf("%s MONITOR_ B_ENTRY_REMOVED\n", info);

			node_ref nodeRef;
			nodeRef.device = device;
			nodeRef.node = node;

			_UnmanageEntry(nodeRef);
			break;
		}
		case B_ENTRY_MOVED:
		{
			// can be about:
			// a query node that we're monitoring and which name has changed
			// a file in a directory that we monitor that moved in or changed name
			// TODO: differentiate the cases

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
HighLevelQuery::_NotifyEvent(uint32 code, /*const*/ HighLevelQueryResult* result)
{
	ListenerList::iterator it = fListeners.begin();
	for(; it != fListeners.end(); it++)
		(*it)->HighLevelQueryEventReceived(code, result);
}


void
HighLevelQuery::InvertSort()		// test
{
	if (fInvertSortToggle)
		fSortFunction = _LesserThan;	
	else
		fSortFunction = _BiggerThan;
	fInvertSortToggle = !fInvertSortToggle;	
	_Sort();
}


void
HighLevelQuery::_Sort()		// full sort and rank
{
	bigtime_t sortStartTime = system_time();	
	std::sort(fResults.begin(), fResults.end(), fSortFunction);
	bigtime_t sortTime = system_time() - sortStartTime;
	
	bigtime_t rankStartTime = system_time();
	ResultVector::iterator it = fResults.begin();
	for (uint32 i = 0; i < fResults.size(); ++i) {
		fResults[i]->rank = i;
		if (fResults[i]->lastRank != i) {
			//_NotifyEvent(HLQ_RANK_CHANGED, &fResults[i]);
			fResults[i]->lastRank = i;
		}
	}
	bigtime_t rankTime = system_time() - rankStartTime;
	
	printf("HighLevelQuery sort %lliµs rank %lliµs\n", sortTime, rankTime);

	_NotifyEvent(HLQ_FULL_UPDATE, NULL);
}


void
HighLevelQuery::DoIt()
{
	int32 count = 0;
	bigtime_t queryStartTime = system_time();

	/*
	status_t status = query.Fetch();
	if (status != B_OK) {
		fprintf(stderr, "%s: bad query expression\n", kProgramName);
		return;
	}
	
	BEntry entry;
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

	BDirectory directory("/boot/system/apps");
	// /boot/home/Desktop/tracker2test   /system/apps   /Data2/mail/Haiku-bugs
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
	printf("HighLevelQuery Watching directory, status=%s\n", strerror(status));
	
	bigtime_t queryTime = system_time() - queryStartTime;
	printf("HighLevelQuery added %ld entries, fs %llims (%fms/kEntry)\n",
		count, queryTime / 1000, (float)queryTime / (float)count);
	
	_Sort();			
}
