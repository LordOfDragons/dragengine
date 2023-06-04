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

#ifndef _DEOGLHTSCLUSTER_H_
#define _DEOGLHTSCLUSTER_H_

#include "../../deoglBasics.h"
#include "../../world/deoglWorldComputeElement.h"

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#define HTSC_MAX_LOD				4

class deHeightTerrainSector;
class deoglRHTSector;
class deImage;
class deoglTerrainHeightImage;
class deoglVAO;


/** Height Terrain Sector Cluster LOD. */
struct deoglHTSClusterLOD{
	int firstBasePoint;
	int basePointCount;
	
	int firstBorderPoint[ 8 ];
	int borderPointCount[ 8 ];
};



/**
 * Cluster in a height terrain sector. Stores the maximum variance in the cluster
 * as well as an axis aligned box enclosing all contained vertices. The AABB is
 * stored only as the middle height and half-height since the X and Z values of
 * the box are given explicitely by the cluster coordinates. In addition a list
 * of LOD meshes is stored for all the levels already retrieved. Also stored is
 * a list of inter-cluster-connection faces to fill the borders matching against
 * different LOD levels of the neighbors.
 */
class deoglHTSCluster{
private:
	/** World compute element. */
	class WorldComputeElement: public deoglWorldComputeElement{
		deoglHTSCluster &pCluster;
	public:
		WorldComputeElement( deoglHTSCluster &cluster );
		virtual void UpdateData( sDataElement &data ) const;
		virtual void UpdateDataGeometries( sDataElementGeometry *data ) const;
	};
	
	
	
	deoglRHTSector *pHTSector;
	decPoint pCoordinates;
	int pIndex;
	
	deoglWorldComputeElement::Ref pWorldComputeElement;
	
	int pFirstPointX;
	int pFirstPointZ;
	int pPointCountX;
	int pPointCountZ;
	
	decVector pCenter;
	decVector pHalfExtends;
	
	deoglHTSClusterLOD pLOD[ HTSC_MAX_LOD + 1 ];
	bool pNoLOD;
	
	GLushort *pFacePoints;
	int pFacePointCount;
	int pFacePointSize;
	
	GLuint pVBODataPoints1;
	GLuint pVBODataPoints2;
	int pOffsetVBODataPoints;
	GLuint pVBODataFaces;
	int pOffsetVBODataFaces;
	deoglVAO *pVAO;
	
	deoglVBOHeightTerrain1 *pDataPoints1;
	deoglVBOHeightTerrain2 *pDataPoints2;
	int pDataPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create cluster. */
	deoglHTSCluster();
	
	/** Clean up cluster. */
	~deoglHTSCluster();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Height terrain sector. */
	inline deoglRHTSector *GetHTSector() const{ return pHTSector; }
	
	/** Set height terrain sector. */
	void SetHTSector( deoglRHTSector *htsector );
	
	/** Coordinates. */
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	
	/** Set coordinates. */
	void SetCoordinates( const decPoint &coordinates );
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
	
	
	/** First point in X direction. */
	inline int GetFirstPointX() const{ return pFirstPointX; }
	
	/** First point in Z direction. */
	inline int GetFirstPointZ() const{ return pFirstPointZ; }
	
	/** Count of points in X direction. */
	inline int GetPointCountX() const{ return pPointCountX; }
	
	/** Count of points in Z direction. */
	inline int GetPointCountZ() const{ return pPointCountZ; }
	
	/** Set size. */
	void SetSize( int firstPointX, int firstPointZ, int pointCountX, int pointCountZ );
	
	
	
	/** Center of the enclosing box. */
	inline const decVector &GetCenter() const{ return pCenter; }
	
	/** Half extends of the enclosing box. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	
	
	
	/** Initializes the cluster from the given height image during main thread. */
	void InitFromHeightImage( const deHeightTerrainSector &sector );
	
	
	
	/** Cluster has no lod levels. */
	inline bool GetNoLOD() const{ return pNoLOD; }
	
	/** LOD level. */
	deoglHTSClusterLOD &GetLODAt( int level );
	const deoglHTSClusterLOD &GetLODAt( int level ) const;
	
	
	
	/** Update height extends. */
	void UpdateHeightExtends( float minHeight, float maxHeight );
	
	/** Print out some debuging information. */
	void DebugPrint();
	
	
	
	/** VBO point offset. */
	inline int GetOffsetVBODataPoints() const{ return pOffsetVBODataPoints; }
	
	/** Set offset data vbo. */
	void SetOffsetVBODataPoints( int offset );
	
	/** Count of data vbo points. */
	inline int GetCountVBODataPoints() const{ return pDataPointCount; }
	
	/** Position data vbo. */
	inline GLuint GetVBODataPoints1() const{ return pVBODataPoints1; }
	
	/** Set position data vbo. */
	void SetVBODataPoints1( GLuint vbo );
	
	/** Update position vbo. */
	void UpdateVBOData1();
	
	/** Height data vbo. */
	inline GLuint GetVBODataPoints2() const{ return pVBODataPoints2; }
	
	/** Set height data vbo. */
	void SetVBODataPoints2( GLuint vbo );
	
	/** Update height vbo. */
	void UpdateVBOData2();
	
	
	
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** Update vao. */
	void UpdateVAO();
	
	
	
	/** VBO data faces offset. */
	inline int GetOffsetVBODataFaces() const{ return pOffsetVBODataFaces; }
	
	/** Set offset data faces vbo. */
	void SetOffsetVBODataFaces( int offset );
	
	/** Count of data vbo faces. */
	inline int GetCountVBODataFaces() const{ return pFacePointCount; }
	
	/** Faces data vbo. */
	inline GLuint GetVBODataFaces() const{ return pVBODataFaces; }
	
	/** Set faces data vbo. */
	void SetVBODataFaces( GLuint vbo );
	
	/** Update data faces vbo. */
	void UpdateVBODataFaces();
	
	
	
	/** Add to world compute. */
	void AddToWorldCompute( deoglWorldCompute &worldCompute );
	
	/** Update world compute. */
	void UpdateWorldCompute();
	
	/** Update world compute textures. */
	void UpdateWorldComputeTextures();
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute();
	/*@}*/
	
	
	
	/** \name Face Points */
	/*@{*/
	/** Face points. */
	inline GLushort *GetFacePoints() const{ return pFacePoints; }
	
	/** Count of face points. */
	inline int GetFacePointCount() const{ return pFacePointCount; }
	
	/** Add face point. */
	void AddFacePoints( int p1, int p2, int p3 );
	
	/** Set face point count. Has to be less or equal the current point count. */
	void SetFacePointCount( int count );
	/*@}*/
	
	
	
private:
	void pCreateBaseLOD( const deHeightTerrainSector &sector, int width );
	void pCreateCheapLOD( int lodLevel );
};

#endif
