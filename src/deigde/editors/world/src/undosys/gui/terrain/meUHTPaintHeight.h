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
#ifndef _MEUHTPAINTHEIGHT_H_
#define _MEUHTPAINTHEIGHT_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;



/**
 * \brief Undo Height Terrain Height Painting.
 *
 * Undo action to undo and redo height painting on a height terrain.
 */
class meUHTPaintHeight : public igdeUndo{
private:
	meWorld *pWorld;
	
	decBoundary pSector;
	decBoundary pGrid;
	decPoint pSize;
	
	float *pOldHeights;
	float *pNewHeights;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUHTPaintHeight> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintHeight( int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid, const decPoint &size, float *oldHeights );
	/** \brief Clean up object. */
	virtual ~meUHTPaintHeight();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
	void pSaveHeights();
	void pRestoreHeights( float *heights );
};

// end of include only once
#endif
