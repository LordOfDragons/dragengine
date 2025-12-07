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

#ifndef _DEOGLADDTOPERSISTENTRENDERTASK_H_
#define _DEOGLADDTOPERSISTENTRENDERTASK_H_

#include "../../../deoglBasics.h"
#include "../../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>


class deoglCollideList;
class deoglCollideListComponent;
class deoglRComponent;
class deoglPersistentRenderTask;
class deoglPersistentRenderTaskStep;
class deoglPersistentRenderTaskTexture;
class deoglPersistentRenderTaskVAO;
class deoglPersistentRenderTaskOwner;
class deoglRenderThread;
class deoglSPBlockUBO;
class deoglTexUnitsConfig;
class deoglVAO;


/**
 * Add elements to persistent render task.
 */
class deoglAddToPersistentRenderTask{
private:
	deoglRenderThread &pRenderThread;
	
	deoglPersistentRenderTask &pRenderTask;
	deoglSkinTexturePipelines::eTypes pSkinPipelineType;
	int pSkinPipelineModifier;
	
	bool pSolid;
	bool pNoShadowNone;
	bool pNoNotReflected;
	bool pNoRendered;
	bool pOutline;
	
	bool pFilterXRay;
	bool pXRay;
	
	bool pFilterHoles;
	bool pWithHoles;
	
	bool pFilterDoubleSided;
	bool pDoubleSided;
	bool pForceDoubleSided;
	
	bool pFilterDecal;
	bool pDecal;
	
	int pFilterCubeFace;
	
	bool pUseSpecialParamBlock;
	
	const deoglPipeline *pEnforcePipeline;
	const deoglSPBlockUBO *pEnforceParamBlock;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new add to render task. */
	deoglAddToPersistentRenderTask(deoglRenderThread &renderThread, deoglPersistentRenderTask &renderTask);
	
	/** Clean up add to render task . */
	~deoglAddToPersistentRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Pipeline type. */
	inline deoglSkinTexturePipelines::eTypes GetSkinPipelineType() const{return pSkinPipelineType;}
	
	/** Set pipeline type. */
	void SetSkinPipelineType(deoglSkinTexturePipelines::eTypes type);
	
	/** Pipeline modifier. */
	inline int GetSkinPipelineModifier() const{return pSkinPipelineModifier;}
	
	/** Set pipeline modifier. */
	void SetSkinPipelineModifier(int modifier);
	
	
	
	/** Solid or transparent textures are added. */
	inline bool GetSolid() const{return pSolid;}
	
	/** Set if solid or transparent texture are added. */
	void SetSolid(bool solid);
	
	/** Textures with the shadow none property are not added. */
	inline bool GetNoShadowNone() const{return pNoShadowNone;}
	
	/** Set if textures with the shadow none property are not added. */
	void SetNoShadowNone(bool noShadowNone);
	
	/** Textures without the reflected property are not added. */
	inline bool GetNoNotReflected() const{return pNoNotReflected;}
	
	/** Set if textures without the reflected property are not added. */
	void SetNoNotReflected(bool noNotReflected);
	
	/** Rendered textures are not added. */
	inline bool GetNoRendered() const{return pNoRendered;}
	
	/** Set if rendered textures are not added. */
	void SetNoRendered(bool noRendered);
	
	/** Outline textures are added. */
	inline bool GetOutline() const{return pOutline;}
	
	/** Set if outline transparent texture are added. */
	void SetOutline(bool outline);
	
	
	
	/** Filtering for XRay is enabled. */
	inline bool GetFilterXRay() const{return pFilterXRay;}
	
	/** Set if filtering for XRay is enabled. */
	void SetFilterXRay(bool filterXRay);
	
	/** XRay textures are added. */
	inline bool GetXRay() const{return pXRay;}
	
	/** Set if xray texture are added. */
	void SetXRay(bool xray);
	
	
	
	/** Filtering for holes is enabled. */
	inline bool GetFilterHoles() const{return pFilterHoles;}
	
