/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAISPACE_H_
#define _DEDAISPACE_H_

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>

class dedaiLayer;
class dedaiHeightTerrainNavSpace;
class dedaiNavSpace;
class deDebugDrawer;
class deDebugDrawerShape;
class dedaiWorld;
class dedaiSpaceMesh;
class dedaiSpaceGrid;
class deDEAIModule;



/**
 * \brief Navigation space peer.
 */
class dedaiSpace{
private:
	deDEAIModule &pDEAI;
	dedaiWorld *pParentWorld;
	
	dedaiNavSpace *pOwnerNavSpace;
	dedaiHeightTerrainNavSpace *pOwnerHTNavSpace;
	
	deNavigationSpace::eSpaceTypes pType;
	int pLayerNumber;
	dedaiLayer *pLayer;
	decDVector pPosition;
	decQuaternion pOrientation;
	float pSnapDistance;
	float pSnapAngle;
	int pBlockingPriority;
	decShapeList pBlockerShape;
	
	dedaiSpaceGrid *pGrid;
	dedaiSpaceMesh *pMesh;
	
	decDMatrix pMatrix;
	decDMatrix pInvMatrix;
	decDVector pMinExtends;
	decDVector pMaxExtends;
	bool pDirtyMatrix;
	bool pDirtyExtends;
	bool pDirtyLayout;
	bool pDirtyLinks;
	
	bool pDirtyBlocking;
	
	decConvexVolumeList pBlockerConvexVolumeList;
	
	deDebugDrawer *pDebugDrawer;
	deDebugDrawerShape *pDDSSpace;
	deDebugDrawerShape *pDDSCorners;
	deDebugDrawerShape *pDDSNormals;
	deDebugDrawerShape *pDDSMismatching;
	deDebugDrawerShape *pDDSHilightCostType;
	
	int pDebugLastHilightCostType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space. */
	dedaiSpace( deDEAIModule &deai );
	
	/** \brief Clean up space. */
	~dedaiSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief AI module. */
	inline deDEAIModule &GetDEAI() const{ return pDEAI; }
	
	
	
	/** \brief Parent world or \em NULL. */
	inline dedaiWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( dedaiWorld *world );
	
	
	
	/** \brief Owner navigation space or \em NULL. */
	inline dedaiNavSpace *GetOwnerNavSpace() const{ return pOwnerNavSpace; }
	
	/** \brief Set owner navigation space or \em NULL. */
	void SetOwnerNavSpace( dedaiNavSpace *navSpace );
	
	/** \brief Owner height terrain navigation space or \em NULL. */
	inline dedaiHeightTerrainNavSpace *GetOwnerHTNavSpace() const{ return pOwnerHTNavSpace; }
	
	/** \brief Set owner height terrain navigation space or \em NULL. */
	void SetOwnerHTNavSpace( dedaiHeightTerrainNavSpace *htNavSpace );
	
	
	
	/** \brief Space type. */
	inline deNavigationSpace::eSpaceTypes GetType() const{ return pType; }
	
	/** \brief Set space type. */
	void SetType( deNavigationSpace::eSpaceTypes type );
	
	/** \brief Layer number. */
	inline int GetLayerNumber() const{ return pLayerNumber; }
	
	/** \brief Set layer number. */
	void SetLayerNumber( int layerNumber );
	
	/** \brief Layer or \em NULL if not in a world. */
	inline dedaiLayer *GetLayer() const{ return pLayer; }
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Snap distance. */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/** \brief Set snap distance. */
	void SetSnapDistance( float distance );
	
	/** \brief Snap angle. */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/** \brief Set snap angle. */
	void SetSnapAngle( float angle );
	
	/** \brief Blocking priority. */
	inline int GetBlockingPriority() const{ return pBlockingPriority; }
	
	/** \brief Set blocking priority. */
	void SetBlockingPriority( int priority );
	
	/** \brief Blocker shape. */
	inline const decShapeList &GetBlockerShape() const{ return pBlockerShape; }
	
