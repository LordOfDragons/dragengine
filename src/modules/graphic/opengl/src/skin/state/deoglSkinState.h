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
 * \brief State of skin for a component or other object using dynamic skins.
 * 
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
	/** \brief Create skin state. */
	deoglSkinState( deoglRenderThread &renderThread );
	deoglSkinState( deoglRenderThread &renderThread, deoglRComponent &component, int texture = -1 );
	deoglSkinState( deoglRenderThread &renderThread, deoglRBillboard &billboard );
	deoglSkinState( deoglRenderThread &renderThread, deoglRDecal &decal );
	deoglSkinState( deoglRenderThread &renderThread, deoglRLight &light );
	
	/** \brief Clean up skin state. */
	~deoglSkinState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner component or NULL. */
	inline deoglRComponent *GetOwnerComponent() const{ return pOwnerComponent; }
	
	/** \brief Owner component texture. */
	inline int GetOwnerComponentTexture() const{ return pOwnerComponentTexture; }
	
	/** \brief Owner billboard or NULL. */
	inline deoglRBillboard *GetOwnerBillboard() const{ return pOwnerBillboard; }
	
	/** \brief Owner decal or NULL. */
	inline deoglRDecal *GetOwnerDecal() const{ return pOwnerDecal; }
	
	/** \brief Owner light or NULL. */
	inline deoglRLight *GetOwnerLight() const{ return pOwnerLight; }
	
	/** \brief Owner skin or NULL. */
	deoglRSkin *GetOwnerSkin() const;
	
	/** \brief Owner dynamic skin or NULL. */
	deoglRDynamicSkin *GetOwnerDynamicSkin() const;
	
	
	
	/** \brief Time value. */
	inline float GetTime() const{ return pTime; }
	
	/** \brief Set time value. */
	void SetTime( float time );
	
	/** \brief Advance time. */
	void AdvanceTime( float timeStep );
	
	
	
	/** \brief Update number. */
	inline int GetUpdateNumber() const{ return pUpdateNumber; }
	
	/** \brief Set update number. */
	void SetUpdateNumber( int updateNumber );
	
	
	
	/** \brief Drop objects containing delayed deletion support. */
	void DropDelayedDeletionObjects();
	
	
	
	/** \brief Number of video players. */
	int GetVideoPlayerCount() const;
	
	/** \brief Set number of video players. */
	void SetVideoPlayerCount( int count );
	
	/** \brief Video player at index which can be \em NULL if not set. */
	deoglRVideoPlayer *GetVideoPlayerAt( int index ) const;
	
	/** \brief Set video player at index which can be \em NULL if not set. */
	void SetVideoPlayerAt( int index, deoglRVideoPlayer *videoPlayer );
	
	
	
	/** \brief Prepare renderables for rendering if required. */
	void PrepareRenderables( deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Add render plans. */
	void AddRenderPlans( deoglRenderPlan &plan );
	
	
	
	/** \brief Number of renderables. */
	inline int GetRenderableCount() const{ return pRenderableCount; }
	
	/** \brief Renderable at the given index. */
	deoglSkinStateRenderable *GetRenderableAt( int index ) const;
	
	/** \brief Add renderable. */
	deoglSkinStateRenderable *AddRenderable();
	
	/** \brief Add renderables. */
	void AddRenderables( deoglRSkin &skin, deoglRDynamicSkin &dynamicSkin );
	
	/** \brief Remove all renderables. */
	void RemoveAllRenderables();
	
	
	
	/** \brief Number of calculated properties. */
	inline int GetCalculatedPropertyCount() const{ return pCalculatedPropertyCount; }
	
	/** \brief Set calculated property count. */
	void SetCalculatedPropertyCount( int count );
	
	/** \brief Calculated property at index. */
	deoglSkinStateCalculated &GetCalculatedPropertyAt( int index ) const;
	
	/** \brief Initialize calculated properties. */
	void InitCalculatedProperties();
	
	/** \brief Map calculated property bones. */
	void CalculatedPropertiesMapBones( const deComponent &component );
	
	/** \brief Update calculated property bones. */
	void UpdateCalculatedPropertiesBones( const deComponent &component );
	
	
	
	/** \brief Variation seed. */
	inline const decPoint &GetVariationSeed() const{ return pVariationSeed; }
	
	/** \brief Set variation seed. */
	void SetVariationSeed( const decPoint &seed );
	/*@}*/
	
	
	
private:
	void pSharedInit();
};

#endif
