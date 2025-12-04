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

#ifndef _DEDSLCONTROLLERMAPPING_H_
#define _DEDSLCONTROLLERMAPPING_H_

#include <dragengine/resources/animator/deAnimatorInstance.h>


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
	deAnimatorInstance::Ref pAnimatorInstance;
	
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
