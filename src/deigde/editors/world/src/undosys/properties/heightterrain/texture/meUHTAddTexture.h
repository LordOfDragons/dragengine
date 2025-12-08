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
#ifndef _MEUHTADDTEXTURE_H_
#define _MEUHTADDTEXTURE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "meHeightTerrainTexture.h"
#include "meWorld.h"

// predefinitions
class meHeightTerrainSector;



/**
 * \brief Undo Height Terrain Add Texture.
 *
 * Undo action to add a texture to a height terrain.
 */
class meUHTAddTexture : public igdeUndo{
private:
	meWorld::Ref pWorld;
	meHeightTerrainSector *pSector;
	meHeightTerrainTexture::Ref pTexture;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUHTAddTexture> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTAddTexture(meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture);
	
protected:
	/** \brief Clean up object. */
	virtual ~meUHTAddTexture();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
