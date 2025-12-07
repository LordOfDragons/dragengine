/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _SEPROPERTYNODELIST_H_
#define _SEPROPERTYNODELIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class sePropertyNode;



/**
 * \brief Texture property node list.
 */
class sePropertyNodeList{
private:
	decObjectOrderedSet pNodes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node list. */
	sePropertyNodeList();
	
	/** \brief Create copy of node. */
	sePropertyNodeList(const sePropertyNodeList &node);
	
	/** \brief Clean up node. */
	~sePropertyNodeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of nodes. */
	int GetCount() const;
	
	/** \brief Node at index. */
	sePropertyNode *GetAt(int index) const;
	
	/** \brief Node is present. */
	bool Has(sePropertyNode *node) const;
	
	/** \brief Index of node or -1 if absent. */
	int IndexOf(sePropertyNode *node) const;
	
	/** \brief Add node. */
	void Add(sePropertyNode *node);
	
	/** \brief Add node if absent. */
	void AddIfAbsent(sePropertyNode *node);
	
	/** \brief Insert node at index. */
	void Insert(int index, sePropertyNode *node);
	
	/** \brief Remove node. */
	void Remove(sePropertyNode *node);
	
	/** \brief Remove node if present. */
	void RemoveIfPresent(sePropertyNode *node);
	
	/** \brief Remove all nodes. */
	void RemoveAll();
	
	/** \brief Move node to position. */
	void Move(sePropertyNode *node, int index);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	
	/** \brief Copy list. */
	sePropertyNodeList &operator=(const sePropertyNodeList &list);
	/*@}*/
};

#endif
