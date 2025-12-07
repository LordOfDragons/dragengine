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
#ifndef _MEBASEUNDOSCALE_H_
#define _MEBASEUNDOSCALE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"



/**
 * Base class for undo actions scaling things.
 */
class meBaseUndoScale : public igdeUndo{
private:
	bool pModifyPosition;
	bool pModifySize;
	decVector pFactors;
	float pUniformFactor;
	decDVector pPivot;
	bool pScaleUniform;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meBaseUndoScale> Ref;
	
	// constructor, destructor
	meBaseUndoScale();
	~meBaseUndoScale();
	
	// management
	/** Determines if the position has to be modified. */
	inline bool GetModifyPosition() const{return pModifyPosition;}
	/** Sets if the position has to be modified. */
	void SetModifyPosition(bool modifyPosition);
	/** Determines if the size has to be modified. */
	inline bool GetModifySize() const{return pModifySize;}
	/** Sets if the size has to be modified. */
	void SetModifySize(bool modifySize);
	/** Retrieves the scaling factors. */
	inline const decVector &GetFactors() const{return pFactors;}
	/** Sets the scaling factors. */
	void SetFactors(const decVector &factors);
	/** Retrieves the pivot point. */
	inline const decDVector &GetPivot() const{return pPivot;}
	/** Sets the pivot point. */
	void SetPivot(const decDVector &pivot);
	
	/** Retrieves the uniform scaling factor. */
	inline float GetUniformFactor() const{return pUniformFactor;}
	/** Sets the uniform scaling factor. */
	void SetUniformFactor(float factor);
	/** Determines if the scaling has to be done uniformly. */
	inline bool GetScaleUniform() const{return pScaleUniform;}
	/** Sets if the scaling has to be done uniformly. */
	void SetScaleUniform(bool scaleUniform);
	
	void Update();
	
	/**
	 * Convenience method to scale one undo element. The position and
	 * scaling are modified in place.
	 */
	void TransformElement(decDVector &position, decVector &scaling);
	
	// undo and redo actions
	virtual void ProgressiveRedo();
};

// end of include only once
#endif
