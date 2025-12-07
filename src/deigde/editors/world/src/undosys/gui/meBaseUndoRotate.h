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

// include only once
#ifndef _MEBASEUNDOROTATE_H_
#define _MEBASEUNDOROTATE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"



/**
 * Base class for undo actions rotating things around.
 */
class meBaseUndoRotate : public igdeUndo{
private:
	float pAngle;
	decDVector pPivot;
	decDVector pAxis;
	bool pModifyPosition;
	bool pModifyOrientation;
	decDMatrix pMatrix;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meBaseUndoRotate> Ref;
	
	// constructor, destructor
	meBaseUndoRotate();
	~meBaseUndoRotate();
	
	// management
	/** Retrieves the angle to rotate in degrees. */
	inline float GetAngle() const{return pAngle;}
	/** Sets the angle to rotate in degrees. */
	void SetAngle(float angle);
	/** Retrieves the pivot point. */
	inline const decDVector &GetPivot() const{return pPivot;}
	/** Sets the pivot point. */
	void SetPivot(const decDVector &pivot);
	/** Retrieves the rotation axis. */
	inline const decDVector &GetAxis() const{return pAxis;}
	/** Sets the rotation axis. */
	void SetAxis(const decDVector &axis);
	
	/** Determines if the position has to be modified. */
	inline bool GetModifyPosition() const{return pModifyPosition;}
	/** Sets if the position has to be modified. */
	void SetModifyPosition(bool modifyPosition);
	/** Determines if the orientation has to be modified. */
	inline bool GetModifyOrientation() const{return pModifyOrientation;}
	/** Sets if the orientation has to be modified. */
	void SetModifyOrientation(bool modifyOrientation);
	
	/** Retrieves the rotation matrix. */
	inline const decDMatrix &GetRotationMatrix() const{return pMatrix;}
	/** Updates the rotation matrix. */
	void UpdateRotationMatrix();
	
	/**
	 * Convenience method to rotate one undo element. The position and
	 * rotation are modified in place.
	 */
	void TransformElement(decDVector &position, decDVector &rotation);
	
	// undo and redo actions
	virtual void ProgressiveRedo();
};

// end of include only once
#endif
