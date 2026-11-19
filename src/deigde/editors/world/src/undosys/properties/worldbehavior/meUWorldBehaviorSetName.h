/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _MEUWORLDBEHAVIORSETNAME_H_
#define _MEUWORLDBEHAVIORSETNAME_H_

#include "../../../world/meWorldBehavior.h"

#include <deigde/undo/igdeUndo.h>


/**
 * \brief Undo set world behavior name.
 */
class meUWorldBehaviorSetName : public igdeUndo{
public:
	using Ref = deTObjectReference<meUWorldBehaviorSetName>;
	
	
private:
	meWorldBehavior::Ref pBehavior;
	decString pOldValue, pNewValue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	meUWorldBehaviorSetName(meWorldBehavior *behavior, const char *newValue);
	
protected:
	/** Clean up undo object. */
	~meUWorldBehaviorSetName() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	void Undo() override;
	
	/** Redo. */
	void Redo() override;
	/*@}*/
};

#endif
