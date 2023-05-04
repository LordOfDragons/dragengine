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
 * Attach object.
 */
class aeAttachment : public deObject{
public:
	/** Attach types. */
	enum eAttachTypes{
		/** No attaching. */
		eatNone,
		
		/** Attach to bone. */
		eatBone,
		
		/** Attach to rig. */
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
	/** Create attachment. */
	aeAttachment( igdeEnvironment *environment, const char *name = "Attachment" );
	
	/** Clean up attachment. */
	virtual ~aeAttachment();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set parent animator. */
	void SetAnimator( aeAnimator *animator );
	
	
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Attach type. */
	inline eAttachTypes GetAttachType() const{ return pAttachType; }
	
	/** Set attached type. */
	void SetAttachType( eAttachTypes type );
	
	/** Name of the bone to attach to. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** Set name of the bone to attach to. */
	void SetBoneName( const char *name );
	
	
	
	/** Update attachment. */
	void Update( float elapsed );
	
	/** Reset attachment physics state. */
	void ResetPhysics();
	
	/** Attach collider if possible. */
	void AttachCollider();
	
	/** Dettach collider if attached. */
	void DetachCollider();
	
	
	
	/** Object wrapper. */
	inline igdeWObject *GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
