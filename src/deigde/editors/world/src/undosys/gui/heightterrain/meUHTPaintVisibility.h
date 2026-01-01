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
#ifndef _MEUHTPAINTVISIBILITY_H_
#define _MEUHTPAINTVISIBILITY_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meBitArray;



/**
 * \brief Undo Height Terrain Visibility Painting.
 *
 * Undo action to undo and redo visibility painting on a height terrain.
 */
class meUHTPaintVisibility : public igdeUndo{
public:
	typedef deTObjectReference<meUHTPaintVisibility> Ref;
	
	
private:
	meWorld *pWorld;
	
	decBoundary pSector;
	decBoundary pGrid;
	decPoint pSize;
	
	meBitArray *pOldVis;
	meBitArray *pNewVis;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintVisibility(int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid, const decPoint &size, meBitArray *oldVis);
	/** \brief Clean up object. */

protected:
	virtual ~meUHTPaintVisibility();

public:
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
	void pSaveVisibility();
	void pRestoreVisibility(meBitArray *vis);
};

// end of include only once
#endif
