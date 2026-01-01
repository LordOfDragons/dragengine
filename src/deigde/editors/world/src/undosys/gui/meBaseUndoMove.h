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

#ifndef _MEBASEUNDOMOVE_H_
#define _MEBASEUNDOMOVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Base class for undo action moving things around.
 */
class meBaseUndoMove : public igdeUndo{
public:
	typedef deTObjectReference<meBaseUndoMove> Ref;
	typedef decTObjectOrderedSet<meBaseUndoMove> List;
	
	
private:
	decDVector pDistance;
	
	bool pModifyOrientation;
	decDMatrix pMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	meBaseUndoMove();
	
	/** \brief Clean up undo action. */

protected:
	virtual ~meBaseUndoMove();

public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Distance to move. */
	inline const decDVector &GetDistance() const{ return pDistance; }
	
	/** \brief Set distance to move. */
	void SetDistance(const decDVector &distance);
	
	/** \brief Orientation modification enabled. */
	inline bool GetModifyOrientation() const{ return pModifyOrientation; }
	
	/** \brief Set if orientation modification is enabled. */
	void SetModifyOrientation(bool modify);
	
	/** \brief Transformation matrix used only if orientation is modified. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Set transformation matrix used only if orientation is modified. */
	void SetMatrix(const decDMatrix &matrix);
	
	
	
	/**
	 * \brief Convenience method to transform element.
	 * 
	 * Position and orientation are modified in place.
	 */
	void TransformElement(decDVector &position, decDVector &rotation);
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
