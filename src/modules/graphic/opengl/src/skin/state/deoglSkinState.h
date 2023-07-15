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
class deoglSkinStateMapped;
class deoglSkinStateCalculated;
class deoglSkinStateConstructed;
class deoglRDynamicSkin;
class deoglRComponent;
class deoglRBillboard;
class deoglRDecal;
class deoglRLight;
class deoglRenderPlanMasked;

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
	
	deoglSkinStateMapped *pMapped;
	int pMappedCount;
	
	deoglSkinStateCalculated *pCalculatedProperties;
	int pCalculatedPropertyCount;
	
	deoglSkinStateConstructed *pConstructedProperties;
	int pConstructedPropertyCount;
	
	decPoint pVariationSeed;
	
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
	
	
	
	/** Number of video players. */
	int GetVideoPlayerCount() const;
	
	/** Set number of video players. */
	void SetVideoPlayerCount( int count );
	
	/** Video player at index which can be \em NULL if not set. */
	deoglRVideoPlayer *GetVideoPlayerAt( int index ) const;
	
	/** Set video player at index which can be \em NULL if not set. */
	void SetVideoPlayerAt( int index, deoglRVideoPlayer *videoPlayer );
	
	
	
	/** Prepare renderables for rendering if required. */
	void PrepareRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
		const deoglRenderPlanMasked *renderPlanMask );
	
	/** Render renderables if required. */
	void RenderRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
		const deoglRenderPlanMasked *renderPlanMask );
	
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
	
	
	
	/** Count of mapped. */
	inline int GetMappedCount() const{ return pMappedCount; }
	
	/** Set mapped count. */
	void SetMappedCount( int count );
	
	/** Mapped at index. */
	deoglSkinStateMapped &GetMappedAt( int index ) const;
	
	/** Initialize mapped. */
	void InitMapped();
	
	/** Map mapped bones. */
	void MappedMapBones( const deComponent &component );
	
	/** Update mapped bones. */
	void UpdateMappedBones( const deComponent &component );
	
	/** Update mapped. */
	void UpdateMapped();
	
	
	
	/** Number of calculated properties. */
	inline int GetCalculatedPropertyCount() const{ return pCalculatedPropertyCount; }
	
	/** Set calculated property count. */
	void SetCalculatedPropertyCount( int count );
	
	/** Calculated property at index. */
	deoglSkinStateCalculated &GetCalculatedPropertyAt( int index ) const;
	
	/** Initialize calculated properties. */
	void InitCalculatedProperties();
	
	/** Update calculated properties. */
	void UpdateCalculatedProperties();
	
	
	
	/** Number of constructed properties. */
	inline int GetConstructedPropertyCount() const{ return pConstructedPropertyCount; }
	
	/** Set constructed property count. */
	void SetConstructedPropertyCount( int count );
	
	/** Constructed property at index. */
	deoglSkinStateConstructed &GetConstructedPropertyAt( int index ) const;
	
	/** Initialize constructed properties. */
	void InitConstructedProperties();
	
	/** Update constructed properties. */
	void UpdateConstructedProperties();
	
	
	
	/** Variation seed. */
	inline const decPoint &GetVariationSeed() const{ return pVariationSeed; }
	
	/** Set variation seed. */
	void SetVariationSeed( const decPoint &seed );
	/*@}*/
	
	
	
private:
	void pSharedInit();
};

#endif
