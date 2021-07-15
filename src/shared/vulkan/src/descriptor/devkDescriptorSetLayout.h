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

#ifndef _DEVKDESCRIPTORSETLAYOUT_H_
#define _DEVKDESCRIPTORSETLAYOUT_H_

#include "devkDescriptorSetLayoutConfiguration.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>

class devkDevice;


/**
 * Vulkan descriptor layout.
 */
class devkDescriptorSetLayout : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorSetLayout> Ref;
	
	
	
private:
	devkDevice &pDevice;
	const devkDescriptorSetLayoutConfiguration pConfiguration;
	VkDescriptorSetLayout pLayout;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor layout. */
	devkDescriptorSetLayout( devkDevice &device, const devkDescriptorSetLayoutConfiguration &configuration );
	
protected:
	/** Clean up descriptor layout. */
	virtual ~devkDescriptorSetLayout();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Configuration. */
	inline const devkDescriptorSetLayoutConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** Layout. */
	inline const VkDescriptorSetLayout GetLayout() const{ return pLayout; }
	/*@}*/
};

#endif

