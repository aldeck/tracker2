/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _NODE_MODEL_H
#define _NODE_MODEL_H

/*class AttributeModel {
	BString	name;
	BString	value;
};*/

class NodeModel {
public:
	// on-disk entity
	node_ref		nodeRef;
	
	// cached info
	entry_ref		entryRef;	// we always want that
	
	// visibility info
	uint32			rank;		// if we asked for sorted results
	BRect			frame;		// or manual position (any size)
};

#endif	// _NODE_MODEL_H
