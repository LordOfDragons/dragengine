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

#ifndef _DEOGLPROPFIELDCLUSTER_H_
#define _DEOGLPROPFIELDCLUSTER_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglBasics.h"
#include "../skin/deoglSkinTexture.h"
#include "../world/deoglWorldComputeElement.h"

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
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglPropFieldCluster &pCluster;
	public:
		WorldComputeElement( deoglPropFieldCluster &cluster );
		virtual void UpdateData( sDataElement &data ) const;
		virtual void UpdateDataGeometries( sDataElementGeometry *data ) const;
	};
	
	
	
	deoglRPropFieldType &pPropFieldType;
	deoglRenderThread &pRenderThread; // to avoid segfault during destructor
	
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
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
	
	
	
	/** Add to world compute. */
	void AddToWorldCompute( deoglWorldCompute &worldCompute );
	
	/** Update world compute. */
	void UpdateWorldCompute();
	
	/** Update world compute textures. */
	void UpdateWorldComputeTextures();
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute();
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
