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



/**
 * Texture property group node.
 */
class sePropertyNodeGroup : public sePropertyNode{
public:
	typedef deTObjectReference<sePropertyNodeGroup> Ref;
	
	
private:
	sePropertyNode::List pNodes;
	seProperty *pProperty;
	bool pActiveGroup;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create node. */
	explicit sePropertyNodeGroup(const deEngine &engine);
	
	/** Create copy of node. */
	sePropertyNodeGroup(const sePropertyNodeGroup &node);
	
	/** Clean up node. */
protected:
	~sePropertyNodeGroup() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent property or \em nullptr if not present. */
	seProperty *GetProperty() const override;
	
	/** Set parent property or \em nullptr. */
	void SetProperty(seProperty *property);
	
	/** Node group is the active one. */
	inline bool GetActiveGroup() const{ return pActiveGroup; }
	
	/** Set if node group is the active one. */
	void SetActiveGroup(bool active);
	
	
	
	/** Nodes. */
	inline const sePropertyNode::List &GetNodes() const{ return pNodes; }
	
	/** Add node. */
	void AddNode(sePropertyNode *node);
	
	/** Insert node at index. */
	void InsertNode(int index, sePropertyNode *node);
	
	/** Remove node. */
	void RemoveNode(sePropertyNode *node);
	
	/** Remove all nodes. */
	void RemoveAllNodes();
	
	/** Move node to position. */
	void MoveNode(sePropertyNode *node, int index);
	
	
	
	/** Create copy of node. */
	sePropertyNode::Ref Copy() const override;
	
	/** Update resources. */
	void UpdateResources() override;
	/*@}*/
};

#endif
