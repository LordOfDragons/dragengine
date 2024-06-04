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

#ifndef _DEOGLGIINSTANCES_H_
#define _DEOGLGIINSTANCES_H_

#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decPointerDictionaryExt.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglRComponent;
class deoglRDecal;
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
	inline deoglGIState &GetGIState() const{ return pGIState; }
	
	/** Classify content. */
	static bool IsComponentStatic( const deoglRComponent &component );
	static bool IsDecalStatic( const deoglRDecal &decal );
	
	/** Count of available instance slots. */
	inline int GetInstanceCount() const{ return pInstances.GetCount(); }
	
	/** Instance at slot. */
	deoglGIInstance &GetInstanceAt( int slot ) const;
	
	/** Instance with component or NULL. */
	deoglGIInstance *GetInstanceWithComponent( deoglRComponent *component ) const;
	
	/** Instance with decal or NULL. */
	deoglGIInstance *GetInstanceWithDecal( deoglRDecal *decal ) const;
	
	/** Add instance slot. */
	deoglGIInstance &AddInstance();
	
	/** First free instance slot creating a new one if all slots are occupied. */
	deoglGIInstance &NextFreeSlot();
	
	/** Register instance element. */
	void RegisterElement( deoglRComponent *component, deoglGIInstance *instance );
	void RegisterElement( deoglRDecal *decal, deoglGIInstance *instance );
	void RegisterElement( void *element, unsigned int hash, deoglGIInstance *instance );
	
	/** Unregister instance element. */
	void UnregisterElement( deoglRComponent *component );
	void UnregisterElement( deoglRDecal *decal );
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
	
	/** Add decal. */
	void AddDecal( deoglRDecal *decal, bool invalidate );
	
	/** Add decals. */
	void AddDecals( const deoglRComponent &component, bool invalidate );
	
	/** Remove component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** Remove components. */
	void RemoveComponents( const deoglCollideList &list );
	
	/** Remove decal. */
	void RemoveDecal( deoglRDecal *decal );
	
	/** Remove decals. */
	void RemoveDecals( const deoglRComponent &component );
	
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
	void pInvalidateAddInstance( const deoglGIInstance &instance );
};

#endif
