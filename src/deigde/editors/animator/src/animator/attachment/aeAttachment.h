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
#include <deigde/meta/igdeMetaContext.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>

class aeAnimator;
class aeWindowMain;

class igdeEnvironment;

class deEngine;



/**
 * Attach object.
 */
class aeAttachment : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<aeAttachment>;
	
	/** \brief List type. */
	using List = decTCollectionQueryByName<decTObjectOrderedSet<aeAttachment>, aeAttachment>;
	
	
	/** Attach types. */
	enum eAttachTypes{
		/** No attaching. */
		eatNone,
		
		/** Attach to bone. */
		eatBone,
		
		/** Attach to rig. */
		eatRig
	};
	
	class cAsyncLoadListener : public igdeWObject::cAsyncLoadFinished{
	private:
		aeAttachment &pAttachment;
		
	public:
		cAsyncLoadListener(aeAttachment &attachment);
		void LoadFinished(igdeWObject &wrapper, bool succeeded) override;
	};
	
	using MetaContext = igdeMetaContextType<aeAttachment>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeAttachment *attachment);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	MetaContext::Ref pMetaContext;
	
	aeAnimator *pAnimator;
	
	cAsyncLoadListener pAsyncLoadListener;
	igdeWObject::Ref pObjectWrapper;
	
	igdeMetaPropertyStringStorage::Storage pMPName;
	igdeMetaPropertySelectionEnumStorage<eAttachTypes>::Storage pMPAttachType;
	igdeMetaPropertyStringStorage::Storage pMPBoneName;
	igdeMetaPropertyContextStorage::Storage pMPWObject;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create attachment. */
	explicit aeAttachment(aeWindowMain &windowMain, const char *name = "Attachment");
	
	/** Create copy of attachment. */
	aeAttachment(aeWindowMain &windowMain, const aeAttachment &copy);
	
	aeAttachment(const aeAttachment &copy) = delete;
	aeAttachment(aeAttachment &&copy) = delete;
	aeAttachment &operator=(const aeAttachment &copy) = delete;
	aeAttachment &operator=(aeAttachment &&copy) = delete;
	
protected:
	/** Clean up attachment. */
	~aeAttachment() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Get meta context. */
	inline MetaContext::Ref GetMetaContext() const{ return pMetaContext; }
	
	inline igdeMetaPropertyStringStorage::Storage &GetMPName(){ return pMPName; }
	inline igdeMetaPropertySelectionEnumStorage<eAttachTypes>::Storage &GetMPAttachType(){ return pMPAttachType; }
	inline igdeMetaPropertyStringStorage::Storage &GetMPBoneName(){ return pMPBoneName; }
	inline igdeMetaPropertyContextStorage::Storage &GetMPWObject(){ return pMPWObject; }
	
	
	/** Parent animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	aeAnimator &GetAnimatorRef() const;
	
	/** Set parent animator. */
	void SetAnimator(aeAnimator *animator);
	
	igdeEnvironment &GetEnvironment() const;
	igdeUndoSystem *GetUndoSystem() const;
	
	
	/** Name. */
	inline const decString &GetName() const{ return pMPName; }
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Attach type. */
	inline eAttachTypes GetAttachType() const{ return pMPAttachType; }
	
	/** Set attached type. */
	void SetAttachType(eAttachTypes type);
	
	/** Name of the bone to attach to. */
	inline const decString &GetBoneName() const{ return pMPBoneName; }
	
	/** Set name of the bone to attach to. */
	void SetBoneName(const char *name);
	
	
	
	/** Update attachment. */
	void Update(float elapsed);
	
	/** Reset attachment physics state. */
	void ResetPhysics();
	
	/** Attach collider if possible. */
	void AttachCollider();
	
	/** Dettach collider if attached. */
	void DetachCollider();
	
	/** Reattach collider. */
	void ReattachCollider();
	
	
	
	/** Object wrapper. */
	inline const igdeWObject::Ref &GetObjectWrapper() const{ return pObjectWrapper; }
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
