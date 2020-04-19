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
 * @brief Texture Units Configuration.
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
	
	int pUsageCount;
	unsigned int pHashCode;
	deoglTexUnitsConfig *pLLPrev;
	deoglTexUnitsConfig *pLLNext;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new texture units configuration. */
	deoglTexUnitsConfig( deoglRenderThread &renderThread );
	/** Cleans up the texture units configuration. */
	~deoglTexUnitsConfig();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the number of units. */
	inline int GetUnitCount() const{ return pUnitCount; }
	/** Sets the number of units. Disabled all units. */
	void SetUnitCount( int count );
	/** Retrieves the texture units pointer for fast access without index checking. */
	inline deoglTexUnitConfig *GetUnits() const{ return pUnits; }
	/** Retrieves a texture unit. */
	deoglTexUnitConfig &GetUnitAt( int index ) const;
	/** Sets the units. */
	void SetUnits( const deoglTexUnitConfig *units, int unitCount );
	
	/** Apply the texture units configuration. */
	void Apply();
	
	/** Retrieves the render task tracking number. */
	inline unsigned int GetRenderTaskTrackingNumber() const{ return pRenderTaskTrackingNumber; }
	/** Sets the render task tracking number. */
	void SetRenderTaskTrackingNumber( unsigned int trackingNumber );
	/** Retrieves the render task texture units configuration index. */
	inline int GetRenderTaskTUCIndex() const{ return pRenderTaskTUCIndex; }
	/** Sets the render task texture units configuration index. */
	void SetRenderTaskTUCIndex( int tucIndex );
	
	/** Determines if this object matches another one. */
	bool Equals( const deoglTexUnitsConfig &tuc ) const;
	/** Determines if this object matches another one. */
	bool Equals( const deoglTexUnitConfig *units, int unitCount ) const;
	
	/** Retrieves the usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	/** Increments the usage count by one. */
	void AddUsage();
	/**
	 * Decreases the usage count by one. If the usage count reaches 0 the object is removed
	 * from the texture units configuration manager and deleted.
	 */
	void RemoveUsage();
	/** Retrieves the hash code. */
	inline unsigned int GetHashCode() const{ return pHashCode; }
	/** Calculate hash code. */
	void CalcHashCode();
	/** Calculates the hash code for a set of unit configurations. */
	static unsigned int CalcHashCodeForUnits( const deoglTexUnitConfig *units, int unitCount );
	
	/** Retrieves the previous entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLPrev() const{ return pLLPrev; }
	/** Sets the previous entry in the linked list. */
	void SetLLPrev( deoglTexUnitsConfig *entry );
	/** Retrieves the next entry in the linked list. */
	inline deoglTexUnitsConfig *GetLLNext() const{ return pLLNext; }
	/** Sets the next entry in the linked list. */
	void SetLLNext( deoglTexUnitsConfig *entry );
	/*@}*/
};

#endif
