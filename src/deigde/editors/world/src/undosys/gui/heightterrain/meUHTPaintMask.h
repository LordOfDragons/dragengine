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
#ifndef _MEUHTPAINTMASK_H_
#define _MEUHTPAINTMASK_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;



/**
 * \brief Undo Mask Terrain Mask Painting.
 *
 * Undo action to undo and redo mask painting on a height terrain.
 */
class meUHTPaintMask : public igdeUndo{
private:
	meWorld *pWorld;
	meHeightTerrainSector *pSector;
	meHeightTerrainTexture *pTexture;
	
	decBoundary pGrid;
	decPoint pSize;
	
	unsigned char *pOldValues;
	unsigned char *pNewValues;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTPaintMask( int drawMode, meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture,
		const decPoint &grid, const decPoint &size, unsigned char *oldValues );
	/** \brief Clean up object. */
	virtual ~meUHTPaintMask();
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
	void pSaveValues();
	void pRestoreValues( unsigned char *values );
};

// end of include only once
#endif
