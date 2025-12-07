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

#ifndef _DECOLLIDERCOMPONENT_H_
#define _DECOLLIDERCOMPONENT_H_

#include "deColliderRig.h"
#include "../component/deComponent.h"

class deCollisionInfo;


/**
 * \brief Component collider.
 *
 * Colliders are used by the physics module to provide collision
 * detection and optional collision response. After the collision
 * detection stage is done each Collider contains the new state
 * of motion. The user can then update the game state depending
 * on the changes. The scripting peer receives all relevant events
 * during a collision detection run. The Collider is defined by a
 * Component object. The Component object contains a Model and a
 * Rig object. The Rig object defines the bones and the physical
 * properties but the collision volumes are ignored. Instead the
 * triangle mesh stored in the Model is used as collision volume.
 * Optional a Collision Model object can be assigned to be used
 * instead of the Model object providing more a optimized
 * collision volume.
 */
class DE_DLL_EXPORT deColliderComponent : public deColliderRig{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deColliderComponent> Ref;
	
	
private:
	deComponent::Ref pComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create component collider. */
	deColliderComponent(deColliderManager *manager);
	
protected:
	/**
	 * \brief Clean up component collider.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deColliderComponent();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Component or NULL if not set. */
	inline deComponent *GetComponent() const{return pComponent;}
	
	/**
	 * \brief Set component or NULL if not set.
	 * 
	 * Sets also the rig ro the component rig or NULL. If the rig in
	 * the component changes SetRig() has to be called to synchronize
	 * the component collider with the changed situation.
	 * \note Component does not track in what collider it is used. The physics
	 * module has to track this and call SetRig() whenever a rig change
	 * in the component is detected.
	 */
	void SetComponent(deComponent *component);
	
	
	
	/**
	 * \brief Copy bone states of the bones from the component if set.
	 * 
	 * If the component is not set nothing is done at all. All linear and
	 * angular velocities are set to zero.
	 */
	void CopyStatesFromComponent();
	
	/**
	 * \brief Copy a bone state from the component bone if set.
	 * 
	 * If the component is not set nothing is done at all. All linear and
	 * angular velocities are set to zero.
	 */
	void CopyStateFromComponent(int bone);
	
	/**
	 * \brief Copy bone states to component bones if set.
	 * 
	 * If the component is not set nothing is done at all.
	 */
	void CopyStatesToComponent() const;
	
	/**
	 * \brief Copy bone state to component bone if set.
	 * 
	 * If the component is not set nothing is done at all.
	 */
	void CopyStateToComponent(int bone) const;
	
	
	
	/**
	 * \brief Set up weight based attachment parameters.
	 * 
	 * Before calling set up attachment position and orientation relative to the component
	 * using attachment.SetPosition() and attachment.SetOrientation() as if static attachment
	 * is used. This is required to properly calculate the weighting. After this call finishes
	 * the position and orientation usually changed.
	 * 
	 * \param[in] attachment Attachment to modify.
	 * \param[in] face Index of face. Typically comes from deCollisionInfo::GetFace().
	 * \throws EInvalidParam Face index is outside allowed boundaries.
	 */
	void InitWeightAttachment(deColliderAttachment &attachment, int face);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit collider. */
	virtual void Visit(deColliderVisitor &visitor);
	/*@}*/
};

#endif
