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
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decPointerDictionaryExt.h>
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
	struct sBox{
		decVector minExtend;
		decVector maxExtend;
	};
	
	
	
private:
	deoglGIState &pGIState;
	
	decObjectList pInstances;
	decPointerList pEmptyInstances;
	decPointerDictionaryExt pElementInstanceMap;
	
	sBox *pDynamicBoxes;
	int pDynamicBoxCount;
	int pDynamicBoxSize;
	
	decPointerList pChangedInstances;
	
	
	
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
	inline deoglGIState &GetGICascade() const{ return pGIState; }
	
	/** Classify content. */
	static bool IsComponentStatic( const deoglRComponent &component );
	
	/** Count of available instance slots. */
	inline int GetInstanceCount() const{ return pInstances.GetCount(); }
	
	/** Instance at slot. */
	deoglGIInstance &GetInstanceAt( int slot ) const;
	
	/** Instance with component or NULL. */
	deoglGIInstance *GetInstanceWithComponent( deoglRComponent *component ) const;
	
	/** Add instance slot. */
	deoglGIInstance &AddInstance();
	
	/** First free instance slot creating a new one if all slots are occupied. */
	deoglGIInstance &NextFreeSlot();
	
	/** Register instance element. */
	void RegisterElement( deoglRComponent *component, deoglGIInstance *instance );
	void RegisterElement( void *element, unsigned int hash, deoglGIInstance *instance );
	
	/** Unregister instance element. */
	void UnregisterElement( deoglRComponent *component );
	void UnregisterElement( void *element, unsigned int hash );
	
	
	
	/** Dynamic boxes list. */
	inline const sBox * const GetDynamicBoxes() const{ return pDynamicBoxes; }
	
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
	
	/** Apply changes. */
	void ApplyChanges();
	
	/** Add component. */
	void AddComponent( deoglRComponent *component, bool invalidate );
	
	/** Add components. */
	void AddComponents( const deoglCollideList &list, bool invalidate );
	
	/** Remove component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** Remove components. */
	void RemoveComponents( const deoglCollideList &list );
	
	/** Mark components. */
	void MarkComponents( bool marked );
	
	
	
	/** Remove instance. */
	void RemoveInstance( deoglGIInstance &instance );
	
	/** Instance changed. */
	void InstanceChanged( deoglGIInstance &instance );
	
	
	
	/** Debug print. */
	void DebugPrint();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
