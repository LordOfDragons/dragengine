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

#ifndef _IGDEWOSOBILLBOARD_H_
#define _IGDEWOSOBILLBOARD_H_

#include "igdeWOSubObject.h"
#include "../../../resourceloader/igdeResourceLoaderListenerReference.h"

#include <dragengine/resources/billboard/deBillboardReference.h>
#include <dragengine/resources/collider/deColliderReference.h>


class deColliderAttachment;
class igdeGDCBillboard;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOBillboard : public igdeWOSubObject{
private:
	const igdeGDCBillboard &pGDBillboard;
	deBillboardReference pBillboard;
	igdeResourceLoaderListenerReference pResLoad;
	bool pAddedToWorld;
	deColliderReference pAttachedToCollider;
	deColliderAttachment *pAttachment;
	decVector2 pBaseOffset;
	bool pRenderEnvMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOBillboard( igdeWObject &wrapper, const igdeGDCBillboard &gdBillboard, const decString &prefix );
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Billboard resource. */
	inline deBillboard *GetBillboard() const{ return pBillboard; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Geometry changed. */
	virtual void UpdateGeometry();
	
	/** \brief Update visibility. */
	virtual void UpdateVisibility();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief Camera changed. */
	virtual void CameraChanged();
	
	/** \brief Frame update. */
	virtual void Update( float elapsed );
	
	/** \brief Visit. */
	virtual void Visit( igdeWOSOVisitor &visitor );
	
	/** \brief For internal use only. */
	void AsyncLoadFinished( bool success );
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pLoadResources();
	void pUpdateBillboard();
	void pDestroyBillboard();
	void pUpdateExtends();
};

#endif
