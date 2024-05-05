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
