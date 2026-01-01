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

#ifndef _IGDEWOSOENVMAPPROBE_H_
#define _IGDEWOSOENVMAPPROBE_H_

#include "igdeWOSubObject.h"

#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/collider/deCollider.h>


class deColliderAttachment;
class igdeGDCEnvMapProbe;


/**
 * \brief Object wrapper sub object.
 */
class DE_DLL_EXPORT igdeWOSOEnvMapProbe : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOEnvMapProbe> Ref;
	
	
private:
	const igdeGDCEnvMapProbe &pGDEnvMapProbe;
	deEnvMapProbe::Ref pEnvMapProbe;
	bool pAddedToWorld;
	deCollider::Ref pAttachedToCollider;
	deColliderAttachment *pAttachment;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object. */
	igdeWOSOEnvMapProbe(igdeWObject &wrapper, const igdeGDCEnvMapProbe &gdEnvMapProbe, const decString &prefix);
	
protected:
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOEnvMapProbe();
	
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief EnvMapProbe resource. */
	inline const deEnvMapProbe::Ref &GetEnvMapProbe() const{ return pEnvMapProbe; }
	
	/** \brief Update parameters. */
	virtual void UpdateParameters();
	
	/** \brief Layer masks changed. */
	virtual void UpdateLayerMasks();
	
	/** \brief All sub components finished loading. */
	virtual void OnAllSubObjectsFinishedLoading();
	
	/** \brief Visit. */
	virtual void Visit(igdeWOSOVisitor &visitor);
	/*@}*/
	
	
	
protected:
	void AttachToCollider();
	void DetachFromCollider();
	
	
	
private:
	void pUpdateEnvMapProbe();
	void pDestroyEnvMapProbe();
};

#endif
