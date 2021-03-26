/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLGIINSTANCES_H_
#define _DEOGLGIINSTANCES_H_

#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decObjectList.h>

class deoglCollideList;
class deoglGIInstance;
class deoglRenderThread;


/**
 * Global illumination instances.
 * 
 * Stores global list of instances. Instances can be components or other scene elements
 * used for ray tracing.
 */
class deoglGIInstances{
private:
	deoglRenderThread &pRenderThread;
	
	decObjectList pInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGIInstances( deoglRenderThread &renderThread );
	
	/** Clean up global illumination ray tracing. */
	~deoglGIInstances();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Count of available instance slots. */
	inline int GetInstanceCount() const{ return pInstances.GetCount(); }
	
	/** Instance at slot. */
	deoglGIInstance &GetInstanceAt( int slot ) const;
	
	/** Add instance slot. */
	deoglGIInstance &AddInstance();
	
	/** First free instance slot creating a new one if all slots are occupied. */
	deoglGIInstance &NextFreeSlot();
	
	/** One or more instances have changed. */
	bool AnyChanged() const;
	
	/** One or more components have changed. */
	bool AnyComponentChanged() const;
	
	/** One or more occlusion meshes have changed. */
	bool AnyOcclusionMeshChanged() const;
	
	/** Clear changed flag of all instances. */
	void ClearAllChanged();
	
	/** Add components if not present. Returns true if components have been added. */
	bool AddComponents( deoglCollideList &list );
	
	/** Remove components if present. Returns true if components have been removed. */
	bool RemoveComponents( deoglCollideList &list );
	
	/** Add occlusion meshes if not present already. Returns true if occlusion meshes have been added. */
	bool AddOcclusionMeshes( deoglCollideList &list );
	
	/** Remove occlusion meshes if present. Returns true if occlusion meshes have been removed. */
	bool RemoveOcclusionMeshes( deoglCollideList &list );
	
	/** Mark instances. */
	void MarkAll( bool marked );
	
	/** Mark components. */
	void MarkComponents( bool marked );
	
	/** Mark occlusion meshes. */
	void MarkOcclusionMeshes( bool marked );
	
	/** Debug print. */
	void DebugPrint();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
