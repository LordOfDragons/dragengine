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

#ifndef _DEVKPIPELINEMANAGER_H_
#define _DEVKPIPELINEMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>

class devkDevice;
class devkPipeline;
class devkPipelineConfiguration;
class devkRenderPass;


/**
 * Pipeline manager.
 */
class devkPipelineManager{
private:
	devkDevice &pDevice;
	
	decObjectList pLayouts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline manager. */
	devkPipelineManager(devkDevice &device);
	
	/** Clean up pipelines manager. */
	~devkPipelineManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{return pDevice;}
	
	/** Count of pipelines. */
	int GetCount() const;
	
	/** Pipeline at index. Caller does not hold reference. */
	devkPipeline *GetAt(int index) const;
	
	/** Pipeline with configuration creating it if absent. Caller does not hold reference. */
	devkPipeline *GetWith(const devkPipelineConfiguration &configuration);
	
	/** Pipeline with configuration is present. */
	bool HasWith(const devkPipelineConfiguration &configuration) const;
	
	/** Remove all pipelines. */
	void Clear();
	/*@}*/
};

#endif
