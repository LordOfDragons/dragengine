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

#ifndef _DEVKDESCRIPTORSETLAYOUTMANAGER_H_
#define _DEVKDESCRIPTORSETLAYOUTMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>

class devkDevice;
class devkDescriptorSetLayout;
class devkDescriptorSetLayoutConfiguration;


/**
 * Descriptor set layout manager.
 */
class devkDescriptorSetLayoutManager{
private:
	devkDevice &pDevice;
	
	decObjectList pLayouts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor set layout manager. */
	devkDescriptorSetLayoutManager( devkDevice &device );
	
	/** Clean up descriptor set layouts manager. */
	~devkDescriptorSetLayoutManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Count of descriptor set layouts. */
	int GetCount() const;
	
	/** Descriptor set layout at index. Caller does not hold reference. */
	devkDescriptorSetLayout *GetAt( int index ) const;
	
	/** Descriptor set layout with configuration creating it if absent. Caller does not hold reference. */
	devkDescriptorSetLayout *GetWith( const devkDescriptorSetLayoutConfiguration &configuration );
	
	/** Descriptor set layout with configuration is present. */
	bool HasWith( const devkDescriptorSetLayoutConfiguration &configuration ) const;
	
	/** Remove all descriptor set layouts. */
	void Clear();
	/*@}*/
};

#endif
