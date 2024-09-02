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

#ifndef _MEUNDODATANAVSPACE_H_
#define _MEUNDODATANAVSPACE_H_

#include <dragengine/common/math/decMath.h>

class meNavigationSpace;



/**
 * \brief Object data for undo actions.
 * 
 * Stores information about an navigation space suitable for undo actions. The
 * navigation space is stored as a pointer which is either owned or not. If the
 * navigation space is owned it will be freed once this navigation space is freed.
 * If not owned the navigation space is just a pointer which will not be freed.
 * By default the navigation space is not owned.
 */
class meUndoDataNavSpace{
private:
	meNavigationSpace *pNavSpace;
	decDVector pOldPosition;
	decVector pOldOrientation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation space undo data. */
	meUndoDataNavSpace( meNavigationSpace *navspace );
	
	/** \brief Clean up navigation space undo data. */
	virtual ~meUndoDataNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Navigation space. */
	inline meNavigationSpace *GetNavSpace() const{ return pNavSpace; }
	
	/** \brief Position before action. */
	inline const decDVector &GetOldPosition() const{ return pOldPosition; }
	
	/** \brief Orientation before action. */
	inline const decVector &GetOldOrientation() const{ return pOldOrientation; }
	/*@}*/
};

#endif
