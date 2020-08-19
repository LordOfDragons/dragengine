/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSLCONTROLLERMAPPING_H_
#define _DEDSLCONTROLLERMAPPING_H_

#include <dragengine/resources/animator/deAnimatorInstanceReference.h>


class dedsLocomotion;


/**
 * \brief Locomotion controller mapping.
 */
class dedsLControllerMapping{
public:
	/** \brief Attributes animator instance controllers can be linked to. */
	enum eAttributes{
		/** \brief Elapsed time modified by play speed. */
		eaElapsedTime,
		
		/** \brief Look up and down. */
		eaLookVertical,
		
		/** \brief Look left and right. */
		eaLookHorizontal,
		
		/** \brief Moving velocity in meters per second. */
		eaMovingSpeed,
		
		/** \brief Moving direction relative to the view direction. */
		eaMovingDirection,
		
		/**
		 * \brief Relative moving speed.
		 * 
		 * Similar to eaMovingSpeed but negated if absolute eaMovingDirection
		 * is larger than 90 degrees.
		 */
		eaRelativeMovingSpeed,
		
		/** \brief Turning velocity in degrees per second. */
		eaTurningSpeed,
		
		/** \brief Stance. */
		eaStance,
		
		/** \brief Displacement in meters. */
		eaDisplacement,
		
		/** \brief Playback time turn-inplace. */
		eaTimeTurnIP,
		
		/** \brief Tilt offset. */
		eaTiltOffset,
		
		/** \brief Tilt up and down. */
		eaTiltVertical,
		
		/** \brief Tilt right and left. */
		eaTiltHorizontal,
		
		/**
		 * \brief Relative displacement in meters.
		 * 
		 * Same as eaDisplacement but negated if absolute eaMovingDirection
		 * is larger than 90 degrees.
		 */
		eaRelativeDisplacement
	};
	
	
	
private:
	/** \brief Animator instance. */
	deAnimatorInstanceReference pAnimatorInstance;
	
	/** \brief Index of controller in the animator. */
	int pController;
	
	/** \brief Attribute to assign to the controller. */
	eAttributes pAttribute;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create mapping. */
	dedsLControllerMapping();
	
	/** \brief Create mapping. */
	dedsLControllerMapping( const dedsLControllerMapping &mapping );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Animator instance to update or NULL if not used. */
	inline deAnimatorInstance *GetAnimatorInstance() const{ return pAnimatorInstance; }
	
	/** \brief Set animator instance to update or NULL if not used. */
	void SetAnimatorInstance( deAnimatorInstance *instance );
	
	/** \brief Controller. */
	inline int GetController() const{ return pController; }
	
	/** \brief Set controller. */
	void SetController( int controller );
	
	/** \brief Attribute. */
	inline eAttributes GetAttribute() const{ return pAttribute; }
	
	/** \brief Set attribute. */
	void SetAttribute( eAttributes attribute );
	
	
	
	/** \brief Apply mapping. */
	void Apply( const dedsLocomotion &locomotion, float elapsed );
	
	/** \brief Apply mapping for post locomotion only. */
	void ApplyPost( const dedsLocomotion &locomotion, float elapsed );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from other mapping. */
	dedsLControllerMapping &operator=( const dedsLControllerMapping &mapping );
	/*@}*/
};

#endif
