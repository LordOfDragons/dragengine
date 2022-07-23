/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEVSPECIALIZATION_H_
#define _DEVSPECIALIZATION_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>


/**
 * Vulkan specialization. Defines the layout of specialization only.
 */
class devkSpecialization : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkSpecialization> Ref;
	
	
	
private:
	VkSpecializationMapEntry *pEntries;
	int pEntryCount;
	char *pData;
	int pDataSize;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create specialization configuration. */
	devkSpecialization();
	
	/** Create specialization configuration. */
	devkSpecialization( const void *data, int dataSize, int entryCount );
	
	/** Create copy of specialization configuration. */
	devkSpecialization( const devkSpecialization &configuration );
	
protected:
	/** Clean up specialization configuration. */
	virtual ~devkSpecialization();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Count of entries. */
	inline int GetEntryCount() const{ return pEntryCount; }
	
	/** Set count of entries. */
	void SetEntryCount( int count );
	
	/** Entry. */
	inline const VkSpecializationMapEntry &GetEntryAt( int index ) const;
	
	/** Set entry. */
	void SetEntryAt( int index, const VkSpecializationMapEntry &entry );
	
	/** Set entry. */
	void SetEntryAt( int index, int constantID, int offset, int size );
	void SetEntryIntAt( int index, int constantID, int offset );
	void SetEntryUIntAt( int index, int constantID, int offset );
	void SetEntryFloatAt( int index, int constantID, int offset );
	void SetEntryBoolAt( int index, int constantID, int offset );
	
	/** For devkSpecialization use only. */
	inline const VkSpecializationMapEntry *GetEntries() const{ return pEntries; }
	
	/** Data. */
	inline const void *GetData() const{ return pData; }
	
	/** Data size. */
	inline int GetDataSize() const{ return pDataSize; }
	
	/** Set data. */
	void SetData( const void *data, int size );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkSpecialization &configuration ) const;
	
	/** Copy configuration. */
	devkSpecialization &operator=( const devkSpecialization &configuration );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

