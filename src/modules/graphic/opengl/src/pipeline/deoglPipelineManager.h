/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLPIPELINEMANAGER_H_
#define _DEOGLPIPELINEMANAGER_H_

#include "deoglPipelineState.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglRenderThread;
class deoglPipeline;
class deoglPipelineConfiguration;


/**
 * Pipeline manager.
 */
class deoglPipelineManager : public deObject{
public:
	typedef deTObjectReference<deoglPipelineManager> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	deoglPipelineState pState;
	decObjectList pPipelines;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline manager. */
	deoglPipelineManager( deoglRenderThread &renderThread );
	
protected:
	/** Clean up pipelines manager. */
	virtual ~deoglPipelineManager();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** State. */
	inline deoglPipelineState &GetState(){ return pState; }
	
	/** Count of pipelines. */
	int GetCount() const;
	
	/** Pipeline at index. Caller does not hold reference. */
	const deoglPipeline *GetAt( int index ) const;
	
	/** Pipeline with configuration creating it if absent. Caller does not hold reference. */
	const deoglPipeline *GetWith( const deoglPipelineConfiguration &configuration,
		bool assignRTSPipelineIndex = false );
	
	/** Pipeline with configuration is present. */
	bool HasWith( const deoglPipelineConfiguration &configuration ) const;
	
	/** Remove all pipelines. */
	void Clear();
	/*@}*/
};

#endif
