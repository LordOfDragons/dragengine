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

#ifndef _DEOGLRSKIN_H_
#define _DEOGLRSKIN_H_
#include "../memory/consumption/deoglMemoryConsumptionSkinUse.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglSkin;
class deoglSkinTexture;
class deoglSkinRenderable;
class deoglSkinBone;
class deoglSkinMapped;
class deoglSkinCalculatedProperty;
class deoglSkinConstructedProperty;
class deoglVSRetainImageData;

class deSkin;



/**
 * Render skin.
 */
class deoglRSkin : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRSkin> Ref;
	
	
	enum ePropertyStates{
		epsAll,
		epsSome,
		epsNobody
	};
	
private:
	deoglRenderThread &pRenderThread;
	deoglSkin *pOwnerSkin;
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
	decObjectList pMapped;
	decObjectList pCalculatedProperties;
	decObjectList pConstructedProperties;
	decObjectList pBones;
	
	deoglVSRetainImageData *pVSRetainImageData;
	bool pTexturePipelinesReady;
	
	deoglMemoryConsumptionSkinUse pMemUse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render skin. */
	deoglRSkin(deoglRenderThread &renderThread, deoglSkin &owner, const deSkin &Skin);
	
	/** Clean up render skin. */
	virtual ~deoglRSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Filename of skin file. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Owner skin. For special use only. */
	inline deoglSkin *GetOwnerSkin() const{ return pOwnerSkin; }
	void DropOwnerSkin();

	
	
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
	deoglSkinTexture &GetTextureAt(int index) const;
	
	
	
	/** Number of renderables. */
	int GetRenderableCount() const;
	
	/** Renderable at index. */
	deoglSkinRenderable &GetRenderableAt(int index);
	
	/** Add renderable if absent. */
	int AddRenderable(const char *name);
	
	/** Index of named renderable or -1 if not found. */
	int IndexOfRenderableNamed(const char *name) const;
	
	
	
	/** Number of video players. */
	inline int GetVideoPlayerCount() const{ return pVideoPlayerCount; }
	
	/**
	 * Add video player.
	 * \returns Index of video player.
	 */
	int AddVideoPlayer();
	
	
	
	/** Count of mapped. */
	int GetMappedCount() const;
	
	/** Mapped. */
	deoglSkinMapped *GetMappedAt(int index) const;
	
	
	
	/** Number of calculated properties. */
	int GetCalculatedPropertyCount() const;
	
	/** Calculated property. */
	deoglSkinCalculatedProperty *GetCalculatedPropertyAt(int index) const;
	
	/**
	 * Add calculated property.
	 * \returns Index of calculated property.
	 */
	int AddCalculatedProperty(deoglSkinCalculatedProperty *calculated);
	
	
	
	/** Number of constructed properties. */
	int GetConstructedPropertyCount() const;
	
	/** Constructed property. */
	deoglSkinConstructedProperty *GetConstructedPropertyAt(int index) const;
	
	/** Add constructed property and returns index of property. */
	int AddConstructedProperty(deoglSkinConstructedProperty *constructed);
	
	
	
	/** Count of bones. */
	int GetBoneCount() const;
	
	/** Bones. */
	deoglSkinBone *GetBoneAt(int index) const;
	
	/** Add bone and returns index. */
	int AddBone(const char *name);



	/** Drop all caches. */
	void DropAllCaches();
 	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pRetainImageData(const deSkin &skin);
};

#endif
