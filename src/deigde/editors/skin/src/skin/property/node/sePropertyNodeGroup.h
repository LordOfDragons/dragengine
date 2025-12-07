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

#ifndef _SEPROPERTYNODEGROUP_H_
#define _SEPROPERTYNODEGROUP_H_

#include "sePropertyNode.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>



/**
 * \brief Texture property group node.
 */
class sePropertyNodeGroup : public sePropertyNode{
private:
	decObjectOrderedSet pNodes;
	seProperty *pProperty;
	bool pActiveGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNodeGroup(deEngine &engine);
	
	/** \brief Create copy of node. */
	sePropertyNodeGroup(const sePropertyNodeGroup &node);
	
	/** \brief Clean up node. */
	~sePropertyNodeGroup() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent property or \em NULL if not present. */
	seProperty *GetProperty() const override;
	
	/** \brief Set parent property or \em NULL. */
	void SetProperty(seProperty *property);
	
	/** \brief Node group is the active one. */
	inline bool GetActiveGroup() const{ return pActiveGroup; }
	
	/** \brief Set if node group is the active one. */
	void SetActiveGroup(bool active);
	
	
	
	/** \brief Number of nodes. */
	int GetNodeCount() const;
	
	/** \brief Node at index. */
	sePropertyNode *GetNodeAt(int index) const;
	
	/** \brief Node is present. */
	bool HasNode(sePropertyNode *node) const;
	
	/** \brief Index of node or -1 if absent. */
	int IndexOfNode(sePropertyNode *node) const;
	
	/** \brief Add node. */
	void AddNode(sePropertyNode *node);
	
	/** \brief Insert node at index. */
	void InsertNode(int index, sePropertyNode *node);
	
	/** \brief Remove node. */
	void RemoveNode(sePropertyNode *node);
	
	/** \brief Remove all nodes. */
	void RemoveAllNodes();
	
	/** \brief Move node to position. */
	void MoveNode(sePropertyNode *node, int index);
	
	
	
	/** \brief Create copy of node. */
	sePropertyNode *Copy() const override;
	
	/** \brief Update resources. */
	void UpdateResources() override;
	/*@}*/
};

#endif
