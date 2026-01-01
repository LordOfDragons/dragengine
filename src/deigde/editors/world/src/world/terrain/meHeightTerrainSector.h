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

#ifndef _MEHEIGHTTERRAINSECTOR_H_
#define _MEHEIGHTTERRAINSECTOR_H_

#include "meHeightTerrainTexture.h"
#include "meHeightTerrainPropField.h"
#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainNavSpace.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class meBitArray;
class meHeightTerrain;
class meHTVegetationLayer;

class deDebugDrawerShape;
class deEngine;
class deHeightTerrainSector;
class dePropField;
class deWorld;

class igdeEnvironment;
class igdeWDebugDrawerShape;



/**
 * Height Terrain Sector.
 *
 * Defines the height terrain related information inside a world sector.
 */
class meHeightTerrainSector : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainSector> Ref;
	typedef decTObjectOrderedSet<meHeightTerrainSector> List;
	typedef decTObjectOrderedSet<deDebugDrawer> DDEdgeList;
	
	
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
	/** Debug drawer cluster for performance optimization. */
	class cDDCluster : public deObject{
	public:
		typedef deTObjectReference<cDDCluster> Ref;
		typedef decTObjectOrderedSet<cDDCluster> List;
		
		decPoint pointFrom;
		decPoint pointTo;
		decPoint faceCount;
		decVector boxFrom;
		decVector boxTo;
		
		cDDCluster() = default;
	};
	
	
	
private:
	meHeightTerrain *pHeightTerrain;
	
	deEngine *pEngine;
	deHeightTerrainSector *pEngSector;
	
	int pDDPointsPerCluster;
	cDDCluster::List pDDClusters;
	DDEdgeList pDDEdges;
	
	decPoint pCoordinates;
	
	bool pActive;
	
	float pRadiusOfInfluence;
	
	// height image
	deImage::Ref pHeightImage;
	eDataTypes pDataType;
	
	decString pPathHeightImage;
	bool pHeightImageChanged;
	bool pHeightImageSaved;
	
	// visibility image
	meBitArray *pVisFaces;
	
	decString pPathVisImage;
	bool pVisImageChanged;
	bool pVisImageSaved;
	
	// textures
	meHeightTerrainTexture::List pTextures;
	meHeightTerrainTexture::Ref pActiveTexture;
	
	// prop fields
	meHeightTerrainPropField::List pPropFields;
	int pPropFieldCellCount;
	
	meHeightTerrainPFLayer::List pPFLayers;
	meHeightTerrainPFLayer::Ref pActivePFLayer;
	
	decString pPathPFCache;
	bool pPFCacheChanged;
	bool pPFCacheSaved;
	
	// navigation spaces
	meHeightTerrainNavSpace::List pNavSpaces;
	meHeightTerrainNavSpace::Ref pActiveNavSpace;
	decTList<int> pSelectedNavPoints;
	
	deDebugDrawer::Ref pDDSelNavPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new height terrain sector. */
	meHeightTerrainSector(deEngine *engine, const decPoint &coordinates);
	
