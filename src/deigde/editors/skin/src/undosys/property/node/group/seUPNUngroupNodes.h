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

#ifndef _SEUPNUNGROUPNODES_H_
#define _SEUPNUNGROUPNODES_H_

#include "../../../../skin/property/node/sePropertyNodeList.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class sePropertyNodeGroup;



/**
 * \brief Undo action undgroup property nodes.
 */
class seUPNUngroupNodes : public igdeUndo{
private:
	struct sNode {
		sePropertyNode *node;
		decPoint3 position;
		decPoint3 size;
		float rotation;
		float shearing;
	};
	
	sePropertyNodeGroup *pParentGroup;
	sePropertyNodeGroup *pNodeGroup;
	int pIndex;
	int pNodeCount;
	sNode *pNodes;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seUPNUngroupNodes> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPNUngroupNodes(sePropertyNodeGroup *nodeGroup);
	
protected:
	/** \brief Clean up undo. */
	virtual ~seUPNUngroupNodes();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
