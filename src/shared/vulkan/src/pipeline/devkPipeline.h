/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEVKPIPELINE_H_
#define _DEVKPIPELINE_H_

#include "devkPipelineConfiguration.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>

class devkDevice;
class devkDescriptorSetLayout;
class devkShaderModule;
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
	
	VkPipelineBindPoint pBindPoint;
	
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
	
	/** Bind point. */
	inline VkPipelineBindPoint GetBindPoint() const{ return pBindPoint; }
	
	/** Pipeline layout. */
	inline VkPipelineLayout GetLayout() const{ return pLayout; }
	
	/** Pipeline. */
	inline VkPipeline GetPipeline() const{ return pPipeline; }
	/*@}*/
	
	
	
protected:
	void pInitShaderStage( VkPipelineShaderStageCreateInfo &info,
		VkShaderStageFlagBits stage, devkShaderModule &module,
		const VkSpecializationInfo *specialization = nullptr );
	
	
	
private:
	void pCleanUp();
	decPath pCachePath() const;
	void pDropCache();
};

#endif

