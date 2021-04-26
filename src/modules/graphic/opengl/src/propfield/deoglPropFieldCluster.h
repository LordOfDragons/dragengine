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

class dePropFieldType;
class deoglRPropFieldType;
class deoglTexUnitsConfig;



/**
 * @brief Prop Field Cluster.
 * Cluster in a prop field resource. Clusters are a cubic area containing a set
 * of prop field instances belonging to the same type.
 */
class deoglPropFieldCluster{
public:
	/** \brief Instance. */
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
	
	bool pDirtyTUCDepth;
	bool pDirtyTUCGeometry;
	bool pDirtyTUCShadow;
	bool pDirtyTUCEnvMap;
	bool pDirtyBendStates;
	
	GLuint pTBOInstances;
	GLuint pTBOBendStates;
	GLuint pVBOInstances;
	GLuint pVBOBendStates;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new prop field cluster. */
	deoglPropFieldCluster( deoglRPropFieldType &propFieldType );
	/** Cleans up the prop field cluster. */
	~deoglPropFieldCluster();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Prop field type. */
	inline deoglRPropFieldType &GetPropFieldType() const{ return pPropFieldType; }
	
	/** Retrieves the minimum extend. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	/** Sets the extends. */
	void SetExtends( const decVector &minExtend, const decVector &maxExtend );
	
	/** Retrieves the number of instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	/** Sets the number of instances. */
	void SetInstanceCount( int count );
	/** Retrieves the instances. */
	inline sInstance *GetInstances() const{ return pInstances; }
	
	/** Retrieves the instances TBO. */
	inline GLuint GetTBOInstances() const{ return pTBOInstances; }
	/** Retrieves the bend states TBO. */
	inline GLuint GetTBOBendStates() const{ return pTBOBendStates; }
	/** Retrieves the instance VBO. */
	inline GLuint GetVBOInstances() const{ return pVBOInstances; }
	/** Retrieves the bend states VBO. */
	inline GLuint GetVBOBendStates() const{ return pVBOBendStates; }
	/** Update tbos if required. */
	void UpdateTBOs();
	/** Update tbo instances. */
	void UpdateTBOInstances();
	/** Update tbo bend states. */
	void UpdateTBOBendStates();
	
	/** \brief Prepare bend state data. */
	void PrepareBendStateData( const dePropFieldType &type );
	
	/** Retrieves the texture units configuration for the given shader type. */
	deoglTexUnitsConfig *GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType );
	/**
	 * Retrieves the texture units configuration for depth type shaders or NULL if empty.
	 * This texture units configuration works for the shader types estComponentDepth, estComponentDepthClipPlane,
	 * estComponentCounter and estComponentCounterClipPlane.
	 */
	deoglTexUnitsConfig *GetTUCDepth();
	/**
	 * Retrieves the texture units configuration for geometry type shaders or NULL if empty.
	 * This texture units configuration works for the shader type estComponentGeometry.
	 */
	deoglTexUnitsConfig *GetTUCGeometry();
	/**
	 * Retrieves the texture units configuration for shadow type shaders or NULL if empty.
	 * This texture units configuration works for these shader types:
	 * - estComponentShadowProjection
	 * - estComponentShadowOrthogonal
	 * - estComponentShadowOrthogonalCascaded
	 * - estComponentShadowDistance
	 */
	deoglTexUnitsConfig *GetTUCShadow();
	/**
	 * Retrieves the texture units configuration for the environment map shader or NULL if empty.
	 * This texture units configuration works for the shader type estEnvMap.
	 */
	deoglTexUnitsConfig *GetTUCEnvMap();
	/** Obtain texture units configuration for a shader type. Bare call not to be used directly. */
	deoglTexUnitsConfig *BareGetTUCFor( deoglSkinTexture::eShaderTypes shaderType ) const;
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	/*@}*/
};

#endif
