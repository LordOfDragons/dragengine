/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHEIGHTTERRAINSECTOR_H_
#define _MEHEIGHTTERRAINSECTOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImageReference.h>

class meBitArray;
class meHeightTerrain;
class meHeightTerrainNavSpace;
class meHeightTerrainPFLayer;
class meHeightTerrainPropField;
class meHeightTerrainTexture;
class meHTVegetationLayer;

class deDebugDrawer;
class deDebugDrawerShape;
class deEngine;
class deHeightTerrainSector;
class dePropField;
class deWorld;

class igdeEnvironment;
class igdeWDebugDrawerShape;



/**
 * \brief Height Terrain Sector.
 *
 * Defines the height terrain related informations inside a world sector.
 */
class meHeightTerrainSector : public deObject{
public:
	/** Data types. */
	enum eDataTypes{
		/** 8-bit integer. */
		edtInt8,
		/** 16-bit integer. */
		edtInt16,
		/** float. */
		edtFloat
	};
	
	
	
public:
	/** \brief Debug drawer cluster for performance optimization. */
	struct sDDCluster{
		decPoint pointFrom;
		decPoint pointTo;
		decPoint faceCount;
		decVector boxFrom;
		decVector boxTo;
	};
	
	
	
private:
	meHeightTerrain *pHeightTerrain;
	
	deEngine *pEngine;
	deHeightTerrainSector *pEngSector;
	
	int pDDPointsPerCluster;
	int pDDClusterCount;
	sDDCluster *pDDClusters;
	decObjectList pDDEdges;
	
	decPoint pCoordinates;
	
	bool pActive;
	
	float pRadiusOfInfluence;
	
	// height image
	deImageReference pHeightImage;
	int pDataType;
	
	decString pPathHeightImage;
	bool pHeightImageChanged;
	bool pHeightImageSaved;
	
	// visibility image
	meBitArray *pVisFaces;
	
	decString pPathVisImage;
	bool pVisImageChanged;
	bool pVisImageSaved;
	
	// textures
	meHeightTerrainTexture **pTextures;
	int pTextureCount;
	int pTextureSize;
	meHeightTerrainTexture *pActiveTexture;
	
	// prop fields
	meHeightTerrainPropField **pPropFields;
	int pPropFieldCount;
	int pPropFieldCellCount;
	
	meHeightTerrainPFLayer **pPFLayers;
	int pPFLayerCount;
	int pPFLayerSize;
	meHeightTerrainPFLayer *pActivePFLayer;
	
	decString pPathPFCache;
	bool pPFCacheChanged;
	bool pPFCacheSaved;
	
	// navigation spaces
	decObjectList pNavSpaces;
	meHeightTerrainNavSpace *pActiveNavSpace;
	decIntList pSelectedNavPoints;
	
	deDebugDrawer *pDDSelNavPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new height terrain sector. */
	meHeightTerrainSector( deEngine *engine, const decPoint &coordinates );
	/** Cleans up the height terrain sector. */
	virtual ~meHeightTerrainSector();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Rebuilds the engine sector. */
	void RebuildEngineSector();
	
	/** Retrieves the parent height terrain. */
	inline meHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	/** Sets the parent height terrain. */
	void SetHeightTerrain( meHeightTerrain *heightTerrain );
	
	/** \brief Engine height terrain sector. */
	inline deHeightTerrainSector *GetEngineSector() const{ return pEngSector; }
	
	/** \brief Create engine height terrain sector if absent. */
	void CreateEngineSector();
	
	/** \brief Destroy engine height terrain sector if present. */
	void DestroyEngineSector();
	
	
	
	/** \brief Points per debug drawer cluster. */
	inline int GetDDPointsPerCluster() const{ return pDDPointsPerCluster; }
	
	/** \brief Number of debug drawer clusters. */
	inline int GetDDClusterCount() const{ return pDDClusterCount; }
	
	/** \brief Debug drqwer clusters. */
	inline const sDDCluster *GetDDClusters() const{ return pDDClusters; }
	
	
	
	/** Retrieves the coordinates. */
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	/** Sets the coordinates. */
	void SetCoordinates( const decPoint &coordinates );
	
	/** \brief Height terrain sector size or resolution changed. */
	void SectorSizeOrResChanged();
	
	/** Notifies the engine object that the sector changed. */
	void NotifySectorChanged();
	
	/** \brief Sector is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if sector is active. */
	void SetActive( bool active );
	
	/** Init the delegates. */
	void InitDelegates( igdeEnvironment *environment );
	
	/** \brief Get coordinates in grid points (0 to image-dimension-1) for point. */
	decVector2 GetGridPointAt( const decDVector &position ) const;
	
	/** \brief Height at grid point. */
	float GetHeightAtPoint( int x, int z ) const;
	
