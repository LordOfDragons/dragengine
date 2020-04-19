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

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>

#include "../deoglGL.h"

class deoglRenderThread;
class deoglRPropFieldType;
class deoglRWorld;

class dePropField;



/**
 * \brief Render prop field.
 * The LOD parameter indicates the lod level using a value in the range of 0 to 1.
 * 0 Indicates that the prop field is fully ignored. Changing to this lod level
 * clears the content of the prop field. A value of 1 indicates that the prop field
 * is fully populated with all possible instances. Changing to this lod level
 * clears the prop field and calls the script to create the instances with 1
 * density as parameter.
 */
class deoglRPropField : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	decDVector pPosition;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	decObjectList pTypes;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render prop field. */
	deoglRPropField( deoglRenderThread &renderThread );
	
	/** \brief Clean up render prop field. */
	virtual ~deoglRPropField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	
	
	/** \brief Minimum extend. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** \brief Update extends. */
	void UpdateExtends( const dePropField &propField );
	
	
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	
	/**
	 * \brief Prepare instances for rendering using a camera position.
	 * \details First all instances are removed. Then each instance is distance tested
	 *          using the given camera position. Only instances inside the clipping
	 *          distance are added to the list of instances to render. For all the
	 *          instances entered into the list their matrix is calculated using the
	 *          given camera matrix. After this is done the instance matrices are ready
	 *          to be used in subsequent rendering calls.
	 */
	void PrepareInstances( const decDVector &cameraPosition, const decDMatrix &cameraMatrix );
	
	
	
	/** \brief Number of types. */
	int GetTypeCount() const;
	
	/** \brief Type at index. */
	deoglRPropFieldType &GetTypeAt( int index ) const;
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	
	/** \brief Add type. */
	void AddType( deoglRPropFieldType* type );
	
	
	
	/** \brief World reference point changed. */
	void WorldReferencePointChanged();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
};

#endif
