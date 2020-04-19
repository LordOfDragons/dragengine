/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEATTACHMENT_H_
#define _AEATTACHMENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class aeAnimator;

class igdeEnvironment;
class igdeWObject;

class deEngine;



/**
 * \brief Attach object.
 */
class aeAttachment : public deObject{
public:
	/** \brief Attach types. */
	enum eAttachTypes{
		/** \brief No attaching. */
		eatNone,
		
		/** \brief Attach to bone. */
		eatBone,
		
		/** \brief Attach to rig. */
		eatRig
	};
	
private:
	aeAnimator *pAnimator;
	
	igdeWObject *pObjectWrapper;
	
	decString pName;
	eAttachTypes pAttachType;
	decString pBoneName;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create attachment. */
	aeAttachment( igdeEnvironment *environment, const char *name = "Attachment" );
	
	/** \brief Clean up attachment. */
	virtual ~aeAttachment();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set parent animator. */
	void SetAnimator( aeAnimator *animator );
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Attach type. */
	inline eAttachTypes GetAttachType() const{ return pAttachType; }
	
	/** \brief Set attached type. */
	void SetAttachType( eAttachTypes type );
	
	/** \brief Name of the bone to attach to. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set name of the bone to attach to. */
	void SetBoneName( const char *name );
	
	
	
	/** \brief Update attachment. */
	void Update( float elapsed );
	
	/** \brief Reset attachment physics state. */
	void ResetPhysics();
	
	/** \brief Attach collider if possible. */
	void AttachCollider();
	
	/** \brief Dettach collider if attached. */
	void DetachCollider();
	
	
	
	/** \brief Object wrapper. */
	inline igdeWObject *GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
