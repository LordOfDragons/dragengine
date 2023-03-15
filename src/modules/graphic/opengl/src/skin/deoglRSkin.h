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

#ifndef _DEOGLRSKIN_H_
#define _DEOGLRSKIN_H_
#include "../memory/consumption/deoglMemoryConsumptionSkinUse.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglSkinTexture;
class deoglSkinRenderable;
class deoglVSRetainImageData;
class deoglSkinCalculatedProperty;

class deSkin;



/**
 * Render skin.
 */
class deoglRSkin : public deObject{
public:
	enum ePropertyStates{
		epsAll,
		epsSome,
		epsNobody
	};
	
private:
	deoglRenderThread &pRenderThread;
	decString pFilename;
	
	deoglSkinTexture **pTextures;
	int pTextureCount;
	
	bool pIsSolid;
	bool pHasHoles;
	bool pHasXRay;
	bool pShadeless;
	bool pHasMirrors;
	bool pHasDynamicChannels;
	bool pHasRenderables;
	ePropertyStates pReflected;
	ePropertyStates pShadowNone;
	int pShadowImportance;
	
	bool pCastSolidShadow;
	bool pCastTranspShadow;
	
	decObjectList pRenderables;
	int pVideoPlayerCount;
	decObjectList pCalculatedProperties;
	
	deoglVSRetainImageData *pVSRetainImageData;
	bool pTexturePipelinesReady;
	
	deoglMemoryConsumptionSkinUse pMemUse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render skin. */
	deoglRSkin( deoglRenderThread &renderThread, const deSkin &Skin );
	
	/** Clean up render skin. */
	virtual ~deoglRSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Filename of skin file. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	
	
	/**
	 * Finalize after asynchronous resource loading.
	 * \details Also released retained image data.
	 */
	void FinalizeAsyncResLoading();
	
	/** Prepare texture pipelines if not prepared yet. */
	void PrepareTexturePipelines();
	
	
	
	/** Skin is solid. */
	inline bool GetIsSolid() const{ return pIsSolid; }
	
	/** Skin has holes. */
	inline bool GetHasHoles() const{ return pHasHoles; }
	
	/** Skin has XRay. */
	inline bool GetHasXRay() const{ return pHasXRay; }
	
	/** Skin has mirrors. */
	inline bool GetHasMirrors() const{ return pHasMirrors; }
	
	/** Skin has textures with dynamic channels. */
	inline bool GetHasDynamicChannels() const{ return pHasDynamicChannels; }
	
	/** Skin has renderables. */
	inline bool GetHasRenderables() const{ return pHasRenderables; }
	
	/** Skin is shadeless. */
	inline bool GetShadeless() const{ return pShadeless; }
	
	/** Reflected type. */
	inline ePropertyStates GetReflected() const{ return pReflected; }
	
	/** Shadow casting type. */
	inline ePropertyStates GetShadowNone() const{ return pShadowNone; }
	
	/** Shadow importance level. */
	inline int GetShadowImportance() const{ return pShadowImportance; }
	
	/** Skin casts solid shadows. */
	inline bool GetCastSolidShadow() const{ return pCastSolidShadow; }
	
	/** Skin casts transparent shadows. */
	inline bool GetCastTransparentShadow() const{ return pCastTranspShadow; }
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Texture at index. */
	deoglSkinTexture &GetTextureAt( int index ) const;
	
	
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglSkinRenderable &GetRenderableAt( int index );
	
	/** Add renderable if absent. */
	int AddRenderable( const char *name );
	
	/** Index of named renderable or -1 if not found. */
	int IndexOfRenderableNamed( const char *name ) const;
	
	
	
	/** Number of video players. */
	inline int GetVideoPlayerCount() const{ return pVideoPlayerCount; }
	
	/**
	 * Add video player.
	 * \returns Index of video player.
	 */
	int AddVideoPlayer();
	
	
	
	/** Number of calculated properties. */
	int GetCalculatedPropertyCount() const;
	
	/** Calculated property. */
	deoglSkinCalculatedProperty *GetCalculatedPropertyAt( int index ) const;
	
	/**
	 * Add calculated property.
	 * \returns Index of calculated property.
	 */
	int AddCalculatedProperty( deoglSkinCalculatedProperty *calculated );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pRetainImageData( const deSkin &skin );
};

#endif
