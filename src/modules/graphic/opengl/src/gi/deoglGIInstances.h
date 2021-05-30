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
#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglRComponent;
class deoglGIInstance;
class deoglGIState;
class deoglRenderThread;


/**
 * Global illumination instances.
 * 
 * Stores global list of instances. Instances can be components or other scene elements
 * used for ray tracing.
 */
class deoglGIInstances{
public:
	struct sDynamicBox{
		decVector minExtend;
		decVector maxExtend;
	};
	
	
	
private:
	deoglGIState &pGIState;
	
	decObjectList pInstances;
	
	sDynamicBox *pDynamicBoxes;
	int pDynamicBoxCount;
	int pDynamicBoxSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination ray tracing. */
	deoglGIInstances( deoglGIState &giState );
	
	/** Clean up global illumination ray tracing. */
	~deoglGIInstances();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** GI state. */
	inline deoglGIState &GetGIState() const{ return pGIState; }
	
	/** Classify content. */
	static bool IsComponentStatic( const deoglRComponent &component );
	static bool IsOcclusionMeshStatic( const deoglRComponent &component );
	
	/** Count of available instance slots. */
	inline int GetInstanceCount() const{ return pInstances.GetCount(); }
	
	/** Instance at slot. */
	deoglGIInstance &GetInstanceAt( int slot ) const;
	
	/** Add instance slot. */
	deoglGIInstance &AddInstance();
	
	/** First free instance slot creating a new one if all slots are occupied. */
	deoglGIInstance &NextFreeSlot();
	
	
	
	/** Dynamic boxes list. */
	inline const sDynamicBox * const GetDynamicBoxes() const{ return pDynamicBoxes; }
	
	/** Count of dynamic boxes. */
	inline int GetDynamicBoxCount() const{ return pDynamicBoxCount; }
	
	/** Update dynamic boxes list from dynamic instances while enlarging boxes. */
	void UpdateDynamicBoxes( const decDVector &offset, const decVector &enlarge );
	
	/** One or more dynamic boxes contains point. */
	bool DynamicBoxesContain( const decVector &point ) const;
	
	/** Count of dynamic boxes contains point. */
	int CountDynamicBoxesContaining( const decVector &point ) const;
	
	
	
	/** Clear without triggering effects. */
	void Clear();
	
	/** One or more static instances have changed. */
	void AnyChanged() const;
	
	/** Clear changed flag of all instances. */
	void ClearAllChanged();
	
	/** Add components if not present */
	void AddComponents( deoglCollideList &list );
	
	/** Remove components if present */
	void RemoveComponents( deoglCollideList &list );
	
	/** Add occlusion meshes if not present already. Returns true if static occlusion meshes have been added. */
	bool AddOcclusionMeshes( deoglCollideList &list );
	
	/** Remove occlusion meshes if present. Returns true if static occlusion meshes have been removed. */
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
