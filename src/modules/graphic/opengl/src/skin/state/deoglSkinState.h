/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINSTATE_H_
#define _DEOGLSKINSTATE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglRenderPlan;
class deoglRenderThread;
class deoglRSkin;
class deoglRVideoPlayer;
class deoglSkinStateRenderable;
class deoglSkinStateCalculated;
class deoglRDynamicSkin;
class deoglRComponent;
class deoglRBillboard;
class deoglRDecal;
class deoglRLight;

class deComponent;


/**
 * State of skin for a component or other object using dynamic skins.
 * Stores also renderable link indices to alter skin states dynamically. The
 * number of renderable links matches the number of renderables in the skin
 * object. The links point to the host object renderables.
 */
class deoglSkinState{
private:
	deoglRenderThread &pRenderThread;
	float pTime;
	
	deoglRComponent *pOwnerComponent;
	int pOwnerComponentTexture;
	deoglRBillboard *pOwnerBillboard;
	deoglRDecal *pOwnerDecal;
	deoglRLight *pOwnerLight;
	
	deoglSkinStateRenderable **pRenderables;
	int pRenderableCount;
	int pRenderableSize;
	
	decObjectList pVideoPlayers;
	
	deoglSkinStateCalculated *pCalculatedProperties;
	int pCalculatedPropertyCount;
	
	decPoint pVariationSeed;
	
	int pUpdateNumber;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state. */
	deoglSkinState( deoglRenderThread &renderThread );
	deoglSkinState( deoglRenderThread &renderThread, deoglRComponent &component, int texture = -1 );
	deoglSkinState( deoglRenderThread &renderThread, deoglRBillboard &billboard );
	deoglSkinState( deoglRenderThread &renderThread, deoglRDecal &decal );
	deoglSkinState( deoglRenderThread &renderThread, deoglRLight &light );
	
	/** Clean up skin state. */
	~deoglSkinState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Owner component or NULL. */
	inline deoglRComponent *GetOwnerComponent() const{ return pOwnerComponent; }
	
	/** Owner component texture. */
	inline int GetOwnerComponentTexture() const{ return pOwnerComponentTexture; }
	
	/** Owner billboard or NULL. */
	inline deoglRBillboard *GetOwnerBillboard() const{ return pOwnerBillboard; }
	
	/** Owner decal or NULL. */
	inline deoglRDecal *GetOwnerDecal() const{ return pOwnerDecal; }
	
	/** Owner light or NULL. */
	inline deoglRLight *GetOwnerLight() const{ return pOwnerLight; }
	
	/** Owner skin or NULL. */
	deoglRSkin *GetOwnerSkin() const;
	
	/** Owner dynamic skin or NULL. */
	deoglRDynamicSkin *GetOwnerDynamicSkin() const;
	
	
	
	/** Time value. */
	inline float GetTime() const{ return pTime; }
	
	/** Set time value. */
	void SetTime( float time );
	
	/** Advance time. */
	void AdvanceTime( float timeStep );
	
	
	
	/** Update number. */
	inline int GetUpdateNumber() const{ return pUpdateNumber; }
	
	/** Set update number. */
	void SetUpdateNumber( int updateNumber );
	
	
	
	/** Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	
	
	
	/** Number of video players. */
	int GetVideoPlayerCount() const;
	
	/** Set number of video players. */
	void SetVideoPlayerCount( int count );
	
	/** Video player at index which can be \em NULL if not set. */
	deoglRVideoPlayer *GetVideoPlayerAt( int index ) const;
	
	/** Set video player at index which can be \em NULL if not set. */
	void SetVideoPlayerAt( int index, deoglRVideoPlayer *videoPlayer );
	
	
	
	/** Prepare renderables for rendering if required. */
	void PrepareRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin );
	
	/** Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** Number of renderables. */
	inline int GetRenderableCount() const{ return pRenderableCount; }
	
	/** Renderable at the given index. */
	deoglSkinStateRenderable *GetRenderableAt( int index ) const;
	
	/** Add renderable. */
	deoglSkinStateRenderable *AddRenderable();
	
	/** Add renderables. */
	void AddRenderables( deoglRSkin &skin, deoglRDynamicSkin &dynamicSkin );
	
	/** Remove all renderables. */
	void RemoveAllRenderables();
	
	
	
	/** Number of calculated properties. */
	inline int GetCalculatedPropertyCount() const{ return pCalculatedPropertyCount; }
	
	/** Set calculated property count. */
	void SetCalculatedPropertyCount( int count );
	
	/** Calculated property at index. */
	deoglSkinStateCalculated &GetCalculatedPropertyAt( int index ) const;
	
	/** Initialize calculated properties. */
	void InitCalculatedProperties();
	
	/** Map calculated property bones. */
	void CalculatedPropertiesMapBones( const deComponent &component );
	
	/** Update calculated property bones. */
	void UpdateCalculatedPropertiesBones( const deComponent &component );
	
	/** Update calculated properties. */
	void UpdateCalculatedProperties();
	
	
	
	/** Variation seed. */
	inline const decPoint &GetVariationSeed() const{ return pVariationSeed; }
	
	/** Set variation seed. */
	void SetVariationSeed( const decPoint &seed );
	/*@}*/
	
	
	
private:
	void pSharedInit();
};

#endif
