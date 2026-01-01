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

#include "meHeightTerrainSector.h"
#include "../heightterrain/meHTVegetationLayer.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>

class meObject;
class meWorld;

class deDebugDrawer;

class igdeEnvironment;



/**
 * Height terrain.
 */
class meHeightTerrain : public deObject{
private:
	meWorld &pWorld;
	deHeightTerrain::Ref pEngHT;
	
	decString pPathHT;
	bool pChanged;
	bool pDepChanged;
	bool pSaved;
	
	float pSectorSize;
	int pSectorResolution;
	float pBaseHeight;
	float pHeightScaling;
	
	meHeightTerrainSector::List pSectors;
	meHeightTerrainSector::Ref pActiveSector;
	
	meHTVegetationLayer::List pVLayers;
	meHTVegetationLayer::Ref pActiveVLayer;
	
	
	
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<meHeightTerrain> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create object. */
	explicit meHeightTerrain(meWorld &world);
	
	
	
protected:
	/** Clean up object. */
	~meHeightTerrain() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** Engine height terrain. */
	inline const deHeightTerrain::Ref &GetEngineHeightTerrain() const{ return pEngHT; }
	
	
	
	/** Height terrain file path. */
	inline const decString &GetPathHT() const{ return pPathHT; }
	
	/** Set height terrain file path. */
	void SetPathHT(const char *path);
	
	/** Base directory. */
	decString GetBaseDirectory() const;
	
	/** World dependency has changed. */
	inline bool GetDepChanged() const{ return pDepChanged; }
	
	/** Set if world dependency has changed. */
	void SetDepChanged(bool changed);
	
	/** World has changed. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** Set if world has changed. */
	void SetChanged(bool changed);
	
	/** World has saved. */
	inline bool GetSaved() const{ return pSaved; }
	
	/** Set if world has saved. */
	void SetSaved(bool saved);
	
	
	
	/** Sector size. */
	inline float GetSectorSize() const{ return pSectorSize; }
	
	/** Set sector size. */
	void SetSectorSize(float size);
	
	/** Sector resolution. */
	inline int GetSectorResolution() const{ return pSectorResolution; }
	
	/** Set sector resolution. */
	void SetSectorResolution(int resolution);
	
	/** Base height. */
	inline float GetBaseHeight() const{ return pBaseHeight; }
	
	/** Set base height. */
	void SetBaseHeight(float height);
	
	/** Height scaling. */
	inline float GetHeightScaling() const{ return pHeightScaling; }
	
	/** Set height scaling. */
	void SetHeightScaling(float scaling);
	
	
	
	/** Set world changed. */
	void SetWorldChanged();
	
	
	
	/** Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	
	
	/** Notify engine height values changed. */
	void NotifyHeightsChanged(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** Notify engine height values changed. */
	void NotifyHeightsChanged(const decBoundary &areaSector, const decBoundary &areaGrid);
	
	/** Invalidate heights. */
	void InvalidateHeights(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** Invalidate all heights. */
	void InvalidateAllHeights();
	
	
	
	/** Invalidate prop fields. */
	void InvalidatePropFields(const decPoint &fromSector, const decPoint &fromGrid,
		const decPoint &toSector, const decPoint &toGrid);
	
	/** Invalidate prop fields. */
	void InvalidatePropFields(const decBoundary &areaSector, const decBoundary &areaGrid);
	
	/** Invalidate prop fields. */
	void InvalidatePropFields(meHeightTerrainSector *sector);
	
	/** Invalidate prop fields. */
	void InvalidatePropFields(meObject *object);
	
	/** Invalidate all prop fields. */
	void InvalidateAllPropFields();
	
	
	
	/** Init delegates. */
	void InitDelegates(igdeEnvironment *environment);
	
	
	
	/** Update height terrain. */
	void Update();
	
	/** Force update of vegetation. */
	void ForceUpdateVegetation(bool fullUpdate);
	
	/** Clear vegetation. */
	void ClearVegetation();
	
	/** Rebuild height terrain engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	/*@}*/
	
	
	
	/** \name Sectors */
	/*@{*/
	/** Sectors. */
	inline const meHeightTerrainSector::List &GetSectors() const{ return pSectors; }
	
	/** Sector at coordinates or \em nullptr if absent. */
	meHeightTerrainSector *GetSectorWith(const decPoint &coordinates) const;
	
	/** Add sector. */
	void AddSector(meHeightTerrainSector *sector);
	
	/** Remove sector. */
	void RemoveSector(meHeightTerrainSector *sector);
	
	/** Remove all sectors. */
	void RemoveAllSectors();
	
	/** Active sector. */
	inline const meHeightTerrainSector::Ref &GetActiveSector() const{ return pActiveSector; }
	
	/** Set active sector. */
	void SetActiveSector(meHeightTerrainSector *sector);
	/*@}*/
	
	
	
	/** \name Vegetation */
	/*@{*/
	/** Vegetation layers. */
	inline const meHTVegetationLayer::List &GetVLayers() const{ return pVLayers; }
	
	/** Add vegetation layer. */
	void AddVLayer(meHTVegetationLayer *vlayer);
	
	/** Inserts vegetation layer. */
	void InsertVLayer(int before, meHTVegetationLayer *vlayer);
	
	/** Move vegetation layer to new position. */
	void MoveVLayer(meHTVegetationLayer *vlayer, int moveTo);
	
	/** Remove vegetation layer. */
	void RemoveVLayer(meHTVegetationLayer *vlayer);
	
	/** Remove all vegetation layers. */
	void RemoveAllVLayers();
	
	/** Active vegetation layer or nullptr. */
	inline meHTVegetationLayer *GetActiveVLayer() const{ return pActiveVLayer; }
	
	/** Set active vegetation layer or nullptr. */
	void SetActiveVLayer(meHTVegetationLayer *vlayer);
	
	/** Notify world vegetation layer changed. */
	void NotifyVLayerChanged(meHTVegetationLayer *vlayer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateHeightTerrain();
};

#endif
