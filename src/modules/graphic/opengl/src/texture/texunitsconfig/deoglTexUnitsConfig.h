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

#ifndef _DEOGLTEXUNITSCONFIG_H_
#define _DEOGLTEXUNITSCONFIG_H_

#include "../../deoglBasics.h"

class deoglRenderThread;
class deoglCubeMap;
class deoglRenderPlan;
class deoglTexture;
class deoglTexUnitConfig;



/**
 * Texture Units Configuration.
 * 
 * Stores the configuration of up to 8 texture units. This class does not check
 * for errors. The user of the texture units configuration is responsible to
 * ensure a correct setup. The hash code is calculated from the opengl texture
 * names assigned to the units. This works no matter what texture is used since
 * every texture in opengl has a unique name no matter what type of texture it
 * is. These values are added up using a bit of bit shifting.
 */
class deoglTexUnitsConfig{
public:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitConfig *pUnits;
	int pUnitCount;
	
	unsigned int pRenderTaskTrackingNumber;
	int pRenderTaskTUCIndex;
	
	int pMaterialIndex;
	
	int pUsageCount;
	unsigned int pHashCode;
	deoglTexUnitsConfig *pLLPrev;
	deoglTexUnitsConfig *pLLNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create texture units configuration. */
	deoglTexUnitsConfig( deoglRenderThread &renderThread );
	
	/** Clean up texture units configuration. */
	~deoglTexUnitsConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of units. */
	inline int GetUnitCount() const{ return pUnitCount; }
	
	/** Set count of units. Disabled all units. */
	void SetUnitCount( int count );
	
	/** Texture units pointer for fast access without index checking. */
	inline deoglTexUnitConfig *GetUnits() const{ return pUnits; }
	
	/** Texture unit. */
	deoglTexUnitConfig &GetUnitAt( int index ) const;
	
	/** Set units. */
	void SetUnits( const deoglTexUnitConfig *units, int unitCount );
	
	/** Apply texture units configuration. */
	void Apply();
	
	
	
	/** Render task tracking number. */
	inline unsigned int GetRenderTaskTrackingNumber() const{ return pRenderTaskTrackingNumber; }
	
	/** Set render task tracking number. */
	void SetRenderTaskTrackingNumber( unsigned int trackingNumber );
	
	/** Render task texture units configuration index. */
	inline int GetRenderTaskTUCIndex() const{ return pRenderTaskTUCIndex; }
	
	/** Set render task texture units configuration index. */
	void SetRenderTaskTUCIndex( int tucIndex );
	
	
	
	/** \brief Material index or -1. */
	inline int GetMaterialIndex() const{ return pMaterialIndex; }
	
	/** \brief Set material index or -1. */
	void SetMaterialIndex( int index );
	
	
	
	/** Texture units configuration matches another one. */
	bool Equals( const deoglTexUnitsConfig &tuc ) const;
	
	/** Texture units configuration matches another one. */
	bool Equals( const deoglTexUnitConfig *units, int unitCount ) const;
	
	/** Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** Increment usage count by one. */
	void AddUsage();
	
	/**
	 * Decrease usage count by one. If the usage count reaches 0 the object is removed
	 * from the texture units configuration manager and deleted.
	 */
	void RemoveUsage();
	
	
	
	/** Hash code. */
	inline unsigned int GetHashCode() const{ return pHashCode; }
	
	/** Calculate hash code. */
	void CalcHashCode();
	
	/** Calculate hash code for set of unit configurations. */
	static unsigned int CalcHashCodeForUnits( const deoglTexUnitConfig *units, int unitCount );
	
	
	
	/** Previous entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLPrev() const{ return pLLPrev; }
	
	/** Set previous entry in the linked list. */
	void SetLLPrev( deoglTexUnitsConfig *entry );
	
	/** Next entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLNext() const{ return pLLNext; }
	
	/** Set next entry in the linked list. */
	void SetLLNext( deoglTexUnitsConfig *entry );
	/*@}*/
};

#endif
