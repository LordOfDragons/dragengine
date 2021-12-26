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

#ifndef _DEBASEPHYSICSFORCEFIELD_H_
#define _DEBASEPHYSICSFORCEFIELD_H_

#include "../../../dragengine_export.h"


/**
 * \brief Physics system force field peer.
 */
class DE_DLL_EXPORT deBasePhysicsForceField{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBasePhysicsForceField();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsForceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Influence area changed. */
	virtual void InfluenceAreaChanged();
	
	/** \brief Radius changed. */
	virtual void RadiusChanged();
	
	/** \brief Exponent changed. */
	virtual void ExponentChanged();
	
	/** \brief Field or application type changed. */
	virtual void TypeChanged();
	
	/** \brief Direction changed. */
	virtual void DirectionChanged();
	
	/** \brief Force changed. */
	virtual void ForceChanged();
	
	/** \brief Direction or force fluctuation changed. */
	virtual void FluctuationChanged();
	
	/** \brief Shape changed. */
	virtual void ShapeChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	/*@}*/
};

#endif
