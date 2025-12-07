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

#ifndef _DEOGLHEIGHTTERRAIN_H_
#define _DEOGLHEIGHTTERRAIN_H_

#include "deoglRHeightTerrain.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicHeightTerrain.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglWorld;
class deoglHTSector;
class deGraphicOpenGl;

class deHeightTerrain;



/**
 * Height terrain peer.
 */
class deoglHeightTerrain : public deBaseGraphicHeightTerrain{
private:
	deGraphicOpenGl &pOgl;
	const deHeightTerrain &pHeightTerrain;
	
	deoglRHeightTerrain::Ref pRHeightTerrain;
	
	decObjectList pSectors;
	bool pDirtySectors;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglHeightTerrain(deGraphicOpenGl &ogl, const deHeightTerrain &heightTerrain);
	
	/** Clean up peer. */
	virtual ~deoglHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{return pOgl;}
	
	/** Height terrain. */
	inline const deHeightTerrain &GetHeightTerrain() const{return pHeightTerrain;}
	
	/** Render height terrain. */
	inline const deoglRHeightTerrain::Ref &GetRHeightTerrain() const{return pRHeightTerrain;}
	
	/** Update. */
	void Update(float elapsed);
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Parameter changes. */
	virtual void ParametersChanged();
	
	/** Sector heights changed. */
	virtual void HeightChanged(const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates);
	
	
	
	/** Sector has been added. */
	virtual void SectorAdded(deHeightTerrainSector *sector);
	
	/** Sector has been removed. */
	virtual void SectorRemoved(int index);
	
	/** All sectors have been removed. */
	virtual void AllSectorsRemoved();
	
	/** Sector changed. */
	virtual void SectorChanged(int index);
	
	
	
	/** Decal has been added. */
	virtual void DecalAdded(int sector, deDecal *decal);
	
	/** Decal has been removed. */
	virtual void DecalRemoved(int sector, deDecal *decal);
	
	/** All decals have been removed. */
	virtual void AllDecalsRemoved(int sector);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
