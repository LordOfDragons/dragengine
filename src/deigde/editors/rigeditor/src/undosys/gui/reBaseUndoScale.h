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

#ifndef _REBASEUNDOSCALE_H_
#define _REBASEUNDOSCALE_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Base class for undo actions scaling things.
 */
class reBaseUndoScale : public igdeUndo{
private:
	bool pModifyPosition;
	bool pModifySize;
	decVector pFactors;
	decVector pCenter;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reBaseUndoScale> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reBaseUndoScale();
	
protected:
	virtual ~reBaseUndoScale();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline bool GetModifyPosition() const{ return pModifyPosition; }
	void SetModifyPosition(bool modifyPosition);
	inline bool GetModifySize() const{ return pModifySize; }
	void SetModifySize(bool modifySize);
	inline const decVector &GetFactors() const{ return pFactors; }
	void SetFactors(const decVector &factors);
	inline const decVector &GetCenter() const{ return pCenter; }
	void SetCenter(const decVector &center);
	
	void Update();
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
