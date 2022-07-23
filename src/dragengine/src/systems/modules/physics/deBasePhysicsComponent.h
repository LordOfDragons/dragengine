/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEPHASICSCOMPONENT_H_
#define _DEBASEPHASICSCOMPONENT_H_

#include "../../../common/math/decMath.h"

class deDecal;
class deDecalList;
class decShape;


/**
 * \brief Physics module component peer.
 */
class DE_DLL_EXPORT deBasePhysicsComponent{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBasePhysicsComponent();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsComponent();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Scaling changed. */
	virtual void ScalingChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Model object changed. */
	virtual void ModelChanged();
	
	/** \brief Rig object changed. */
	virtual void RigChanged();
	
	/** \brief Visitility changed. */
	virtual void VisibilityChanged();
	
	/** \brief Extends changed. */
	virtual void ExtendsChanged();
	
	/**
	 * \brief Mesh vertices have been invalidated.
	 * 
	 * Called if Model changed or bones have been invalidated.
	 */
	virtual void MeshDirty();
	
	/** \brief Parameter or hint changed. */
    virtual void ParametersChanged();
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved();
	/*@}*/
	
	
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Collect decals in contact with point.
	 * 
	 * The point is best taken from an earlier collision test and is in world
	 * coordinates. The point is considered to be in the same sector as the component.
	 * 
	 * \param[in] point Point on the surface to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	virtual void FindDecalsAt( const decVector &point, deDecalList &list );
	
	/**
	 * \brief Collect decals in contact with shape.
	 * 
	 * The shape is considered to be in the same sector as the component.
	 * 
	 * \param[in] shape Shape to test.
	 * \param[out] list List of decals to fill in. Not emptied before filling.
	 */
	virtual void FindDecalsTouching( decShape *shape, deDecalList &list );
	/*@}*/
};

#endif