	/** Set if filtering for holes is enabled. */
	void SetFilterHoles(bool filterHoles);
	
	/** Textures with or without holes are added. */
	inline bool GetWithHoles() const{return pWithHoles;}
	
	/** Set if textures with or without holes are added. */
	void SetWithHoles(bool withHoles);
	
	
	
	/** Filtering for double sided is enabled. */
	inline bool GetFilterDoubleSided() const{return pFilterDoubleSided;}
	
	/** Set if filtering for double sided is enabled. */
	void SetFilterDoubleSided(bool filterDoubleSided);
	
	/** Doubled sided textures are selected if double sided filtering is enabled. */
	inline bool GetDoubleSided() const{return pDoubleSided;}
	
	/** Set if doubled sided textures are selected if double sided filtering is enabled. */
	void SetDoubleSided(bool doubleSided);
	
	/** All geometry is forced to be double sided. */
	inline bool GetForceDoubleSided() const{return pForceDoubleSided;}
	
	/** Set all geometry is forced to be double sided. */
	void SetForceDoubleSided(bool doubleSided);
	
	
	
	/** Filtering for decal is enabled. */
	inline bool GetFilterDecal() const{return pFilterDecal;}
	
	/** Set if filtering for decal is enabled. */
	void SetFilterDecal(bool filterDecal);
	
	/** Decal textures are selected if decal filtering is enabled. */
	inline bool GetDecal() const{return pDecal;}
	
	/** Set if decal textures are selected if decal filtering is enabled. */
	void SetDecal(bool decal);
	
	
	
	/** Filter by cube face test result or -1 if disabled. */
	inline int GetFilterCubeFace() const{return pFilterCubeFace;}
	
	/** Set filter by cube face test result or -1 if disabled. */
	void SetFilterCubeFace(int cubeFace);
	
	
	
	/** Use special shader parameter blocks. */
	inline bool GetUseSpecialParamBlock() const{return pUseSpecialParamBlock;}
	
	/** Set if special shader parameter blocks are used. */
	void SetUseSpecialParamBlock(bool use);
	
	
	
	/** Pipeline to enforce or \em NULL if free. */
	inline const deoglPipeline *GetEnforcedPipeline() const{return pEnforcePipeline;}
	
	/** Set pipeline to enforce or \em NULL if free. */
	void SetEnforcePipeline(const deoglPipeline *pipeline);
	
	/** Shader parameter block to enforce or \em NULL if free. */
	inline const deoglSPBlockUBO *GetEnforcedParamBlock() const{return pEnforceParamBlock;}
	
	/** Set shader parameter block to enforce or \em NULL if free. */
	void SetEnforceParamBlock(const deoglSPBlockUBO *block);
	
	
	
	/** Add component. */
	void AddComponent(deoglPersistentRenderTaskOwner &owner,
		const deoglCollideListComponent &clcomponent);
	
	/**
	 * Add continuous run of all faces of texture of component. Component is supposed to be
	 * updated already and model and skin exist as well as parent world exists.
	 */
	void AddComponentFaces(deoglPersistentRenderTaskOwner &owner,
		const deoglRComponent &component, int texture, int lodLevel, int specialFlags);
	
	/**
	 * Add continuous run of faces of component. Component is supposed to be updated already
	 * and that model and skin exist as well as parent world exist.
	 */
	void AddComponentFaces(deoglPersistentRenderTaskOwner &owner, const deoglRComponent &component,
		int texture, int firstFace, int faceCount, int lodLevel, int specialFlags);
	/*@}*/
	
	
	
private:
	bool pFilterReject(const deoglSkinTexture *skinTexture) const;
	bool pFilterRejectNoSolid(const deoglSkinTexture *skinTexture) const;
	
	deoglPersistentRenderTaskVAO *pGetTaskVAO(
		deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType,
		deoglSkinTexturePipelines::eTypes pipelineType, int pipelineModifier,
		const deoglSkinTexture &skinTexture, const deoglTexUnitsConfig *tuc,
		const deoglVAO *vao) const;
};

#endif
