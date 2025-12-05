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

#ifndef _AEATTACHMENT_H_
#define _AEATTACHMENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

#include <deigde/gui/wrapper/igdeWObject.h>

class aeAnimator;

class igdeEnvironment;

class deEngine;



/**
 * Attach object.
 */
class aeAttachment : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeAttachment> Ref;


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
	
	igdeWObject::Ref pObjectWrapper;
	
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
	inline const igdeWObject::Ref &GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
