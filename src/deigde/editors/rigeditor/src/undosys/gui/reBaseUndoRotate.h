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

#ifndef _REBASEUNDOROTATE_H_
#define _REBASEUNDOROTATE_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>



/**
 * Base class for undo actions rotating things around.
 */
class reBaseUndoRotate : public igdeUndo{
private:
	float pAngle;
	decPoint3 pCenterSector;
	decVector pCenterPosition, pAxis;
	bool pModifyPosition;
	bool pModifyOrientation;
	decMatrix pMatrix;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reBaseUndoRotate> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reBaseUndoRotate();
	
protected:
	/** \brief Clean up undo. */
	virtual ~reBaseUndoRotate();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline float GetAngle() const{return pAngle;}
	void SetAngle(float angle);
	inline const decPoint3 &GetCenterSector() const{return pCenterSector;}
	void SetCenterSector(const decPoint3 &sector);
	inline const decVector &GetCenterPosition() const{return pCenterPosition;}
	void SetCenterPosition(const decVector &position);
	inline const decVector &GetAxis() const{return pAxis;}
	void SetAxis(const decVector &axis);
	inline bool GetModifyPosition() const{return pModifyPosition;}
	void SetModifyPosition(bool modifyPosition);
	inline bool GetModifyOrientation() const{return pModifyOrientation;}
	void SetModifyOrientation(bool modifyOrientation);
	inline const decMatrix &GetRotationMatrix() const{return pMatrix;}
	void Update();
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
