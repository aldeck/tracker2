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

#include <map>
#include <vector>

class HighLevelQueryListener;


class HighLevelQuery : public BHandler {
public:
								HighLevelQuery();
	virtual						~HighLevelQuery();	

	virtual void				MessageReceived(BMessage* message);
			void				AddListener(HighLevelQueryListener* listener);

private:
			//void				_AddQuery(BVolume& volume,
			//						const char* predicate);									
			//void				_AddFilter(...);
									
			void				_Perform();
			
			void				_NotifyEntryAdded(const entry_ref& entry);
			void				_NotifyEntryRemoved(const entry_ref& entry);
			void				_NotifyEntryChanged(const entry_ref& entry);
			

			void				_QueryUpdate(BMessage* message);
			void				_NodeMonitorUpdate(BMessage* message);

			void				_ManageEntry(const entry_ref& entry);
			void				_UnmanageEntry(const node_ref& node);
			void				_UpdateEntry(const node_ref& nodeRef,
									const entry_ref& entry);
									
			struct	lesserThanRefNode {
				bool operator()(const node_ref & a, const node_ref & b)
				{
					return a.device < b.device
						|| (a.device == b.device && a.node < b.node);
				}
			};

			typedef std::map<node_ref, entry_ref, lesserThanRefNode> EntryMap;
			EntryMap			fEntries;
			
			typedef std::vector<HighLevelQueryListener*> ListenerList;
			ListenerList		fListeners;
			
			uint32				fEntryCount; //debug
};

#endif
