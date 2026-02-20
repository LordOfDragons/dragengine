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

#ifndef _DEOGLMODELOCTREEVISITOR_H_
#define _DEOGLMODELOCTREEVISITOR_H_

#include "../../utils/octree/deoalOctreeVisitor.h"

class deoalModelFace;
class deoalModelOctree;



/**
 * \brief Model octree visitor.
 */
class deoalModelOctreeVisitor : public deoalOctreeVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model octree visitor. */
	deoalModelOctreeVisitor();
	
	/** \brief Clean up model octree visitor. */
	~deoalModelOctreeVisitor() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * \brief Visit octree node.
	 * 
	 * Visitor is responsible to check if the node and children thereof have to be
	 * visited. This allows to apply special checks not handled by deoalOctree.
	 * 
	 * To visit model octree call VisitNode(*model.GetOctree()).
	 * 
	 * The default implementation calls VisitNode(deoalOctree*,int) then visits child nodes.
	 */
	virtual void VisitNode(deoalModelOctree &node);
	
	/**
	 * \brief Visit octree node.
	 * 
	 * Default implementation visits all faces stored in node.
	 */
	void VisitNode(deoalOctree *node, int intersection) override;
	
	/** \brief Visit face. */
	virtual void VisitFace(deoalModelFace *face);
	/*@}*/
};

#endif
