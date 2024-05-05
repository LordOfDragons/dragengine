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
class deoglSkinStateBone;
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
	
	deoglSkinStateBone *pBones;
	int pBoneCount;
	
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
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
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
	
	
	
	/** Number of calculated properties. */
	inline int GetCalculatedPropertyCount() const{ return pCalculatedPropertyCount; }
	
	/** Set calculated property count. */
	void SetCalculatedPropertyCount( int count );
	
	/** Calculated property at index. */
	deoglSkinStateCalculated &GetCalculatedPropertyAt( int index ) const;
	
	
	
	/** Number of constructed properties. */
	inline int GetConstructedPropertyCount() const{ return pConstructedPropertyCount; }
	
	/** Set constructed property count. */
	void SetConstructedPropertyCount( int count );
	
	/** Constructed property at index. */
	deoglSkinStateConstructed &GetConstructedPropertyAt( int index ) const;
	
	/** Prepare constructed properties for rendering. */
	void PrepareConstructedProperties();
	
	
	
	/** Count of bones. */
	inline int GetBoneCount() const{ return pBoneCount; }
	
	/** Set bones count. */
	void SetBoneCount( int count );
	
	/** Bones at index. */
	deoglSkinStateBone &GetBoneAt( int index ) const;
	
	
	
	/** Init all states. */
	void InitAll();
	
	/** Update all states. */
	void UpdateAll();
	
	/** Map bones. */
	void MapBonesAll( const deComponent &component );
	
	/** Update bones. */
	void UpdateBonesAll( const deComponent &component );
	
	
	
	/** Variation seed. */
	inline const decPoint &GetVariationSeed() const{ return pVariationSeed; }
	
	/** Set variation seed. */
	void SetVariationSeed( const decPoint &seed );
	/*@}*/
	
	
	
private:
	void pSharedInit();
};

#endif
