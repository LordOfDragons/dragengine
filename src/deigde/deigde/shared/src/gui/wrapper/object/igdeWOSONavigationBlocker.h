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

#ifndef _IGDEWOSONAVIGATIONBLOCKER_H_
#define _IGDEWOSONAVIGATIONBLOCKER_H_

#include "igdeWOSubObject.h"

#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/collider/deCollider.h>


class deColliderAttachment;
class igdeGDCNavigationBlocker;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSONavigationBlocker : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSONavigationBlocker> Ref;
	
	
private:
	const igdeGDCNavigationBlocker &pGDNavigationBlocker;
	deNavigationBlocker::Ref pNavigationBlocker;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSONavigationBlocker( igdeWObject &wrapper, const igdeGDCNavigationBlocker &gdNavigationBlocker, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSONavigationBlocker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief NavigationBlocker resource. */
	inline deNavigationBlocker *GetNavigationBlocker() const{ return pNavigationBlocker; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Visit. */
	virtual void Visit( igdeWOSOVisitor &visitor );
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pUpdateNavigationBlocker();
	void pDestroyNavigationBlocker();
};

#endif
