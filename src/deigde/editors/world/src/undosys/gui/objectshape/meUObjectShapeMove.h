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

#ifndef _MEUOBJECTSHAPEMOVE_H_
#define _MEUOBJECTSHAPEMOVE_H_

#include "../meBaseUndoMove.h"
#include "../../../world/object/meObject.h"
#include "../../../world/objectshape/meObjectShape.h"

#include <dragengine/common/collection/decTList.h>



/**
 * \brief Move object shapes undo action.
 */
class meUObjectShapeMove : public meBaseUndoMove{
public:
	typedef deTObjectReference<meUObjectShapeMove> Ref;
	
private:
	meObject::Ref pObject;
	decString pProperty;
	
	bool pPropertyExists;
	decString pOldValue;
	decTList<int> pList;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectShapeMove(meObject *object, const char *property, const meObjectShape::List &list);
	
protected:
	/** \brief Clean up undo object. */
	~meUObjectShapeMove() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/** Progressive redo the action. */
	void ProgressiveRedo() override;
};

#endif
