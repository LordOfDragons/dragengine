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

#ifndef _DEOGLRPROPFIELD_H_
#define _DEOGLRPROPFIELD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

#include "../deoglGL.h"

class deoglRenderPlan;
class deoglRenderThread;
class deoglRPropFieldType;
class deoglRWorld;

class dePropField;


/**
 * Render prop field. The LOD parameter indicates the lod level using a value in the range
 * of 0 to 1. 0 Indicates that the prop field is fully ignored. Changing to this lod level
 * clears the content of the prop field. A value of 1 indicates that the prop field is fully
 * populated with all possible instances. Changing to this lod level clears the prop field
 * and calls the script to create the instances with 1 density as parameter.
 */
class deoglRPropField : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	decDVector pPosition;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decObjectList pTypes;
	bool pTypesRequirePrepareForRender;
	
	bool pWorldMarkedRemove;
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRPropField> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render prop field. */
	deoglRPropField( deoglRenderThread &renderThread );
	
	/** Clean up render prop field. */
	virtual ~deoglRPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Parent world or NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or NULL if not set. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decDVector &position );
	
	
	
	/** Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Update extends. */
	void UpdateExtends( const dePropField &propField );
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender();
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRenderRender();
	
	/**
	 * Prepare instances for rendering using a camera position.
	 * First all instances are removed. Then each instance is distance tested
	 * using the given camera position. Only instances inside the clipping
	 * distance are added to the list of instances to render. For all the
	 * instances entered into the list their matrix is calculated using the
	 * given camera matrix. After this is done the instance matrices are ready
	 * to be used in subsequent rendering calls.
	 */
	void PrepareInstances( const decDVector &cameraPosition, const decDMatrix &cameraMatrix );
	
	
	
	/** Number of types. */
	int GetTypeCount() const;
	
	/** Type at index. */
	deoglRPropFieldType &GetTypeAt( int index ) const;
	
	/** Remove all types. */
	void RemoveAllTypes();
	
	/** Add type. */
	void AddType( deoglRPropFieldType* type );
	
	/** Type requires prepare for render. */
	void TypeRequiresPrepareForRender();
	
	
	
	/** World reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** Marked for removal. For use by deoglRWorld only. Non-thread safe. */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/** Set marked for removal. */
	void SetWorldMarkedRemove( bool marked );
	
	/** World prepare for render linked list. */
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
	
	
private:
	void pRequiresPrepareForRender();
};

#endif
