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
#include "../../../resourceloader/igdeResourceLoaderListener.h"

#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/collider/deCollider.h>


class deColliderAttachment;
class igdeGDCBillboard;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOBillboard : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOBillboard> Ref;
	
	
private:
	const igdeGDCBillboard &pGDBillboard;
	deBillboard::Ref pBillboard;
	igdeResourceLoaderListener::Ref pResLoad;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	decVector2 pBaseOffset;
	bool pRenderEnvMap;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOBillboard(igdeWObject &wrapper, const igdeGDCBillboard &gdBillboard, const decString &prefix);
	
	/** \brief Clean up object wrapper sub object. */
	~igdeWOSOBillboard() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Billboard resource. */
	inline deBillboard *GetBillboard() const{ return pBillboard; }
	
	/** \brief Update parameters. */
	void UpdateParameters() override;
	
	/** \brief All sub components finished loading. */
	void OnAllSubObjectsFinishedLoading() override;
	
	/** \brief Geometry changed. */
	void UpdateGeometry() override;
	
	/** \brief Update visibility. */
	void UpdateVisibility() override;
	
	/** \brief Layer masks changed. */
	void UpdateLayerMasks() override;
	
	/** \brief Camera changed. */
	void CameraChanged() override;
	
	/** \brief Frame update. */
	void Update(float elapsed) override;
	
	/** \brief Visit. */
	void Visit(igdeWOSOVisitor &visitor) override;
	
	/** \brief For internal use only. */
	void AsyncLoadFinished(bool success);
	
	/**
	 * \brief Sub object is visible.
	 * 
	 * Returns true if wrapped resources is visible. For example a component resource without
	 * valid model or with no texture containing a valid skin is invisible. This check does not
	 * include the visibility state of the object. Hence invisible content is still considered
	 * visible if it would be visible otherwise.
	 */
	bool IsContentVisible() override;
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
