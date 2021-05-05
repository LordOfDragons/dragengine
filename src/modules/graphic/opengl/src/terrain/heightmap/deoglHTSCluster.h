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

#include <dragengine/common/math/decMath.h>

#define HTSC_MAX_LOD				4

class deHeightTerrainSector;
class deoglRHTSector;
class deImage;
class deoglTerrainHeightImage;
class deoglVAO;
class deoglRenderTaskSharedInstance;


enum eHTSCBorders{
	ehtscbLeft = 0,
	ehtscbTop,
	ehtscbRight,
	ehtscbBottom,
	ehtscbFixLeft,
	ehtscbFixTop,
	ehtscbFixRight,
	ehtscbFixBottom
};

/** Height Terrain Sector Cluster LOD. */
struct deoglHTSClusterLOD{
	int firstBasePoint;
	int basePointCount;
	
	int firstBorderPoint[ 8 ];
	int borderPointCount[ 8 ];
};



/**
 * @brief Height Terrain Sector Cluster.
 *
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
	deoglRHTSector *pHTSector;
	
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
	
	deoglRenderTaskSharedInstance *pRTSInstance[ 5 ];
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new cluster. */
	deoglHTSCluster();
	/** Cleans up the cluster. */
	~deoglHTSCluster();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets the height terrain sector. */
	void SetHTSector( deoglRHTSector *htsector );
	
	/** Retrieves the first point in X direction. */
	inline int GetFirstPointX() const{ return pFirstPointX; }
	/** Retrieves the first point in Z direction. */
	inline int GetFirstPointZ() const{ return pFirstPointZ; }
	/** Retrieves the number of points in X direction. */
	inline int GetPointCountX() const{ return pPointCountX; }
	/** Retrieves the number of points in Z direction. */
	inline int GetPointCountZ() const{ return pPointCountZ; }
	/** Sets the size. */
	void SetSize( int firstPointX, int firstPointZ, int pointCountX, int pointCountZ );
	
	/** Retrieves the center of the enclosing box. */
	inline const decVector &GetCenter() const{ return pCenter; }
	/** Retrieves the half extends of the enclosing box. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/** \brief Initializes the cluster from the given height image during main thread. */
	void InitFromHeightImage( const deHeightTerrainSector &sector );
	
	/** Determines if this cluster has no lod levels. */
	inline bool GetNoLOD() const{ return pNoLOD; }
	/** Retrieves the given LOD level. */
	deoglHTSClusterLOD &GetLODAt( int level );
	
	/** Updates the height extends. */
	void UpdateHeightExtends( float minHeight, float maxHeight );
	
	/** Print out some debuging information. */
	void DebugPrint();
	
	/** Retrieves the vbo point offset. */
	inline int GetOffsetVBODataPoints() const{ return pOffsetVBODataPoints; }
	/** Sets the offset to the data vbo. */
	void SetOffsetVBODataPoints( int offset );
	/** Retrieves the number of data vbo points. */
	inline int GetCountVBODataPoints() const{ return pDataPointCount; }
	
	/** Retrieves the position data vbo. */
	inline GLuint GetVBODataPoints1() const{ return pVBODataPoints1; }
	/** Sets the position data vbo. */
	void SetVBODataPoints1( GLuint vbo );
	/** Updates the position vbo. */
	void UpdateVBOData1();
	
	/** Retrieves the height data vbo. */
	inline GLuint GetVBODataPoints2() const{ return pVBODataPoints2; }
	/** Sets the height data vbo. */
	void SetVBODataPoints2( GLuint vbo );
	/** Update the height vbo. */
	void UpdateVBOData2();
	
	/** Retrieves the vao. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	/** Update the vao. */
	void UpdateVAO();
	
	/** Retrieves the vbo data faces offset. */
	inline int GetOffsetVBODataFaces() const{ return pOffsetVBODataFaces; }
	/** Sets the offset to the data faces vbo. */
	void SetOffsetVBODataFaces( int offset );
	/** Retrieves the number of data vbo faces. */
	inline int GetCountVBODataFaces() const{ return pFacePointCount; }
	/** Retrieves the faces data vbo. */
	inline GLuint GetVBODataFaces() const{ return pVBODataFaces; }
	/** Sets the faces data vbo. */
	void SetVBODataFaces( GLuint vbo );
	/** Updates the data faces vbo. */
	void UpdateVBODataFaces();
	
	/** Render task shared instance. */
	inline deoglRenderTaskSharedInstance *GetRTSInstanceAt( int index ) const{ return pRTSInstance[ index ]; }
	
	/** Update render task shared instances. */
	void UpdateRTSInstances();
	/*@}*/
	
	/** @name Face Points */
	/*@{*/
	/** Retrieves the face points. */
	inline GLushort *GetFacePoints() const{ return pFacePoints; }
	/** Retrieves the number of face points. */
	inline int GetFacePointCount() const{ return pFacePointCount; }
	/** Adds a face point. */
	void AddFacePoints( int p1, int p2, int p3 );
	/** Sets the face point count. Has to be less or equal the current point count. */
	void SetFacePointCount( int count );
	/*@}*/
	
private:
	void pCreateBaseLOD( const deHeightTerrainSector &sector, int width );
	void pCreateCheapLOD( int lodLevel );
};

#endif