	/** \brief Set blocker shape. */
	void SetBlockerShape( const decShapeList &shape );
	
	
	
	/** \brief Matrix. */
	const decDMatrix &GetMatrix();
	
	/** \brief Inverse matrix. */
	const decDMatrix &GetInverseMatrix();
	
	/** \brief Minimum extends. */
	const decDVector &GetMinimumExtends();
	
	/** \brief Maximum extends. */
	const decDVector &GetMaximumExtends();
	
	
	
	/** \brief Grid or \em NULL. */
	inline dedaiSpaceGrid *GetGrid() const{ return pGrid; }
	
	/** \brief Mesh or \em NULL. */
	inline dedaiSpaceMesh *GetMesh() const{ return pMesh; }
	
	
	
	/** \brief Add type mapping if absent returning the mapping index. */
	int AddTypeMapping( int typeNumber );
	
	
	
	/** \brief Blocker convex volume list used to block other navigation spaces. */
	inline decConvexVolumeList &GetBlockerConvexVolumeList(){ return pBlockerConvexVolumeList; }
	inline const decConvexVolumeList &GetBlockerConvexVolumeList() const{ return pBlockerConvexVolumeList; }
	
	
	
	/** \brief Prepare space if required. */
	void Prepare();
	
	/** \brief Prepare link if required. */
	void PrepareLinks();
	
	/** \brief Clear links. */
	void ClearLinks();
	
	
	
	/** \brief Invalidate due to blocking change. */
	void InvalidateBlocking();
	
	/** \brief Layout of owner content changed. */
    void OwnerLayoutChanged();
	
	/**
	 * \brief Link of space removed.
	 * \details This is called by dedaiSpaceMesh or dedaiSpaceGrid only to trigger an dupdate
	 *          of links the next time possible. Do not call directly.
	 */
	void LinksRemoves();
	
	
	
	/** \brief Debug drawer shape for space or \em NULL. */
	inline deDebugDrawerShape *GetDDSSpace() const{ return pDDSSpace; }
	
	/** \brief Debug drawer shape for corners or \em NULL. */
	inline deDebugDrawerShape *GetDDSCorners() const{ return pDDSCorners; }
	
	/** \brief Debug drawer shape for normals or \em NULL. */
	inline deDebugDrawerShape *GetDDSNormals() const{ return pDDSNormals; }
	
	/** \brief Debug drawer shape for mismatches or \em NULL. */
	inline deDebugDrawerShape *GetDDSMismatching() const{ return pDDSMismatching; }
	
	/** \brief Debug drawer shape for hilighting cost types or \em NULL. */
	inline deDebugDrawerShape *GetDDSHilightCostType() const{ return pDDSHilightCostType; }
	
	
	
	/**
	 * \brief Update debug drawer shape for the space.
	 * 
	 * If space visualization is enabled in the developer mode ensures a debug
	 * drawer shape exists and is added to the appropriate parent world debug
	 * drawer object. If visualization is disabled ensures the the debug drawer
	 * shape is deleted and removed from the appropriate parent world debug
	 * drawer object. Updating the shape is handled by UpdateDDSSpaceShape.
	 */
	void UpdateDDSSpace();
	
	/** \brief Update debug drawer shape from the space if existing. */
	void UpdateDDSSpaceShape();
	
	/** \brief Add overlapping blockers in parent world as convex volume. */
	void AddBlockerSplitters( decConvexVolumeList &list );
	
	/**
	 * \brief Add overlapping space blocker shapes in parent world as convex volume.
	 * \details Exclude owner.
	 */
	void AddSpaceBlockerSplitters( decConvexVolumeList &list );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateMatrices();
	
	void pUpdateExtends();
	void pUpdateExtendsNavSpace();
	void pUpdateExtendsHTNavSpace();
	
	void pUpdateSpace();
	void pUpdateBlocking();
	void pUpdateBlockerConvexVolumeList();
	
	void pInvalidateLayerBlocking();
	void pInvalidateLayerLinks();
};

#endif