	/** Retrieves the height image relative coordinates. */
	void GetHeightLocalAt( float x, float z, int &imgX, int &imgZ, float &imgFacX, float &imgFacZ ) const;
	/** Determines the height at a given position. */
	bool GetHeightAt( float x, float z, float &height ) const;
	/** Retrieves height terrain parameters at a given position. */
	bool GetHeightNormalAt( float x, float z, float &height, decVector &normal ) const;
	
	/** \brief World position of sector center. */
	decDVector GetWorldPosition() const;
	
	/** Updates the height terrain sector. */
	void Update();
	/** Updates the prop field vegetation instances. */
	void UpdateVInstances();
	/** Clear vegetation. */
	void ClearVegetation();
	
	/** \brief Rebuild height terrain engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	
	/** Retrieves the radius of influence due to vegetation rules. */
	inline float GetRadiusOfInfluence() const{ return pRadiusOfInfluence; }
	/** Sets the radius of influence due to vegetation rules. */
	void SetRadiusOfInfluence( float radius );
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
	/** \name Height Image */
	/*@{*/
	/** Retrieves the height image or null. */
	inline deImage *GetHeightImage() const{ return pHeightImage; }
	
	/** Retrieves the data type. */
	inline int GetDataType() const{ return pDataType; }
	/** Sets the data type. */
	void SetDataType( int dataType );
	/** Determines if the height values have to be limited to the range from -0.5 to 0.5 inclusive. */
	bool DoClampValues() const;
	
	/** Retrieves the height image path. */
	inline const decString &GetPathHeightImage() const{ return pPathHeightImage; }
	/** Sets the height image path. */
	void SetPathHeightImage( const char *path, bool loadFile );
	/** Determines if the height image has changed. */
	inline bool GetHeightImageChanged() const{ return pHeightImageChanged; }
	/** Sets if the height image has changed. */
	void SetHeightImageChanged( bool changed );
	/** Determines if the height image has been saved. */
	inline bool GetHeightImageSaved() const{ return pHeightImageSaved; }
	/** Sets if the height image has been saved. */
	void SetHeightImageSaved( bool saved );
	/*@}*/
	
	/** \name Visibility */
	/*@{*/
	/** Retrieves the bit array with the faces visibility. */
	inline meBitArray *GetVisibilityFaces() const{ return pVisFaces; }
	
	/** Retrieves the visibility image path. */
	inline const decString &GetPathVisibilityImage() const{ return pPathVisImage; }
	/** Sets the visibility image path. */
	void SetPathVisibilityImage( const char *path );
	/** Determines if the visibility has changed. */
	inline bool GetVisibilityChanged() const{ return pVisImageChanged; }
	/** Sets if the visibility has changed. */
	void SetVisibilityChanged( bool changed );
	/** Determines if the visibility has been saved. */
	inline bool GetVisibilitySaved() const{ return pVisImageSaved; }
	/** Sets if the visibility has been saved. */
	void SetVisibilitySaved( bool saved );
	
	/** Inits the values from the given height map. */
	void InitVisibilityFromSector( deHeightTerrainSector *sector ) const;
	/** Sets the value in the given height map using the stored values. */
	void UpdateVisibilitySector( deHeightTerrainSector *sector ) const;
	
	/** Loads the visibility from the stored path. */
	void LoadVisibilityFromImage();
	/*@}*/
	
	/** \name Texture */
	/*@{*/
	/** Retrieves the number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	/** Retrieves the texture at the given index. */
	meHeightTerrainTexture *GetTextureAt( int index ) const;
	/** Retrieves the texture with the given name of NULL if not found. */
	meHeightTerrainTexture *GetTextureNamed( const char *name ) const;
	/** Determines if a texture with the given name exists. */
	bool HasTextureNamed( const char *name ) const;
	/** Retrieves the index of the texture or -1 if not found. */
	int IndexOfTexture( meHeightTerrainTexture *texture ) const;
	/** Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed( const char *name ) const;
	/** Determines if the texture exists. */
	bool HasTexture( meHeightTerrainTexture *texture ) const;
	/** Adds a texture. */
	void AddTexture( meHeightTerrainTexture *texture );
	/** Removes a texture. */
	void RemoveTexture( meHeightTerrainTexture *texture );
	/** Removes all textures. */
	void RemoveAllTextures();
	/** Retrieves the active texture or NULL. */
	inline meHeightTerrainTexture *GetActiveTexture() const{ return pActiveTexture; }
	/** Sets the active texture or NULL. */
	void SetActiveTexture( meHeightTerrainTexture *texture );
	/** Retrieves the texture under the given point. */
	meHeightTerrainTexture *GetTextureUnder( float x, float y ) const;
	/*@}*/
	
	
	
