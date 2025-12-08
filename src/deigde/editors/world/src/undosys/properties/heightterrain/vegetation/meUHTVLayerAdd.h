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
#ifndef _MEUHTVLAYERADD_H_
#define _MEUHTVLAYERADD_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
#include "../../../../world/meWorld.h"
class meHeightTerrain;
#include "../../../../world/heightterrain/meHTVegetationLayer.h"



/**
 * \brief Undo Add Height Terrain Vegetation Layer.
 *
 * Undo action to add a vegetation layer to a height terrain heightTerrain.
 */
class meUHTVLayerAdd : public igdeUndo{
private:
	meWorld::Ref pWorld;
	meHeightTerrain *pHeightTerrain;
	meHTVegetationLayer::Ref pVLayer;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meUHTVLayerAdd> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUHTVLayerAdd(meWorld *world, meHeightTerrain *heightTerrain, meHTVegetationLayer *vlayer);
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUHTVLayerAdd();
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
