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

#ifndef _DECOLLIDERVISITORIDENTIFY_H_
#define _DECOLLIDERVISITORIDENTIFY_H_

#include "deColliderVisitor.h"


/**
 * \brief Identify collider visitor.
 */
class DE_DLL_EXPORT deColliderVisitorIdentify : public deColliderVisitor{
public:
	/** \brief Collider types. */
	enum eColliderTypes{
		/** \brief Unknown type. */
		ectUnknown,
		
		/** \brief Collider volume. */
		ectVolume,
		
		/** \brief Collider rig. */
		ectRig,
		
		/** \brief Collider component. */
		ectComponent
	};
	
	
	
private:
	deCollider *pCollider;
	eColliderTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identify visitor. */
	deColliderVisitorIdentify();
	
	/** \brief Clean up identify collider visitor. */
	virtual ~deColliderVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collider type. */
	inline eColliderTypes GetType() const{ return pType; }
	
	
	
	/** \brief Visited collider is an unknown collider. */
	inline bool IsUnknown() const{ return pType == ectUnknown; }
	
	/** \brief Visited collider is a volume collider. */
	inline bool IsVolume() const{ return pType == ectVolume; }
	
	/** \brief Visited collider is a rig collider. */
	inline bool IsRigged() const{ return pType == ectRig; }
	
	/** \brief Visited collider is component collider. */
	inline bool IsComponent() const{ return pType == ectComponent; }
	
	
	
	/**
	 * \brief Cast to volume collider.
	 * \throws deeInvalidParam Visited collider is not a volume collider.
	 */
	deColliderVolume &CastToVolume() const;
	
	/**
	 * \brief Cast to rig collider.
	 * \throws deeInvalidParam Visited collider is not a rig collider.
	 */
	deColliderRig &CastToRig() const;
	
	/**
	 * \brief Cast to component collider.
	 * \throws deeInvalidParam Visited collider is not a component collider.
	 */
	deColliderComponent &CastToComponent() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void VisitCollider( deCollider &collider );
	
	/** \brief Visit volume collider. */
	virtual void VisitVolume( deColliderVolume &collider );
	
	/** \brief Visit rig collider. */
	virtual void VisitRig( deColliderRig &collider );
	
	/** \brief Visit component collider. */
	virtual void VisitComponent( deColliderComponent &collider );
	/*@}*/
};

#endif
