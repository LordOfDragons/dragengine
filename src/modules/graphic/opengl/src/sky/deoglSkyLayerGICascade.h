/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLSKYLAYERTGICASCADE_H_
#define _DEOGLSKYLAYERTGICASCADE_H_

#include "deoglSkyLayerTracker.h"
#include "../component/deoglComponentSet.h"

class deoglRSkyInstanceLayer;
class deoglGICascade;
class deoglShadowCaster;
class deoglRComponent;



/**
 * Sky instance layer GI Cascade related data.
 */
class deoglSkyLayerGICascade{
private:
	deoglRSkyInstanceLayer &pLayer;
	const deoglGICascade &pGICascade;
	deoglSkyLayerTracker pTracker;
	decDVector pPosition;
	double pPositionThreshold;
	deoglShadowCaster *pShadowCaster;
	bool pDirtyStaticShadow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky layer GI Cascade. */
	deoglSkyLayerGICascade( deoglRSkyInstanceLayer &layer, const deoglGICascade &cascade );
	
	/** Clean up sky layer GI Cascade. */
	~deoglSkyLayerGICascade();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Sky instance layer. */
	inline deoglRSkyInstanceLayer &GetLayer() const{ return pLayer; }
	
	/** GI Cascade. */
	inline const deoglGICascade &GetGICascade() const{ return pGICascade; }
	
	/** Tracker. */
	inline deoglSkyLayerTracker &GetTracker(){ return pTracker; }
	inline const deoglSkyLayerTracker &GetTracker() const{ return pTracker; }
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Position threshold. */
	inline double GetPositionThreshold() const{ return pPositionThreshold; }
	
	/** Shadow caster. */
	inline deoglShadowCaster &GetShadowCaster() const{ return *pShadowCaster; }
	
	/** Update. */
	void Update();
	
	/** Static shadow map is dirty. */
	inline bool GetDirtyStaticShadow() const{ return pDirtyStaticShadow; }
	
	/** Clear dirty static shadow map flag. */
	void ClearDirtyStaticShadow();
	
	/** Notify render static component changed requiring updates. */
	void NotifyUpdateStaticComponent( deoglRComponent *component );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
