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

#ifndef _MEHEIGHTTERRAINPROPFIELD_H_
#define _MEHEIGHTTERRAINPROPFIELD_H_

#include <dragengine/common/math/decMath.h>

class meHeightTerrainSector;
class meUpdateHeightTerrainPropField;
class meHTVInstance;

class deEngine;
class dePropField;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * \brief Height terrain prop field.
 */
class meHeightTerrainPropField{
private:
	meHeightTerrainSector *pHTSector;
	
	decVector2 pMinExtend;
	decVector2 pMaxExtend;
	
	deEngine *pEngine;
	dePropField *pEngPF;
	
	meHTVInstance *pVInstances;
	int pVInstanceCount;
	int pVInstanceSize;
	bool pDirtyVInstances;
	
	bool pDirty;
	bool pKeepClean;
	
	meUpdateHeightTerrainPropField *pListener;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrainPropField( deEngine *engine );
	
	/** \brief Clean up object. */
	virtual ~meHeightTerrainPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain sector or \em NULL. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** \brief Set parent height terrain sector or \em NULL. */
	void SetHTSector( meHeightTerrainSector *htsector );
	
	
	
	/** \brief Minimum extend. */
	inline const decVector2 &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decVector2 &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** \brief Set extends. */
	void SetExtend( const decVector2 &minimum, const decVector2 &maximum );
	
	
	
	/** \brief Engine prop field. */
	inline dePropField *GetEnginePropField() const{ return pEngPF; }
	
	
	
	/** \brief Init delegates. */
	void InitDelegates( igdeEnvironment *environment );
	
	/** \brief Invalidate prop field. */
	void Invalidate();
	
	/** \brief Validate prop field. */
	void Validate();
	
	
	
	/** \brief Update prop field. */
	void Update();
	
	/** \brief Clear prop field. */
	void Clear();
	
	
	
	/** \brief Rebuild engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	
	
	
	/** \brief Prop field has to be kept clean. */
	inline bool GetKeepClean() const{ return pKeepClean; }
	
	/** \brief Number of vegetation instances. */
	inline int GetVInstanceCount() const{ return pVInstanceCount; }
	
	/** \brief Vegetation instance at index. */
	meHTVInstance &GetVInstanceAt( int index ) const;
	
	/** \brief Add vegetation instance. */
	meHTVInstance &AddVInstance();
	
	/** \brief Remove all vegetation instances. */
	void RemoveAllVInstances();
	
	/** \brief Update vegetation instances if dirty. */
	void UpdateVInstances();
	
	/** \brief Mark vegetation instances dirty. */
	void MarkVInstancesDirty();
	
	/** \brief Mark vegetation instances valid. */
	void MarkVInstancesValid();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
