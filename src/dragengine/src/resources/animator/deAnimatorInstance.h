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

#ifndef _DEANIMATORINSTANCE_H_
#define _DEANIMATORINSTANCE_H_

#include "rule/deAnimatorRule.h"
#include "deAnimatorReference.h"
#include "../deResource.h"
#include "../animation/deAnimationReference.h"
#include "../component/deComponentReference.h"
#include "../../common/math/decMath.h"

class deBaseScriptingCollider;
class deTouchSensor;
class deAnimatorInstanceManager;
class deAnimatorController;
class deBaseAnimatorAnimatorInstance;


/**
 * \brief Animator Instance Class.
 * 
 * Animators are objects capable of producing an animation state for a
 * component. Every animator composes of a set of rules and controllers
 * which indicate how the final animation state is produced from a set
 * animation. While animators define this production process the animator
 * instance is required to actually apply an animator to a component. This
 * is done for two main reasons. First animators can be shared this way
 * across multiple components without having to create a unique copy for
 * each component. This allows the game to organize animations more
 * efficiently. The second reason is the simple support for animation
 * retargeting. If the rig specified in the animator does not match the
 * rig in the component used in the instance automatic retargeting is
 * conducted. This way the user does not have to think about specifying
 * retargeting explicitely nor does he have to worry about when this is
 * required. The same animator can therefore be used for all kinds of
 * components as long as the rigs used in the components are similar
 * enough to the rig used in the animator (not a requirement but for good
 * results a good practise). More than one animator can act on one component
 * one after the other if complex animations are required that can not be
 * done using one single animator. Animators can not be used at the same
 * time as a collider component if the rig of the component is driven by
 * physics. Animators can though be used if the rig is kinematic. 
 */
class deAnimatorInstance : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorInstance> Ref;
	
	
	
private:
	deAnimatorReference pAnimator;
	deComponentReference pComponent;
	deAnimationReference pAnimation;
	
	deAnimatorRule::eBlendModes pBlendMode;
	float pBlendFactor;
	bool pEnableRetargeting;
	bool pProtectDynamicBones;
	
	deAnimatorController *pControllers;
	int pControllerCount;
	int pControllerSize;
	
	deBaseAnimatorAnimatorInstance *pPeerAnimator;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new animator instance.
	 * \throws deeInvalidParam manager is NULL.
	 */
	deAnimatorInstance( deAnimatorInstanceManager *manager );
	
protected:
	/**
	 * \brief Clean up the animator instance.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Animator or NULL if none is set. */
	inline deAnimator *GetAnimator() const{ return pAnimator; }
	
	/**
	 * \brief Set animator or NULL to unset it.
	 * \param[in] keepValues If true keeps all controller values unchanged which have the same
	 * name in the old and new animator. Otherwise initializes all
	 * controllers to the value of the new animator.
	 */
	void SetAnimator( deAnimator *animator, bool keepValues = false );
	
	/** \brief Component or NULL if none is set. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Set component or NULL to unset it. */
	void SetComponent( deComponent *component );
	
	/** \brief Animation or NULL if none is set. */
	inline deAnimation *GetAnimation() const{ return pAnimation; }
	
	/** \brief Set animation or NULL to unset it. */
	void SetAnimation( deAnimation *animation );
	
	
	
	/** \brief Blend mode. */
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/**
	 * \brief Set blend mode.
	 * \throws deeInvalidParam \em mode is not a member of deAnimatorRule::eBlendModes.
	 */
	void SetBlendMode( deAnimatorRule::eBlendModes mode );
	
	/** \brief Blend factor in the range of 0 to 1. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** \brief Set blend factor in the range of 0 to 1. */
	void SetBlendFactor( float factor );
	
	/** \brief Enable retargeting. */
	inline bool GetEnableRetargeting() const{ return pEnableRetargeting; }
	
	/** \brief Set if retargeting is enabled. */
	void SetEnableRetargeting( bool enableRetargeting );
	
	/** \brief Protect dynamic bones from being animated. */
	inline bool GetProtectDynamicBones() const{ return pProtectDynamicBones; }
	
	/** \brief Set if dynamic bones are protected from being animated. */
	void SetProtectDynamicBones( bool protectDynamicBones );
	
	
	
	/** \brief Number of controllers. */
	inline int GetControllerCount() const{ return pControllerCount; }
	
	/**
	 * \brief Controller at index.
	 * \throws deeInvalidParam \em index is less than 0.
	 * \throws deeInvalidParam \em index is greater or equal than GetControllerCount().
	 */
	deAnimatorController &GetControllerAt( int index );
	const deAnimatorController &GetControllerAt( int index ) const;
	
	/** \brief Index of named controller or -1 if absent. */
	int IndexOfControllerNamed( const char *name ) const;
	
	/** \brief Notify peer controller changed. */
	void NotifyControllerChangedAt( int index );
	
	
	
	/**
	 * \brief Apply state of animator to the component if existing.
	 * 
	 * Animator modules can decide to calculate this in parallel. If
	 * \em direct is true the application is always done
	 * synchronously and is done after the call returns. If \em direct
	 * is false the calculation can be parallel. The affected
	 * resources take care of waiting for the result to become ready
	 * if required.
	 */
	void Apply( bool direct = false );
	
	/**
	 * \brief Capture current state into snapshot rules matching identifier
	 */
	void CaptureStateInto( int identifier );
	
	/**
	 * \brief Store animation frame from animation into into rules matching identifier.
	 * 
	 * If \em moveName does not exist in the animation a default state is captured.
	 * 
	 * \throws deeInvalidParam \em moveName is NULL.
	 */
	void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Animator peer or NULL if not set. */
	inline deBaseAnimatorAnimatorInstance *GetPeerAnimator() const{ return pPeerAnimator; }
	
	/** \brief Set animator peer or NULL if not set. */
	void SetPeerAnimator( deBaseAnimatorAnimatorInstance *peer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateControllers();
};

#endif
