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

#ifndef _DEVKPIPELINE_H_
#define _DEVKPIPELINE_H_

#include "devkPipelineConfiguration.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>

class devkDevice;
class devkDescriptorSetLayout;
class decPath;


/**
 * Vulkan pipeline.
 */
class devkPipeline : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkPipeline> Ref;
	
	
	
protected:
	devkDevice &pDevice;
	const devkPipelineConfiguration pConfiguration;
	
	VkPipelineLayout pLayout;
	VkPipeline pPipeline;
	VkPipelineCache pCache;
	bool pSaveCache;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
	devkPipeline( devkDevice &device, const devkPipelineConfiguration &configuration );
	
	/** Clean up pipeline. */
	virtual ~devkPipeline();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Configuration. */
	inline const devkPipelineConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** Pipeline. */
	inline VkPipeline GetPipeline() const{ return pPipeline; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	decPath pCachePath() const;
	void pDropCache();
};

#endif

