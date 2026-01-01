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

#ifndef _MEHEIGHTTERRAINNAVSPACE_H_
#define _MEHEIGHTTERRAINNAVSPACE_H_

#include "meHeightTerrainNavSpaceType.h"

#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class meHeightTerrainSector;

class deEngine;
class deHeightTerrainNavSpace;



/**
 * Height terrain navigation space.
 */
class meHeightTerrainNavSpace : public deObject{
private:
	deEngine &pEngine;
	deHeightTerrainNavSpace *pEngNavSpace;
	
	meHeightTerrainSector *pHTSector;
	
	decString pName;
	deNavigationSpace::eSpaceTypes pSpaceType;
	int pLayer;
	
	float pSnapDistance;
	float pSnapAngle;
	
	meHeightTerrainNavSpaceType::List pTypes;
	meHeightTerrainNavSpaceType::Ref pActiveType;
	
	decString pPathNavSpace;
	bool pNavSpaceChanged;
	bool pNavSpaceSaved;
	
	bool pActive;
	
	deDebugDrawer::Ref pDDTypeFaces;
	bool pBulkUpdate;
	
	
	
public:
	typedef deTObjectReference<meHeightTerrainNavSpace> Ref;
	typedef decTObjectOrderedSet<meHeightTerrainNavSpace> List;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates height terrain navigation space. */
	meHeightTerrainNavSpace(deEngine &engine, const char *name = "");
	
protected:
	/** Clean up height terrain navigation space. */
	~meHeightTerrainNavSpace() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Game engine. */
	inline deEngine &GetEngine() const{ return pEngine; }
	
	/** Engine navigation space or \em nullptr. */
	inline deHeightTerrainNavSpace *GetEngineNavSpace() const{ return pEngNavSpace; }
	
	/** Set engine navigation space or \em nullptr. */
	void SetEngineNavSpace(deHeightTerrainNavSpace *navspace);
	
	/** Create engine resource. */
	deHeightTerrainNavSpace *CreateEngineNavSpace() const;
	
	
	
	/** Parent height terrain sector or \em nullptr. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** Set parent height terrain sector or \em nullptr. */
	void SetHTSector(meHeightTerrainSector *sector);
	
	
	
	/** Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set display name. */
	void SetName(const char *name);
	
	
	
	/** Navigation space type. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	
	/** Set navigation space type. */
	void SetSpaceType(deNavigationSpace::eSpaceTypes type);
	
	/** Navigation layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** Set navigation layer number. */
	void SetLayer(int layer);
	
	
	
	/**
	 * Snap distance in meters up to which edges are linked to neighbor spaces.
	 * \details The default snap distance is 0.001 (1mm).
	 */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/**
	 * Set snap distance in meters up to which edges are linked to neighbor spaces.
	 * \details The default snap distance is 0.001 (1mm).
	 */
	void SetSnapDistance(float distance);
	
	/**
	 * Snap angle in radians up to which edges are linked to neighbor spaces.
	 * \details The default snap angle is 180° in radians.
	 */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/**
	 * Set snap angle in radians up to which edges are linked to neighbor spaces.
	 * \details The default snap angle is 180° in radians.
	 */
	void SetSnapAngle(float angle);
	
	
	
	/** Notify listeners navigation space changed. */
	void NotifyChanged();
	
	/** Invalidate path test. */
	void InvalidatePathTest();
	

	/** Height terrain size changed. */
	void HeightTerrainSizeChanged();
	
	/** Height terrain image dimension changed. */
	void SectorSizeOrResChanged();
	
	/** Height terrain height parameters changed. */
	void HeightTerrainHeightParametersChanged();
	
	/** Add to game world. */
	void AddDDToWorld(deWorld &world);
	
	/** Remove from game world. */
	void RemoveDDFromWorld(deWorld &world);
	
	/** Height image of sector changed. */
	void InvalidateHeights();
	
	/** Update debug drawer colors. */
	void UpdateDDColors();
	
	
	
	/** Navigation space is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set if navigation space is active. */
	void SetActive(bool active);
	/*@}*/
	
	
	
	/** \name Types */
	/*@{*/
	/** Types. */
	inline const meHeightTerrainNavSpaceType::List &GetTypes() const{ return pTypes; }
	
	/** Named type or \em nullptr if absent. */
	meHeightTerrainNavSpaceType *GetTypeNamed(const char *name) const;
	
	/** Named type is present. */
	bool HasTypeNamed(const char *name) const;
	
	/** Index of named type or -1 if absent. */
	int IndexOfTypeNamed(const char *name) const;
	
	/** Add type. */
	void AddType(meHeightTerrainNavSpaceType *type);
	
	/** Remove type. */
	void RemoveType(meHeightTerrainNavSpaceType *type);
	
	/** Remove all types. */
	void RemoveAllTypes();
	
	/** Active type or \em nullptr. */
	inline const meHeightTerrainNavSpaceType::Ref &GetActiveType() const{ return pActiveType; }
	
	/** Set active type or \em nullptr. */
	void SetActiveType(meHeightTerrainNavSpaceType *type);
	
	/** Type faces debug drawer for use by types only. */
	inline const deDebugDrawer::Ref &GetDDTypeFaces() const{ return pDDTypeFaces; }
	
	/** Notify listeners type count changed. */
	void NotifyTypeCountChanged();
	/*@}*/
	
	
	
	/** \name Nav-space file */
	/*@{*/
	/** Nav-space file path. */
	inline const decString &GetPathNavSpace() const{ return pPathNavSpace; }
	
	/** Set nav-space file path. */
	void SetPathNavSpace(const char *path, bool load);
	
	/** Nav-space file changed. */
	inline bool GetNavSpaceChanged() const{ return pNavSpaceChanged; }
	
	/** Set if nav-space file changed. */
	void SetNavSpaceChanged(bool changed);
	
	/** Nav-space file has been saved. */
	inline bool GetNavSpaceSaved() const{ return pNavSpaceSaved; }
	
	/** Set if nav-space file has been saved. */
	void SetNavSpaceSaved(bool saved);
	
	/** Update engine nav-space corners and faces. */
	void UpdateNavSpaceFaces();
	
	/** Load nav-space from file. */
	void LoadNavSpaceFromFile();
	
	/** Bulk update for internal use only. */
	inline bool GetBulkUpdate() const{ return pBulkUpdate; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pRepositionDD();
	void pUpdateDDTypeFaces();
	void pUpdateDDHeights();
};

#endif
