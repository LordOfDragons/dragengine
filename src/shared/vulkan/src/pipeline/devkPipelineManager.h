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
	devkPipelineManager( devkDevice &device );
	
	/** Clean up pipelines manager. */
	~devkPipelineManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Count of pipelines. */
	int GetCount() const;
	
	/** Pipeline at index. Caller does not hold reference. */
	devkPipeline *GetAt( int index ) const;
	
	/** Pipeline with configuration creating it if absent. Caller does not hold reference. */
	devkPipeline *GetWith( const devkPipelineConfiguration &configuration );
	
	/** Pipeline with configuration is present. */
	bool HasWith( const devkPipelineConfiguration &configuration ) const;
	
	/** Remove all pipelines. */
	void Clear();
	/*@}*/
};

#endif