	/** \name Prop Fields */
	/*@{*/
	/** Retrieves the number of prop fields. */
	inline int GetPFLayerCount() const{ return pPFLayerCount; }
	/** Retrieves the prop field at the given index. */
	meHeightTerrainPFLayer *GetPFLayerAt( int index ) const;
	/** Retrieves the index of the prop field or -1 if not found. */
	int IndexOfPFLayer( meHeightTerrainPFLayer *pflayer ) const;
	/** Determines if the prop field exists. */
	bool HasPFLayer( meHeightTerrainPFLayer *pflayer ) const;
	/** Adds a prop field. */
	void AddPFLayer( meHeightTerrainPFLayer *pflayer );
	/** Removes a prop field. */
	void RemovePFLayer( meHeightTerrainPFLayer *pflayer );
	/** Removes all prop fields. */
	void RemoveAllPFLayers();
	/** Retrieves the active prop field or NULL. */
	inline meHeightTerrainPFLayer *GetActivePFLayer() const{ return pActivePFLayer; }
	/** Sets the active prop field or NULL. */
	void SetActivePFLayer( meHeightTerrainPFLayer *pflayer );
	
	/** Retrieves the number of cells prop fields. */
	inline int GetPropFieldCellCount() const{ return pPropFieldCellCount; }
	/** Retrieves the number of prop fields. */
	inline int GetPropFieldCount() const{ return pPropFieldCount; }
	/** Retrieves the prop field at the given index. */
	meHeightTerrainPropField *GetPropFieldAt( int index ) const;
	
	/** \brief Height image of sector changed. */
	void InvalidateHeights( const decPoint &fromGrid, const decPoint &toGrid );
	
	/** Invalidate prop fields in the given area. */
	void InvalidatePropFields( const decPoint &fromGrid, const decPoint &toGrid );
	
	/** \brief Invalidate all prop. */
	void InvalidateAllPropFields();
	
	/** \brief Validate all prop fields. */
	void ValidateAllPropFields();
	
	/** Retrieves the prop field cache path. */
	inline const decString &GetPathPFCacheImage() const{ return pPathPFCache; }
	/** Sets the prop field cache path. */
	void SetPathPFCache( const char *path );
	/** Determines if the prop field cache has changed. */
	inline bool GetPFCacheChanged() const{ return pPFCacheChanged; }
	/** Sets if the prop field cache has changed. */
	void SetPFCacheChanged( bool changed );
	/** Determines if the prop field cache has been saved. */
	inline bool GetPFCacheSaved() const{ return pPFCacheSaved; }
	/** Sets if the prop field cache has been saved. */
	void SetPFCacheSaved( bool saved );
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetNavSpaceCount() const;
	
	/** \brief Navigation space at index. */
	meHeightTerrainNavSpace *GetNavSpaceAt( int index ) const;
	
	/** \brief Named navigation space or \em NULL if absent. */
	meHeightTerrainNavSpace *GetNavSpaceNamed( const char *name ) const;
	
	/** \brief Named navigation space is present. */
	bool HasNavSpaceNamed( const char *name ) const;
	
	/** \brief Index of the navigation space or -1 if absent. */
	int IndexOfNavSpace( meHeightTerrainNavSpace *navspace ) const;
	
	/** \brief Index of named navigation space or -1 if absent. */
	int IndexOfNavSpaceNamed( const char *name ) const;
	
	/** \brief Navigation space is present. */
	bool HasNavSpace( meHeightTerrainNavSpace *navspace ) const;
	
	/** \brief Add navigation space. */
	void AddNavSpace( meHeightTerrainNavSpace *navspace );
	
	/** \brief Remove navigation space. */
	void RemoveNavSpace( meHeightTerrainNavSpace *navspace );
	
	/** \brief Remove all navigation spaces. */
	void RemoveAllNavSpaces();
	
	/** \brief Active navigation space or \em NULL. */
	inline meHeightTerrainNavSpace *GetActiveNavSpace() const{ return pActiveNavSpace; }
	
	/** \brief Set active navigation space or \em NULL. */
	void SetActiveNavSpace( meHeightTerrainNavSpace *navspace );
	
	/** \brief Selected navigation space points. */
	inline const decIntList &GetSelectedNavPoints() const{ return pSelectedNavPoints; }
	
	/** \brief Set selected navigation space points. */
	void SetSelectedNavPoints( const decIntList &points );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pLoadHeightImage();
	void pCreateHeightImage();
	
	void pCreateVisibility();
	void pSetPropFieldCount( int cellCount );
	void pUpdatePropFieldPositions();
	
	void pCreateDDEdges();
	void pFreeDDEdges();
	void pAddDDEdgesToWorld( deWorld &world );
	void pRemoveDDEdgesFromWorld( deWorld &world );
	void pUpdateDDEdgesColors();
	void pUpdateDDEdgesHeights( const decPoint &fromGrid, const decPoint &toGrid );
	
	void pUpdateNavSpaceDDColors();
	void pRepositionDebugDrawers();
	
	void pCreateDDSelNavPoints();
	void pUpdateDDSelNavPointsColors();
	void pUpdateDDSelNavPointsShapes();
	void pUpdateDDSelNavPointHeights();
};

#endif
