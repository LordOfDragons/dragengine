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

#ifndef _MEHEIGHTTERRAINNAVSPACE_H_
#define _MEHEIGHTTERRAINNAVSPACE_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>

class meHeightTerrainNavSpaceType;
class meHeightTerrainSector;

class deEngine;
class deDebugDrawer;
class deHeightTerrainNavSpace;



/**
 * \brief Height terrain navigation space.
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
	
	decObjectOrderedSet pTypes;
	meHeightTerrainNavSpaceType *pActiveType;
	
	decString pPathNavSpace;
	bool pNavSpaceChanged;
	bool pNavSpaceSaved;
	
	bool pActive;
	
	deDebugDrawer *pDDTypeFaces;
	bool pBulkUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates height terrain navigation space. */
	meHeightTerrainNavSpace( deEngine &engine, const char *name = "" );
	
	/** \brief Clean up height terrain navigation space. */
	virtual ~meHeightTerrainNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine &GetEngine() const{ return pEngine; }
	
	/** \brief Engine navigation space or \em NULL. */
	inline deHeightTerrainNavSpace *GetEngineNavSpace() const{ return pEngNavSpace; }
	
	/** \brief Set engine navigation space or \em NULL. */
	void SetEngineNavSpace( deHeightTerrainNavSpace *navspace );
	
	/** \brief Create engine resource. */
	deHeightTerrainNavSpace *CreateEngineNavSpace() const;
	
	
	
	/** \brief Parent height terrain sector or \em NULL. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** \brief Set parent height terrain sector or \em NULL. */
	void SetHTSector( meHeightTerrainSector *sector );
	
	
	
	/** \brief Display name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set display name. */
	void SetName( const char *name );
	
	
	
	/** \brief Navigation space type. */
	inline deNavigationSpace::eSpaceTypes GetSpaceType() const{ return pSpaceType; }
	
	/** \brief Set navigation space type. */
	void SetSpaceType( deNavigationSpace::eSpaceTypes type );
	
	/** \brief Navigation layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set navigation layer number. */
	void SetLayer( int layer );
	
	
	
	/**
	 * \brief Snap distance in meters up to which edges are linked to neighbor spaces.
	 * \details The default snap distance is 0.001 (1mm).
	 */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/**
	 * \brief Set snap distance in meters up to which edges are linked to neighbor spaces.
	 * \details The default snap distance is 0.001 (1mm).
	 */
	void SetSnapDistance( float distance );
	
	/**
	 * \brief Snap angle in radians up to which edges are linked to neighbor spaces.
	 * \details The default snap angle is 180° in radians.
	 */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/**
	 * \brief Set snap angle in radians up to which edges are linked to neighbor spaces.
	 * \details The default snap angle is 180° in radians.
	 */
	void SetSnapAngle( float angle );
	
	
	
	/** \brief Notify listeners navigation space changed. */
	void NotifyChanged();
	
	/** \brief Invalidate path test. */
	void InvalidatePathTest();
	

	/** \brief Height terrain size changed. */
	void HeightTerrainSizeChanged();
	
	/** \brief Height terrain image dimension changed. */
	void SectorSizeOrResChanged();
	
	/** \brief Height terrain height parameters changed. */
	void HeightTerrainHeightParametersChanged();
	
	/** \brief Add to game world. */
	void AddDDToWorld( deWorld &world );
	
	/** \brief Remove from game world. */
	void RemoveDDFromWorld( deWorld &world );
	
	/** \brief Height image of sector changed. */
	void InvalidateHeights();
	
	/** \brief Update debug drawer colors. */
	void UpdateDDColors();
	
	
	
	/** \brief Navigation space is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if navigation space is active. */
	void SetActive( bool active );
	/*@}*/
	
	
	
	/** \name Types */
	/*@{*/
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type at index. */
	meHeightTerrainNavSpaceType *GetTypeAt( int index ) const;
	
	/** \brief Named type or \em NULL if absent. */
	meHeightTerrainNavSpaceType *GetTypeNamed( const char *name ) const;
	
	/** \brief Named type is present. */
	bool HasTypeNamed( const char *name ) const;
	
	/** \brief Index of the type or -1 if absent. */
	int IndexOfType( meHeightTerrainNavSpaceType *type ) const;
	
	/** \brief Index of named type or -1 if absent. */
	int IndexOfTypeNamed( const char *name ) const;
	
	/** \brief Type is present. */
	bool HasType( meHeightTerrainNavSpaceType *type ) const;
	
	/** \brief Add type. */
	void AddType( meHeightTerrainNavSpaceType *type );
	
	/** \brief Remove type. */
	void RemoveType( meHeightTerrainNavSpaceType *type );
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	
	/** \brief Active type or \em NULL. */
	inline meHeightTerrainNavSpaceType *GetActiveType() const{ return pActiveType; }
	
	/** \brief Set active type or \em NULL. */
	void SetActiveType( meHeightTerrainNavSpaceType *type );
	
	/** \brief Type faces debug drawer for use by types only. */
	inline deDebugDrawer *GetDDTypeFaces() const{ return pDDTypeFaces; }
	
	/** \brief Notify listeners type count changed. */
	void NotifyTypeCountChanged();
	/*@}*/
	
	
	
	/** \name Nav-space file */
	/*@{*/
	/** \brief Nav-space file path. */
	inline const decString &GetPathNavSpace() const{ return pPathNavSpace; }
	
	/** \brief Set nav-space file path. */
	void SetPathNavSpace( const char *path, bool load );
	
	/** \brief Nav-space file changed. */
	inline bool GetNavSpaceChanged() const{ return pNavSpaceChanged; }
	
	/** \brief Set if nav-space file changed. */
	void SetNavSpaceChanged( bool changed );
	
	/** \brief Nav-space file has been saved. */
	inline bool GetNavSpaceSaved() const{ return pNavSpaceSaved; }
	
	/** \brief Set if nav-space file has been saved. */
	void SetNavSpaceSaved( bool saved );
	
	/** \brief Update engine nav-space corners and faces. */
	void UpdateNavSpaceFaces();
	
	/** \brief Load nav-space from file. */
	void LoadNavSpaceFromFile();
	
	/** \brief Bulk update for internal use only. */
	inline bool GetBulkUpdate() const{ return pBulkUpdate; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pRepositionDD();
	void pUpdateDDTypeFaces();
	void pUpdateDDHeights();
};

#endif
