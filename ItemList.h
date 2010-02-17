/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _ITEM_LIST_H
#define _ITEM_LIST_H


#include <vector>
#include <set>

class Item;

typedef std::vector<Item*> ItemList;
typedef std::set<Item*> ItemSet;

#endif /* _ITEM_LIST_H */
