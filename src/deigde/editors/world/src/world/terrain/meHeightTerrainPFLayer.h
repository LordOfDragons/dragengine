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

#ifndef _MEHEIGHTTERRAINPFLAYER_H_
#define _MEHEIGHTTERRAINPFLAYER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class meHeightTerrainSector;
class meHeightTerrainPFType;
class meByteArray;

class deEngine;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * \brief Prop field type.
 */
class meHeightTerrainPFLayer : public deObject{
private:
	meHeightTerrainSector *pHTSector;
	
	deEngine *pEngine;
	
	decString pPathMask;
	meByteArray *pMask;
	bool pMaskChanged;
	bool pMaskSaved;
	
	meHeightTerrainPFType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrainPFLayer( deEngine *engine );
	
	/** \brief Clean up object. */
	virtual ~meHeightTerrainPFLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain sector or \em NULL. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** \brief Set parent height terrain sector or \em NULL. */
	void SetHTSector( meHeightTerrainSector *htsector );
	
	
	
	/** \brief Rebuild instances. */
	void RebuildInstances();
	/*@}*/
	
	
	
	/** \name Mask */
	/*@{*/
	/** \brief Mask image path. */
	inline const decString &GetPathMask() const{ return pPathMask; }
	
	/** \brief Set mask image path. */
	void SetPathMask( const char *path );
	
	/** \brief Mask image has changed. */
	inline bool GetMaskChanged() const{ return pMaskChanged; }
	
	/** \brief Set if mask image has changed. */
	void SetMaskChanged( bool changed );
	
	/** \brief Mask image has been saved. */
	inline bool GetMaskSaved() const{ return pMaskSaved; }
	
	/** \brief Set if mask image has been saved. */
	void SetMaskSaved( bool saved );
	
	
	
	/** \brief Mask bytes. */
	inline meByteArray *GetMask() const{ return pMask; }
	
	/** \brief Notify mask changed. */
	void NotifyMaskChanged();
	
	/** \brief Load mask from stored path. */
	void LoadMaskFromImage();
	
	/** \brief Height terrain sector size or resolution changed. */
	void SectorSizeOrResChanged();
	/*@}*/
	
	
	
	/** \name Types */
	/*@{*/
	/** \brief Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Type at index. */
	meHeightTerrainPFType *GetTypeAt( int index ) const;
	
	/** \brief Index of type or -1 if absent. */
	int IndexOfType( meHeightTerrainPFType *type ) const;
	
	/** \brief Type exists. */
	bool HasType( meHeightTerrainPFType *type ) const;
	
	/** \brief Add type. */
	void AddType( meHeightTerrainPFType *type );
	
	/** \brief Remove type. */
	void RemoveType( meHeightTerrainPFType *type );
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pResizeMask();
};

#endif
