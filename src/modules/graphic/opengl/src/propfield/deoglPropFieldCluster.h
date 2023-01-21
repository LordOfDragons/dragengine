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

#ifndef _DEOGLPROPFIELDCLUSTER_H_
#define _DEOGLPROPFIELDCLUSTER_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"
#include "../skin/deoglSkinTexture.h"

class deoglRPropFieldType;
class deoglTexUnitsConfig;
class deoglRenderTaskSharedInstance;

class dePropFieldType;


/**
 * Cluster in a prop field resource. Clusters are a cubic area containing a set of
 * prop field instances belonging to the same type.
 */
class deoglPropFieldCluster{
public:
	/** Instance. */
	struct sInstance{
		int instance;
		float rotation[ 9 ];
		float position[ 3 ];
		float scaling;
		int bstate;
	};
	
	
	
private:
	deoglRPropFieldType &pPropFieldType;
	deoglRenderThread &pRenderThread; // to avoid segfault during destructor
	
	decVector pMinExtend;
	decVector pMaxExtend;
	
	sInstance *pInstances;
	int pInstanceCount;
	
	char *pBendStateData;
	int pBendStateDataSize;
	
	deoglTexUnitsConfig *pTUCDepth;
	deoglTexUnitsConfig *pTUCGeometry;
	deoglTexUnitsConfig *pTUCShadow;
	deoglTexUnitsConfig *pTUCEnvMap;
	
	bool pDirtyTUCs;
	bool pDirtyBendStates;
	
	GLuint pTBOInstances;
	GLuint pTBOBendStates;
	GLuint pVBOInstances;
	GLuint pVBOBendStates;
	
	deoglRenderTaskSharedInstance *pRTSInstance;
	bool pDirtyRTSInstance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new prop field cluster. */
	deoglPropFieldCluster( deoglRPropFieldType &propFieldType );
	
	/** Clean up prop field cluster. */
	~deoglPropFieldCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field type. */
	inline deoglRPropFieldType &GetPropFieldType() const{ return pPropFieldType; }
	
	
	
	/** Minimum extend. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Set extends. */
	void SetExtends( const decVector &minExtend, const decVector &maxExtend );
	
	
	
	/** Count of instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** Set number of instances. */
	void SetInstanceCount( int count );
	
	/** Instances. */
	inline sInstance *GetInstances() const{ return pInstances; }
	
	/** Instances TBO. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	
	/** Bend states TBO. */
	inline GLuint GetTBOBendStates() const{ return pTBOBendStates; }
	
	/** Instance VBO. */
	inline GLuint GetVBOInstances() const{ return pVBOInstances; }
	
	/** Bend states VBO. */
	inline GLuint GetVBOBendStates() const{ return pVBOBendStates; }
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender();
	
	/**
	 * Prepare bend state data.
	 * \warning Called during synchronization by main thread.
	 */
	void PrepareBendStateData( const dePropFieldType &type );
	
	/** Texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForPipelineType( deoglSkinTexturePipelines::eTypes type ) const;
	
	/**
	 * Texture units configuration for depth type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estComponentDepth
	 * - estComponentDepthClipPlane
	 * - estComponentCounter
	 * - estComponentCounterClipPlane
	 */
	inline deoglTexUnitsConfig *GetTUCDepth() const{ return pTUCDepth; }
	
	/**
	 * Texture units configuration for geometry type shaders or NULL if empty.
	 * Works for these shader types:
	 * - estComponentGeometry
	 */
	inline deoglTexUnitsConfig *GetTUCGeometry() const{ return pTUCGeometry; }
	
	/**
	 * Texture units configuration for shadow type shaders or NULL if empty.
	 * Workd for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	inline deoglTexUnitsConfig *GetTUCShadow() const{ return pTUCShadow; }
	
	/**
	 * Texture units configuration for the environment map shader or NULL if empty.
	 * Works for these shader type:
	 * - estEnvMap
	 */
	inline deoglTexUnitsConfig *GetTUCEnvMap() const{ return pTUCEnvMap; }
	
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexturePipelines::eTypes type ) const;
	
	/** Mark texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstance() const{ return pRTSInstance; }
	
	/** Mark render task shared instance dirty. */
	void DirtyRTSInstance();
	/*@}*/
	
	
	
private:
	void pPrepareTUCs();
	
	void pPrepareTBOs();
	void pUpdateTBOInstances();
	void pUpdateTBOBendStates();
	
	void pPrepareRTSInstance();
	void pUpdateRTSInstances();
};

#endif
