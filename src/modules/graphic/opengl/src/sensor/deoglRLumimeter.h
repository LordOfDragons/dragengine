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

#ifndef _DEOGLRLUMIMETER_H_
#define _DEOGLRLUMIMETER_H_

#include "../light/deoglLightList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglRWorld;
class deoglWorldOctree;



/**
 * \brief Render lumimeter.
 */
class deoglRLumimeter : public deObject{
public:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	bool pDirtyOctree;
	
	decDVector pPosition;
	
	float pLuminance;
	decColor pColor;
	
	bool pDirtyMeasurements;
	
	deoglLightList pLights;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render lumimeter. */
	deoglRLumimeter( deoglRenderThread &renderThread );
	
	/** \brief Clean up render lumimeter. */
	virtual ~deoglRLumimeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Mark lumimeter dirty. */
	void MarkDirty();
	
	/** \brief Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent scene or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** \brief Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * \brief Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by the deoglWorldOctree only.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** \brief Update position in the parent octree. */
	void UpdateOctreeNode();
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	
	
	/** \brief Update measurements. */
	void UpdateMeasurements();
	
	/** \brief Measured luminance. */
	inline float GetLuminance() const{ return pLuminance; }
	
	/** \brief Measured color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	
	
	/** \brief Prepare for quick disposal of lumimeter. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Lights Management */
	/*@{*/
	/** \brief Number of lights. */
	int GetLightCount() const;
	
	/** \brief Light at index. */
	deoglRLight *GetLightAt( int index ) const;
	
	/** \brief Add light. */
	void AddLight( deoglRLight *light );
	
	/** \brief Remove light. */
	void RemoveLight( deoglRLight *light );
	
	/** \brief Remove all lights. */
	void RemoveAllLights();
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
