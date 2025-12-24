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

#ifndef _SEUPROPERTYNODESRESIZE_H_
#define _SEUPROPERTYNODESRESIZE_H_

#include "seUPropertyNodeData.h"
#include "../../../../skin/property/node/sePropertyNode.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>



/**
 * \brief Undo action property resize nodes.
 */
class seUPropertyNodesResize : public igdeUndo{
public:
	typedef deTObjectReference<seUPropertyNodesResize> Ref;
	
	
private:
	seUPropertyNodeData::List pNodes;
	
	decVector2 pPivot;
	decVector2 pOrigin;
	decVector2 pDistance;
	float pRotation;
	float pShearing;
	decTexMatrix2 pTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertyNodesResize(const sePropertyNode::List &nodes, const decVector2 &pivot,
		const decVector2 &origin, float rotation, float shearing);
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPropertyNodesResize();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set distance. */
	void SetDistance(const decVector2 &distance);
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pAddNodes(sePropertyNode *node);
};

#endif
