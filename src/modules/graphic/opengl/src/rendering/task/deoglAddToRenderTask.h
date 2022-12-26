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

#ifndef _DEOGLADDTORENDERTASK_H_
#define _DEOGLADDTORENDERTASK_H_

#include "../../deoglBasics.h"
#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglCollideListComponent;
class deoglCollideListHTSector;
class deoglCollideListPropField;
class deoglCollideListPropFieldType;
class deoglComponentList;
class deoglCubeMap;
class deoglHTViewSector;
class deoglParticleEmitterInstanceList;
class deoglModelLOD;
class deoglPipeline;
class deoglRBillboard;
class deoglRComponent;
class deoglRComponentLOD;
class deoglRDecal;
class deoglRParticleEmitterInstance;
class deoglRParticleEmitterInstanceType;
class deoglRPropFieldType;
class deoglRenderTask;
class deoglRenderTaskConfig;
class deoglRenderTaskStep;
class deoglRenderTaskTexture;
class deoglRenderTaskVAO;
class deoglRenderTaskSharedShader;
class deoglRenderTaskSharedVAO;
class deoglRenderThread;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglSkinShader;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexUnitsConfig;
class deoglTexture;
class deoglVAO;


/**
 * Add elements to render task.
 * 
 * Provides support for filtering the elements using texture properties.
 */
class deoglAddToRenderTask{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderTask &pRenderTask;
	deoglSkinTexture::eShaderTypes pSkinShaderType;
	deoglSkinTexture::eShaderTypes pSkinShaderTypeRibbon;
	deoglSkinTexture::eShaderTypes pSkinShaderTypeBeam;
	
	bool pSolid;
	bool pNoShadowNone;
	bool pNoNotReflected;
	bool pNoRendered;
	bool pOutline;
	bool pForceDoubleSided;
	
	bool pFilterXRay;
	bool pXRay;
	
	bool pFilterHoles;
	bool pWithHoles;
	
	bool pFilterDecal;
	bool pDecal;
	
	int pFilterCubeFace;
	
	int pFilters;
	int pFilterMask;
	int pFiltersMasked;
	
	bool pUseSpecialParamBlock;
	
	const deoglPipeline *pEnforcePipeline;
	deoglRenderTaskSharedShader *pEnforceShader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new add to render task. */
	deoglAddToRenderTask( deoglRenderThread &renderThread, deoglRenderTask &renderTask );
	
	/** Clean up add to render task . */
	~deoglAddToRenderTask();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render task. */
	inline deoglRenderTask &GetRenderTask() const{ return pRenderTask; }
	
	/** Shader type to be used for skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderType() const{ return pSkinShaderType; }
	
	/** Set shader type to be used for skin shaders. */
	void SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	/** Shader type to be used for ribbon skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderTypeRibbon() const{ return pSkinShaderTypeRibbon; }
	
	/** Set shader type to be used for ribbon skin shaders. */
	void SetSkinShaderTypeRibbon( deoglSkinTexture::eShaderTypes shaderType );
	
	/** Shader type to be used for beam skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderTypeBeam() const{ return pSkinShaderTypeBeam; }
	
	/** Set shader type to be used for beam skin shaders. */
	void SetSkinShaderTypeBeam( deoglSkinTexture::eShaderTypes shaderType );
	
	
	
	/** Solid or transparent textures are added. */
	inline bool GetSolid() const{ return pSolid; }
	
	/** Set if solid or transparent texture are added. */
	void SetSolid( bool solid );
	
	/** Textures with the shadow none property are not added. */
	inline bool GetNoShadowNone() const{ return pNoShadowNone; }
	
	/** Set if textures with the shadow none property are not added. */
	void SetNoShadowNone( bool noShadowNone );
	
	/** Textures without the reflected property are not added. */
	inline bool GetNoNotReflected() const{ return pNoNotReflected; }
	
	/** Set if textures without the reflected property are not added. */
	void SetNoNotReflected( bool noNotReflected );
	
	/** Rendered textures are not added. */
	inline bool GetNoRendered() const{ return pNoRendered; }
	
	/** Set if rendered textures are not added. */
	void SetNoRendered( bool noRendered );
	
	/** Outline textures are added. */
	inline bool GetOutline() const{ return pOutline; }
	
	/** Set if outline transparent texture are added. */
	void SetOutline( bool outline );
	
	/** Force double sided. */
	inline bool GetForceDoubleSided() const{ return pForceDoubleSided; }
	
	/** Set to force double sided. */
	void SetForceDoubleSided( bool forceDoubleSided );
	
	
	
	/** Filtering for XRay is enabled. */
	inline bool GetFilterXRay() const{ return pFilterXRay; }
	
	/** Set if filtering for XRay is enabled. */
	void SetFilterXRay( bool filterXRay );
	
	/** XRay textures are added. */
	inline bool GetXRay() const{ return pXRay; }
	
	/** Set if XRay texture are added. */
	void SetXRay( bool xray );
	
	
	
	/** Filtering for holes is enabled. */
	inline bool GetFilterHoles() const{ return pFilterHoles; }
	
	/** Set if filtering for holes is enabled. */
	void SetFilterHoles( bool filterHoles );
	
	/** Textures with or without holes are added. */
	inline bool GetWithHoles() const{ return pWithHoles; }
	
	/** Set if textures with or without holes are added. */
	void SetWithHoles( bool withHoles );
	
	
	
	/** Filtering for decal is enabled. */
	inline bool GetFilterDecal() const{ return pFilterDecal; }
	
	/** Set if filtering for decal is enabled. */
	void SetFilterDecal( bool filterDecal );
	
	/** Decal textures are selected if decal filtering is enabled. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Set if decal textures are selected if decal filtering is enabled. */
	void SetDecal( bool decal );
	
	
	
