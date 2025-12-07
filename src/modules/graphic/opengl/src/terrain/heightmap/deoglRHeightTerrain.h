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

#ifndef _DEOGLRHEIGHTTERRAIN_H_
#define _DEOGLRHEIGHTTERRAIN_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/deObject.h>

class deoglRWorld;
class deoglRHTSector;
class deoglRenderThread;
class deoglHeightTerrainListener;

class deHeightTerrain;


/**
 * Render height terrain.
 */
class deoglRHeightTerrain : public deObject{
public:
	typedef deTObjectReference<deoglRHeightTerrain> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	
	int pSectorResolution;
	float pSectorSize;
	
	decObjectList pSectors;
	bool pSectorsRequirePrepareForRender;
	
	decObjectOrderedSet pListeners;
	int pListenerIndex;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render height terrain. */
	deoglRHeightTerrain(deoglRenderThread &renderThread, const deHeightTerrain &heightTerrain);
	
protected:
	/** Clean up render height terrain. */
	virtual ~deoglRHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{return pRenderThread;}
	
	/** Parent world. */
	inline deoglRWorld *GetParentWorld() const{return pParentWorld;}
	
	/** Set parent world. */
	void SetParentWorld(deoglRWorld *parentWorld);
	
	/** Height image size. */
	inline int GetSectorResolution() const{return pSectorResolution;}
	
	/** Sector dimension. */
	inline float GetSectorSize() const{return pSectorSize;}
	
	/** Prepare for render. */
	void PrepareForRender();
	
	
	
	/** Number of sectors. */
	int GetSectorCount() const;
	
	/** Sector at index. */
	deoglRHTSector &GetSectorAt(int index) const;
	
	/** Add sector. */
	void AddSector(deoglRHTSector *htsector);
	
	/** Remove all sectors. */
	void RemoveAllSectors();
	
	/** Sector requires prepare for render. */
	void SectorRequirePrepareForRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener(deoglHeightTerrainListener *listener);
	
	/** Remove listener if existing. */
	void RemoveListener(deoglHeightTerrainListener *listener);
	
	/** Notify all that this height terrain has been destroyed. */
	void NotifyHeightTerrainDestroyed();
	
	/** Notify all sectors changed. */
	void NotifySectorsChanged();
	/*@}*/
};

#endif
