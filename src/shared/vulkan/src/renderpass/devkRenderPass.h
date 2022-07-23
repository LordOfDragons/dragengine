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

#ifndef _DEVKRENDERPASS_H_
#define _DEVKRENDERPASS_H_

#include "devkRenderPassConfiguration.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>

class devkDevice;


/**
 * Vulkan render pass.
 */
class devkRenderPass : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkRenderPass> Ref;
	
	
	
protected:
	devkDevice &pDevice;
	const devkRenderPassConfiguration pConfiguration;
	
	VkRenderPass pRenderPass;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
	devkRenderPass( devkDevice &device, const devkRenderPassConfiguration &configuration );
	
protected:
	/** Clean up pipeline. */
	virtual ~devkRenderPass();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Configuration. */
	inline const devkRenderPassConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** RenderPass. */
	inline VkRenderPass GetRenderPass() const{ return pRenderPass; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

