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

class deoglCubeMap;
class deoglRenderPlan;
class deoglRenderTaskSharedTexture;
class deoglRenderThread;
class deoglSPBlockUBO;
class deoglTexture;
class deoglTexUnitConfig;


/**
 * Stores the configuration of up to 8 texture units and an optional shader parameter block.
 * This class does not check for errors. The user of the texture units configuration is
 * responsible to ensure a correct setup. The hash code is calculated from the opengl texture
 * names assigned to the units. This works no matter what texture is used since every texture
 * in opengl has a unique name no matter what type of texture it is.
 */
class deoglTexUnitsConfig{
public:
	deoglRenderThread &pRenderThread;
	
	deoglTexUnitConfig *pUnits;
	int pUnitCount;
	deoglSPBlockUBO *pParamBlock;
	
	int pMaterialIndex;
	int pMaterialUsageCount;
	
	int pUsageCount;
	unsigned int pUnitsHashCode;
	unsigned int pUniqueKey;
	
	deoglRenderTaskSharedTexture *pRTSTexture;
	
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
	
	/** Shader parameter block or NULL. */
	inline deoglSPBlockUBO *GetParameterBlock() const{ return pParamBlock; }
	
	/** Set shader parameter block or NULL. */
	void SetParameterBlock( deoglSPBlockUBO *paramBlock );
	
	
	
	/** Apply texture units configuration. */
	void Apply() const;
	
	
	
	/** Material index or -1. */
	inline int GetMaterialIndex() const{ return pMaterialIndex; }
	
	/** Set material index or -1. */
	void SetMaterialIndex( int index );
	
	/** Material usage count. */
	inline int GetMaterialUsageCount() const{ return pMaterialUsageCount; }
	
	/** Increment material usage count by one. */
	void AddMaterialUsage();
	
	/** Decrease material usage count by one. */
	void RemoveMaterialUsage();
	
	
	
	/** Texture units configuration matches another one. */
	bool Equals( const deoglTexUnitsConfig &tuc ) const;
	
	/** Texture units configuration matches another one. */
	bool Equals( const deoglTexUnitConfig *units, int unitCount, deoglSPBlockUBO *paramBlock ) const;
	
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
	inline unsigned int GetUnitsHashCode() const{ return pUnitsHashCode; }
	
	/** Calculate hash code. */
	void CalcUnitsHashCode();
	
	/** Calculate hash code for set of unit configurations. */
	static unsigned int CalcUnitsHashCodeForUnits( const deoglTexUnitConfig *units, int unitCount );
	
	
	
	/** Unique key for use with dictionaries. */
	inline unsigned int GetUniqueKey() const{ return pUniqueKey; }
	
	
	
	/** Render task shared texture or NULL. */
	inline deoglRenderTaskSharedTexture *GetRTSTexture() const{ return pRTSTexture; }
	
	/** Ensure render task shared texture is present. */
	void EnsureRTSTexture();
	
	
	
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