	/** Filter by cube face test result or -1 if disabled. */
	inline int GetFilterCubeFace() const{ return pFilterCubeFace; }
	
	/** Set filter by cube face test result or -1 if disabled. */
	void SetFilterCubeFace( int cubeFace );
	
	
	
	/** Use special shader parameter blocks. */
	inline bool GetUseSpecialParamBlock() const{ return pUseSpecialParamBlock; }
	
	/** Set if special shader parameter blocks are used. */
	void SetUseSpecialParamBlock( bool use );
	
	
	
	/** Pipeline to enforce or nullptr if free. */
	inline const deoglPipeline *GetEnforcedPipeline() const{ return pEnforcePipeline; }
	
	/** Set pipeline to enforce or nullptr if free. */
	void SetEnforcePipeline( const deoglPipeline *pipeline );
	
	/** Shader to enforce or NULL if free. */
	inline deoglRenderTaskSharedShader *GetEnforcedShader() const{ return pEnforceShader; }
	
	/** Set shader to enforce or NULL if free. */
	void SetEnforceShader( deoglRenderTaskSharedShader *shader );
	
	
	
	/** Reset render task parameters. */
	void Reset();
	
	
	
	/** Add component. */
	void AddComponent( const deoglRComponentLOD &lod, int specialFlags );
	
	/** Add component. */
	void AddComponent( const deoglCollideListComponent &clcomponent );
	
	/** Add components from the given collide list. */
	void AddComponents( const deoglCollideList &clist );
	
	/** Add continuous run of all faces of a texture of a component. */
	void AddComponentFaces( const deoglRComponentLOD &lod, int texture, int specialFlags );
	
	void AddComponentFaces( const deoglRComponentLOD &lod, const deoglModelLOD &modelLod,
		int texture, const deoglRenderTaskSharedVAO *rtvao, int specialFlags );
	
	
	
	/** Add billboards from collide list. */
	void AddBillboards( const deoglCollideList &clist );
	
	/** Add billboard. */
	void AddBillboard( const deoglRBillboard &billboard );
	
	
	
	/** Add decal. */
	void AddDecal( const deoglRDecal &decal, int lodLevel );
	
	/** Add all decals of a component. */
	void AddDecals( const deoglRComponent &component, int lodLevel );
	
	/** Add all decals of all components from the given collide list. */
	void AddDecals( const deoglCollideList &clist );
	
	
	
	/** Add all clusters of a collide list prop field type. */
	void AddPropFieldType( const deoglCollideListPropFieldType &clPropFieldType,
		const deoglRPropFieldType &propFieldType, bool imposters );
	
	/** Add all types of a collide list prop field. */
	void AddPropField( const deoglCollideListPropField &clPropField, bool imposters );
	
	/** Add all prop fields of a collide list. */
	void AddPropFields( const deoglCollideList &clist, bool imposters );
	
	
	
	/** Add all clusters of height terrain sector texture. */
	void AddHeightTerrainSectorClusters( const deoglCollideListHTSector &clhtsector, int texture );
	
	/** Add a height terrain sector. */
	void AddHeightTerrainSector( const deoglCollideListHTSector &clhtsector, bool firstMask );
	
	/** Add all height terrains of a collide list. */
	void AddHeightTerrains( const deoglCollideList &clist, bool firstMask );
	
	
	
	/** Add an occlusion mesh from a component. */
	void AddOcclusionMesh( const deoglCollideListComponent &clcomponent,
		deoglRenderTaskTexture *taskTexture, bool withSingleSided );
	
	void AddOcclusionMesh( deoglRComponent &component,
		deoglRenderTaskTexture *taskTexture, bool withSingleSided );
	
	/** Add occlusion meshes for all components in a collide list. */
	void AddOcclusionMeshes( const deoglCollideList &clist, bool withSingleSided );
	
	/** Add occlusion meshes for all components in a collide list. */
	void AddOcclusionMeshes( const deoglCollideList &clist,
		deoglRenderTaskTexture *taskTexture, bool withSingleSided );
	
	/** Add occlusion meshes for all components from list. */
	void AddOcclusionMeshes( const deoglComponentList &list, bool withSingleSided );
	
	/** Add occlusion meshes for all components from list. */
	void AddOcclusionMeshes( const deoglComponentList &list,
		deoglRenderTaskTexture *taskTexture, bool withSingleSided );
	
	/** Add a continuous run of faces of an occlusion mesh. */
	void AddOcclusionMeshFaces( const deoglRComponent &component, bool doubleSided,
		deoglRenderTaskTexture *taskTexture, int specialFlags );
	
	
	
	/** Add particles from collide list. */
	void AddParticles( const deoglCollideList &list );
	
	/** Add particles from all emitters in list. */
	void AddParticles( const deoglParticleEmitterInstanceList &list );
	
	/** Add particles from all emitters in list. */
	void AddParticles( deoglRParticleEmitterInstance &emitter );
	
	/** Add particles from emitter. */
	void AddParticles( deoglRParticleEmitterInstance &emitter,
		deoglRParticleEmitterInstanceType &type );
	
	
	
	/** Add render task configuration. */
	void AddRenderTaskConfig( const deoglRenderTaskConfig &config, int specialFlags );
	/*@}*/
	
	
	
private:
	void pUpdateFilters();
	bool pFilterReject( const deoglSkinTexture &skinTexture ) const;
	bool pFilterRejectNoSolid( const deoglSkinTexture &skinTexture ) const;
	
	deoglRenderTaskVAO *pGetTaskVAO( deoglSkinTexture::eShaderTypes shaderType,
		const deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc, deoglVAO *vao ) const;
};

#endif
