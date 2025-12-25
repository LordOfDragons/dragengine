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

#ifndef _SAEUSANIMSETNEUTRALVERTEXPOSITIONSETS_H_
#define _SAEUSANIMSETNEUTRALVERTEXPOSITIONSETS_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/string/decStringSet.h>

#include "../../sanimation/saeSAnimation.h"


/**
 * Undo action set neutral vertex position sets.
 */
class saeUSAnimSetNeutralVertexPositionSets : public igdeUndo{
public:
	typedef deTObjectReference<saeUSAnimSetNeutralVertexPositionSets> Ref;
	
	
private:
	saeSAnimation::Ref pSAnimation;
	
	decStringSet pOldSets;
	decStringSet pNewSets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	saeUSAnimSetNeutralVertexPositionSets(saeSAnimation *sanimation, const decStringSet &sets);
	
protected:
	/** Clean up undo. */
	virtual ~saeUSAnimSetNeutralVertexPositionSets();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
