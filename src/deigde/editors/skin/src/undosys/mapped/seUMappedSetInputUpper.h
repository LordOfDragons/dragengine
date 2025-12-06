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

#ifndef _SEUMAPPEDSETINPUTUPPER_H_
#define _SEUMAPPEDSETINPUTUPPER_H_

#include "../../skin/mapped/seMapped.h"

#include <deigde/undo/igdeUndo.h>



/**
 * Undo action set mapped input upper.
 */
class seUMappedSetInputUpper : public igdeUndo{
private:
	const seMapped::Ref pMapped;
	
	const float pOldInputUpper;
	const float pNewInputUpper;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seUMappedSetInputUpper> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	seUMappedSetInputUpper( seMapped *mapped, float newInputUpper );
	
protected:
	/** Clean up undo. */
	virtual ~seUMappedSetInputUpper();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo action. */
	virtual void Undo();
	
	/** Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