protected:
	/** Cleans up the height terrain sector. */
	~meHeightTerrainSector() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Rebuilds the engine sector. */
	void RebuildEngineSector();
	
	/** Retrieves the parent height terrain. */
	inline meHeightTerrain *GetHeightTerrain() const{ return pHeightTerrain; }
	/** Sets the parent height terrain. */
	void SetHeightTerrain(meHeightTerrain *heightTerrain);
	
	/** Engine height terrain sector. */
	inline deHeightTerrainSector *GetEngineSector() const{ return pEngSector; }
	
	/** Create engine height terrain sector if absent. */
	void CreateEngineSector();
	
	/** Destroy engine height terrain sector if present. */
	void DestroyEngineSector();
	
	
	
	/** Points per debug drawer cluster. */
	inline int GetDDPointsPerCluster() const{ return pDDPointsPerCluster; }
	
	/** Debug drqwer clusters. */
	inline const cDDCluster::List &GetDDClusters() const{ return pDDClusters; }
	
	
	
	/** Retrieves the coordinates. */
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	/** Sets the coordinates. */
	void SetCoordinates(const decPoint &coordinates);
	
	/** Height terrain sector size or resolution changed. */
	void SectorSizeOrResChanged();
	
	/** Notifies the engine object that the sector changed. */
	void NotifySectorChanged();
	
	/** Sector is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set if sector is active. */
	void SetActive(bool active);
	
	/** Init the delegates. */
	void InitDelegates(igdeEnvironment *environment);
	
	/** Get coordinates in grid points (0 to image-dimension-1) for point. */
	decVector2 GetGridPointAt(const decDVector &position) const;
	
	/** Height at grid point. */
	float GetHeightAtPoint(int x, int z) const;
	
	/** Retrieves the height image relative coordinates. */
	void GetHeightLocalAt(float x, float z, int &imgX, int &imgZ, float &imgFacX, float &imgFacZ) const;
	/** Determines the height at a given position. */
	bool GetHeightAt(float x, float z, float &height) const;
	/** Retrieves height terrain parameters at a given position. */
	bool GetHeightNormalAt(float x, float z, float &height, decVector &normal) const;
	
	/** World position of sector center. */
	decDVector GetWorldPosition() const;
	
	/** Updates the height terrain sector. */
	void Update();
	/** Updates the prop field vegetation instances. */
	void UpdateVInstances();
	/** Clear vegetation. */
	void ClearVegetation();
	
	/** Rebuild height terrain engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	
	/** Retrieves the radius of influence due to vegetation rules. */
	inline float GetRadiusOfInfluence() const{ return pRadiusOfInfluence; }
	/** Sets the radius of influence due to vegetation rules. */
	void SetRadiusOfInfluence(float radius);
	
	/** Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
	/** \name Height Image */
	/*@{*/
	/** Retrieves the height image or null. */
	inline const deImage::Ref &GetHeightImage() const{ return pHeightImage; }
	
	/** Retrieves the data type. */
	inline eDataTypes GetDataType() const{ return pDataType; }
	/** Sets the data type. */
	void SetDataType(eDataTypes dataType);
	/** Determines if the height values have to be limited to the range from -0.5 to 0.5 inclusive. */
	bool DoClampValues() const;
	
	/** Retrieves the height image path. */
	inline const decString &GetPathHeightImage() const{ return pPathHeightImage; }
	/** Sets the height image path. */
	void SetPathHeightImage(const char *path, bool loadFile);
	/** Determines if the height image has changed. */
	inline bool GetHeightImageChanged() const{ return pHeightImageChanged; }
	/** Sets if the height image has changed. */
	void SetHeightImageChanged(bool changed);
	/** Determines if the height image has been saved. */
	inline bool GetHeightImageSaved() const{ return pHeightImageSaved; }
	/** Sets if the height image has been saved. */
	void SetHeightImageSaved(bool saved);
	/*@}*/
	
	/** \name Visibility */
	/*@{*/
	/** Retrieves the bit array with the faces visibility. */
	inline meBitArray *GetVisibilityFaces() const{ return pVisFaces; }
	
	/** Retrieves the visibility image path. */
	inline const decString &GetPathVisibilityImage() const{ return pPathVisImage; }
	/** Sets the visibility image path. */
	void SetPathVisibilityImage(const char *path);
	/** Determines if the visibility has changed. */
	inline bool GetVisibilityChanged() const{ return pVisImageChanged; }
	/** Sets if the visibility has changed. */
	void SetVisibilityChanged(bool changed);
	/** Determines if the visibility has been saved. */
	inline bool GetVisibilitySaved() const{ return pVisImageSaved; }
	/** Sets if the visibility has been saved. */
	void SetVisibilitySaved(bool saved);
	
	/** Inits the values from the given height map. */
	void InitVisibilityFromSector(deHeightTerrainSector *sector) const;
	/** Sets the value in the given height map using the stored values. */
	void UpdateVisibilitySector(deHeightTerrainSector *sector) const;
	
	/** Loads the visibility from the stored path. */
	void LoadVisibilityFromImage();
	/*@}*/
	
	/** \name Texture */
	/*@{*/
	/** Textures. */
	inline const meHeightTerrainTexture::List &GetTextures() const{ return pTextures; }
	
	/** Retrieves the texture with the given name of nullptr if not found. */
	meHeightTerrainTexture *GetTextureNamed(const char *name) const;
	
	/** Determines if a texture with the given name exists. */
	bool HasTextureNamed(const char *name) const;
	
	/** Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed(const char *name) const;
	
	/** Adds a texture. */
	void AddTexture(meHeightTerrainTexture *texture);
	
	/** Removes a texture. */
	void RemoveTexture(meHeightTerrainTexture *texture);
	
	/** Removes all textures. */
	void RemoveAllTextures();
	
	/** Retrieves the active texture or nullptr. */
	inline const meHeightTerrainTexture::Ref &GetActiveTexture() const{ return pActiveTexture; }
	
	/** Sets the active texture or nullptr. */
	void SetActiveTexture(meHeightTerrainTexture *texture);
	
	/** Retrieves the texture under the given point. */
	meHeightTerrainTexture *GetTextureUnder(float x, float y) const;
	/*@}*/
	
	
	
	/** \name Prop Fields */
	/*@{*/
	/** Prop fields. */
	inline const meHeightTerrainPFLayer::List &GetPFLayers() const{ return pPFLayers; }
	
	/** Adds a prop field. */
	void AddPFLayer(meHeightTerrainPFLayer *pflayer);
	
	/** Removes a prop field. */
	void RemovePFLayer(meHeightTerrainPFLayer *pflayer);
	
	/** Removes all prop fields. */
	void RemoveAllPFLayers();
	
	/** Retrieves the active prop field or nullptr. */
	inline const meHeightTerrainPFLayer::Ref &GetActivePFLayer() const{ return pActivePFLayer; }
	
	/** Sets the active prop field or nullptr. */
	void SetActivePFLayer(meHeightTerrainPFLayer *pflayer);
	
	
	/** Prop fields. */
	inline const meHeightTerrainPropField::List &GetPropFields() const{ return pPropFields; }
	
	/** Retrieves the number of cells prop fields. */
	inline int GetPropFieldCellCount() const{ return pPropFieldCellCount; }
	
	/** Height image of sector changed. */
	void InvalidateHeights(const decPoint &fromGrid, const decPoint &toGrid);
	
	/** Invalidate prop fields in the given area. */
	void InvalidatePropFields(const decPoint &fromGrid, const decPoint &toGrid);
	
	/** Invalidate all prop. */
	void InvalidateAllPropFields();
	
	/** Validate all prop fields. */
	void ValidateAllPropFields();
	
	/** Retrieves the prop field cache path. */
	inline const decString &GetPathPFCacheImage() const{ return pPathPFCache; }
	/** Sets the prop field cache path. */
	void SetPathPFCache(const char *path);
	/** Determines if the prop field cache has changed. */
	inline bool GetPFCacheChanged() const{ return pPFCacheChanged; }
	/** Sets if the prop field cache has changed. */
	void SetPFCacheChanged(bool changed);
	/** Determines if the prop field cache has been saved. */
	inline bool GetPFCacheSaved() const{ return pPFCacheSaved; }
	/** Sets if the prop field cache has been saved. */
	void SetPFCacheSaved(bool saved);
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** Navigation spaces. */
	inline const meHeightTerrainNavSpace::List &GetNavSpaces() const{ return pNavSpaces; }
	
	/** Named navigation space or \em nullptr if absent. */
	meHeightTerrainNavSpace *GetNavSpaceNamed(const char *name) const;
	
	/** Named navigation space is present. */
	bool HasNavSpaceNamed(const char *name) const;
	
	/** Index of named navigation space or -1 if absent. */
	int IndexOfNavSpaceNamed(const char *name) const;
	
	/** Add navigation space. */
	void AddNavSpace(meHeightTerrainNavSpace *navspace);
	
	/** Remove navigation space. */
	void RemoveNavSpace(meHeightTerrainNavSpace *navspace);
	
	/** Remove all navigation spaces. */
	void RemoveAllNavSpaces();
	
	/** Active navigation space or \em nullptr. */
	inline const meHeightTerrainNavSpace::Ref &GetActiveNavSpace() const{ return pActiveNavSpace; }
	
	/** Set active navigation space or \em nullptr. */
	void SetActiveNavSpace(meHeightTerrainNavSpace *navspace);
	
	/** Selected navigation space points. */
	inline const decTList<int> &GetSelectedNavPoints() const{ return pSelectedNavPoints; }
	
	/** Set selected navigation space points. */
	void SetSelectedNavPoints(const decTList<int> &points);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pLoadHeightImage();
	void pCreateHeightImage();
	
	void pCreateVisibility();
	void pSetPropFieldCount(int cellCount);
	void pUpdatePropFieldPositions();
	
	void pCreateDDEdges();
	void pFreeDDEdges();
	void pAddDDEdgesToWorld(deWorld &world);
	void pRemoveDDEdgesFromWorld(deWorld &world);
	void pUpdateDDEdgesColors();
	void pUpdateDDEdgesHeights(const decPoint &fromGrid, const decPoint &toGrid);
	
	void pUpdateNavSpaceDDColors();
	void pRepositionDebugDrawers();
	
	void pCreateDDSelNavPoints();
	void pUpdateDDSelNavPointsColors();
	void pUpdateDDSelNavPointsShapes();
	void pUpdateDDSelNavPointHeights();
};

#endif
