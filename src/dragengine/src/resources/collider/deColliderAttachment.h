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

#ifndef _DECOLLIDERATTACHEMENT_H_
#define _DECOLLIDERATTACHEMENT_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Attach resources to a collider.
 * 
 * Stores information about a resource attached to a a collider.
 * The following resource types can be attached:
 * - deResourceManager::ertBillboard
 * - deResourceManager::ertCamera
 * - deResourceManager::ertCollider
 * - deResourceManager::ertComponent
 * - deResourceManager::ertDebugDrawer
 * - deResourceManager::ertEnvMapProbe
 * - deResourceManager::ertForceField
 * - deResourceManager::ertLight
 * - deResourceManager::ertLumimeter
 * - deResourceManager::ertMicrophone
 * - deResourceManager::ertNavigationSpace
 * - deResourceManager::ertParticleEmitterInstance
 * - deResourceManager::ertPropField
 * - deResourceManager::ertSpeaker
 * - deResourceManager::ertTouchSensor
 * 
 * All other resource types will generate an exception if attempted to be attached.
 * Resources can be attached using different modes. Not all of them are supported
 * for all resource types. See deColliderAttachment::aAttachType for details.
 * 
 * In the case of attached collider resource of type component or rig a source bone
 * can be specified. If such a bone is defined it is used to attach the collider
 * instead of the collider origin itself. For all other resource types this is not used.
 */
class DE_DLL_EXPORT deColliderAttachment{
public:
	/** \brief Attachment types. */
	enum eAttachType{
		 /**
		  * \brief Attach to collider origin.
		  * 
		  * Supported for all resources on all collider types.
		  */
		eatStatic,
		
		/**
		 * \brief Attach to a rig.
		 * 
		 * Supported on colliders of type component or rig if a rig is set. Supported
		 * only for resources of type collider of type component or rig. All other
		 * resources are treated like deColliderAttachment::eatStatic. For the supported
		 * resource the state of each target bone is copied to the attached bone with
		 * the same name. Not matched bones do not get their state changed.
		 */
		eatRig,
		
		/**
		 * \brief Attach to a bone.
		 * 
		 * Supported on colliders of type component or rig if a rig is set. Supported
		 * on all resource types. The resource is attached to the bone coordinate frame.
		 */
		eatBone,
		
		/**
		 * \brief Attach to multiple bones.
		 * 
		 * Supported on colliders of type component or rig if a rig is set. Supported
		 * on all resource types. The resource is attached to the coordinate frame
		 * calculate by weighting the coordinate frames of multiple bones. With one
		 * bone this behaves the same as deColliderAttachment::eatBone. With no bone
		 * this behaves the same as deColliderAttachment::eatStatic.
		 */
		eatWeight,
		
		/**
		 * \brief Attach with relative movement like riding a platform.
		 * 
		 * Supported for all resources on all collider types. The position of the
		 * attached resource is kept relative to the target collider. Movement of the
		 * resource relative to the collider is carried over allowing to use this
		 * attachment mode to simulate objects riding on other objects where physics
		 * is not a suitable option. This is the typical choice for simulating actors
		 * riding on platforms.
		 */
		eatRelativeMovement
	};
	
	/** \brief Weighted bone used for eatBone. */
	struct DE_DLL_EXPORT sWeight{
		/** \brief Name of the bone to match with the target bone. */
		decString bone;
		/** \brief Bone weight in the range from 0 to 1. */
		float weight;
		/** \brief Constructor. */
		inline sWeight() : weight( 0.0f ){}
	};
	
	
	
private:
	eAttachType pAttachType;
	deResource::Ref pResource;
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	bool pNoScaling;
	decString pTrackBone;
	sWeight *pWeights;
	int pWeightCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create a new attachement.
	 * 
	 * The new attachment holds a reference to the resource.
	 * The attachment mode is set to deColliderAttachment::eatStatic.
	 * 
	 * \throws EInvalidParam \em resource is NULL.
	 * \throws EInvalidParam CanAttachResource() returns false for \em resource.
	 */
	deColliderAttachment( deResource *resource );
	
	/** \brief Clean up the collider attachement object. */
	~deColliderAttachment();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Attach type. */
	inline eAttachType GetAttachType() const{ return pAttachType; }
	
	/**
	 * \brief Set attach type.
	 * \throws EInvalidParam \em attachType does not match any of deColliderAttachment::eAttachType.
	 */
	void SetAttachType( eAttachType attachType );
	
	/** \brief Attached resource. */
	inline deResource *GetResource() const{ return pResource; }
	
	
	
	/** \brief Position of the attached resource relative to the attachment point. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position of the attached resource relative to the attachment point. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation of the attached resource relative to the attachment point. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation of the attached resource relative to the attachment point. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling of the attached resource relative to the attachment point. */
	inline const decVector &GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling of the attached resource relative to the attachment point. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Ignore scaling. */
	inline bool GetNoScaling() const{ return pNoScaling; }
	
	/** \brief Set if scaling is ignored. */
	void SetNoScaling( bool noScaling );
	
	
	
	/** \brief Name of the bone to track in the target collider or null if not used. */
	inline const decString &GetTrackBone() const{ return pTrackBone; }
	
	/**
	 * \brief Set name of the bone to track in the target collider or an empty string if not used.
	 * \throws EInvalidParam \em bone is NULL.
	 */
	void SetTrackBone( const char *bone );
	
	/** \brief Count of bone weights. */
	inline int GetWeightCount() const{ return pWeightCount; }
	
	/** \brief Pointer to array of bone weights. */
	inline sWeight *GetWeights() const{ return pWeights; }
	
	/**
	 * \brief Set number of bone weights.
	 * \throws EInvalidParam \em count is less than 0.
	 */
	void SetWeightCount( int count );
	
	
	
	/**
	 * \brief Determine if a resource is valid for attaching.
	 * 
	 * Creating an attachment fails if \em CanAttachResource returns false.
	 */
	static bool CanAttachResource( const deResource &resource );
	/*@}*/
};

#endif
