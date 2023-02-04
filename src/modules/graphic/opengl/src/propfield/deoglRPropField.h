/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRPROPFIELD_H_
#define _DEOGLRPROPFIELD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decPointerLinkedList.h>

#include "../deoglGL.h"
#include "../world/deoglWorldCompute.h"

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
	/** World compute element. */
	class WorldComputeElement: public deoglWorldCompute::Element{
		deoglRPropField &pPropField;
	public:
		WorldComputeElement( deoglRPropField &propField );
		virtual void UpdateData( const deoglWorldCompute &worldCompute, deoglWorldCompute::sDataElement &data );
	};
	
	
	
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	deoglWorldCompute::Element::Ref pWorldComputeElement;
	
	decDVector pPosition;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decObjectList pTypes;
	bool pTypesRequirePrepareForRender;
	
	bool pWorldMarkedRemove;
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
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
