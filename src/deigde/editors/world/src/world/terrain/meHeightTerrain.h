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

#ifndef _MEHEIGHTTERRAIN_H_
#define _MEHEIGHTTERRAIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class meHeightTerrainSector;
class meHTVegetationLayer;
class meObject;
class meWorld;

class deDebugDrawer;
class deHeightTerrain;

class igdeEnvironment;



/**
 * \brief Height terrain.
 */
class meHeightTerrain : public deObject{
private:
	meWorld &pWorld;
	deHeightTerrain *pEngHT;
	
	decString pPathHT;
	bool pChanged;
	bool pDepChanged;
	bool pSaved;
	
	float pSectorSize;
	int pSectorResolution;
	float pBaseHeight;
	float pHeightScaling;
	
	decObjectOrderedSet pSectors;
	meHeightTerrainSector *pActiveSector;
	
	decObjectOrderedSet pVLayers;
	meHTVegetationLayer *pActiveVLayer;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHeightTerrain> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrain(meWorld &world);
	
	
	
protected:
	/** \brief Clean up object. */
	virtual ~meHeightTerrain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Engine height terrain. */
	inline deHeightTerrain *GetEngineHeightTerrain() const{ return pEngHT; }
	
	
	
	/** \brief Height terrain file path. */
	inline const decString &GetPathHT() const{ return pPathHT; }
	
	/** \brief Set height terrain file path. */
	void SetPathHT(const char *path);
	
	/** \brief Base directory. */
	decString GetBaseDirectory() const;
	
	/** \brief World dependency has changed. */
	inline bool GetDepChanged() const{ return pDepChanged; }
	
	/** \brief Set if world dependency has changed. */
	void SetDepChanged(bool changed);
	
	/** \brief World has changed. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** \brief Set if world has changed. */
	void SetChanged(bool changed);
	
	/** \brief World has saved. */
	inline bool GetSaved() const{ return pSaved; }
	
	/** \brief Set if world has saved. */
	void SetSaved(bool saved);
	
	
	
	/** \brief Sector size. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	/** \brief Set sector size. */
	void SetSectorSize(float size);
	
	/** \brief Sector resolution. */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** \brief Set sector resolution. */
	void SetSectorResolution(int resolution);
	
	/** \brief Base height. */
	inline float GetBaseHeight() const{ return pBaseHeight; }
	
	/** \brief Set base height. */
	void SetBaseHeight(float height);
	
	/** \brief Height scaling. */
	inline float GetHeightScaling() const{ return pHeightScaling; }
	
	/** \brief Set height scaling. */
	void SetHeightScaling(float scaling);
	
	
	
	/** \brief Set world changed. */
	void SetWorldChanged();
	
	
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	
	
	/** \brief Notify engine height values changed. */
	void NotifyHeightsChanged(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** \brief Notify engine height values changed. */
	void NotifyHeightsChanged(const decBoundary &areaSector, const decBoundary &areaGrid);
	
	/** \brief Invalidate heights. */
	void InvalidateHeights(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** \brief Invalidate all heights. */
	void InvalidateAllHeights();
	
	
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields(const decBoundary &areaSector, const decBoundary &areaGrid);
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields(meHeightTerrainSector *sector);
	
	/** \brief Invalidate prop fields. */
	void InvalidatePropFields(meObject *object);
	
	/** \brief Invalidate all prop fields. */
	void InvalidateAllPropFields();
	
	
	
	/** \brief Init delegates. */
	void InitDelegates(igdeEnvironment *environment);
	
	
	
	/** \brief Update height terrain. */
	void Update();
	
	/** \brief Force update of vegetation. */
	void ForceUpdateVegetation(bool fullUpdate);
	
	/** \brief Clear vegetation. */
	void ClearVegetation();
	
	/** \brief Rebuild height terrain engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** \brief Number of sectors. */
	int GetSectorCount() const;
	
	/** \brief Sector at index. */
	meHeightTerrainSector *GetSectorAt(int index) const;
	
	/** \brief Sector at coordinates or \em NULL if absent. */
	meHeightTerrainSector *GetSectorWith(const decPoint &coordinates) const;
	
	/** \brief Index of sector or -1 if absent. */
	int IndexOfSector(meHeightTerrainSector *sector) const;
	
	/** \brief Add sector. */
	void AddSector(meHeightTerrainSector *sector);
	
	/** \brief Remove sector. */
	void RemoveSector(meHeightTerrainSector *sector);
	
	/** \brief Remove all sectors. */
	void RemoveAllSectors();
	
	/** \brief Active sector. */
	inline meHeightTerrainSector *GetActiveSector() const{ return pActiveSector; }
	
	/** \brief Set active sector. */
	void SetActiveSector(meHeightTerrainSector *sector);
	/*@}*/
	
	
	
	/** \name Vegetation */
	/*@{*/
	/** \brief Count of vegetation layers. */
	int GetVLayerCount() const;
	
	/** \brief Vegetation layer at index. */
	meHTVegetationLayer *GetVLayerAt(int index) const;
	
	/** \brief Index of the vegetation layer or -1 if absent. */
	int IndexOfVLayer(meHTVegetationLayer *vlayer) const;
	
	/** \brief Vegetation layer is present. */
	bool HasVLayer(meHTVegetationLayer *vlayer) const;
	
	/** \brief Add vegetation layer. */
	void AddVLayer(meHTVegetationLayer *vlayer);
	
	/** \brief Inserts vegetation layer. */
	void InsertVLayer(int before, meHTVegetationLayer *vlayer);
	
	/** \brief Move vegetation layer to new position. */
	void MoveVLayer(meHTVegetationLayer *vlayer, int moveTo);
	
	/** \brief Remove vegetation layer. */
	void RemoveVLayer(meHTVegetationLayer *vlayer);
	
	/** \brief Remove all vegetation layers. */
	void RemoveAllVLayers();
	
	/** \brief Active vegetation layer or NULL. */
	inline meHTVegetationLayer *GetActiveVLayer() const{ return pActiveVLayer; }
	
	/** \brief Set active vegetation layer or NULL. */
	void SetActiveVLayer(meHTVegetationLayer *vlayer);
	
	/** \brief Notify world vegetation layer changed. */
	void NotifyVLayerChanged(meHTVegetationLayer *vlayer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateHeightTerrain();
};

#endif
