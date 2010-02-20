/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _HIGH_LEVEL_QUERY_H
#define _HIGH_LEVEL_QUERY_H

#include "HighLevelQueryListener.h"

#include <Entry.h>
#include <Handler.h>
#include <Node.h>
#include <Rect.h>
#include <String.h>
//#include <SupportDefs.h>

#include <map>
#include <vector>

#include "NaturalCompare.h"	// test

class HighLevelQueryListener;


class HighLevelQueryResult {
public:
	node_ref		nodeRef;
	entry_ref		entryRef;	// we always want that
	
	// visibility info
	uint32			lastRank;
	uint32			rank;		// if we asked for sorted results
	BRect			frame;		// or manual position (any size)
	
	bool operator<(const HighLevelQueryResult& other) const {
		return 	strcmp(entryRef.name, other.entryRef.name) < 0;
	};
};


class HighLevelQuery : public BHandler {
public:
								HighLevelQuery();
	virtual						~HighLevelQuery();	
	
			void				DoIt();

	virtual void				MessageReceived(BMessage* message);
			void				AddListener(HighLevelQueryListener* listener);
			
			
			void				InvertSort();		// test
			
			
			enum {
				HLQ_FULL_UPDATE = 0,
				HLQ_RESULT_ADDED,
				HLQ_RESULT_REMOVED,
				HLQ_RESULT_CHANGED
			};			
			
			
			
			typedef std::vector<HighLevelQueryResult> ResultVector;
			
	/*const*/	ResultVector&	Results() { return fResults; };
	
			
			

private:
			//void				_AddQuery(BVolume& volume,
			//						const char* predicate);									
			//void				_AddFilter(...);									

			void				_NotifyEvent(uint32 code, /*const*/ HighLevelQueryResult* result);

			void				_QueryUpdate(BMessage* message);
			void				_NodeMonitorUpdate(BMessage* message);

			void				_ManageEntry(const entry_ref& entry);
			void				_UnmanageEntry(const node_ref& node);
			void				_UpdateEntry(const node_ref& nodeRef,
									const entry_ref& entry);
									
			void				_Sort();
									
			struct	lesserThanRefNode {
				bool operator()(const node_ref & a, const node_ref & b)
				{
					return a.device < b.device
						|| (a.device == b.device && a.node < b.node);
				}
			};
			
			typedef std::map<node_ref, entry_ref, lesserThanRefNode> EntryMap;
			EntryMap			fEntries;			
			
			ResultVector		fResults;
			bool (*fSortFunction) (const HighLevelQueryResult&, const HighLevelQueryResult&);		
			
			typedef std::vector<HighLevelQueryListener*> ListenerList;
			ListenerList		fListeners;
			
			uint32				fEntryCount; //debug
			bool				fDebug;
			
			bool				fInvertSortToggle;
};

#endif	// _HIGH_LEVEL_QUERY_H
