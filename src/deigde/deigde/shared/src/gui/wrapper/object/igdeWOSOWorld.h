/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEWOSOWORLD_H_
#define _IGDEWOSOWORLD_H_

#include "igdeWOSubObject.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCWorld;


/**
 * \brief Object wrapper sub world.
 */
class DE_DLL_EXPORT igdeWOSOWorld : public igdeWOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWOSOWorld> Ref;
	
	
private:
	const igdeGDCWorld &pGDWorld;
	decString pPathWorld;
	decDVector pPosition;
	decQuaternion pOrientation;
	decObjectOrderedSet pObjects;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub world. */
	igdeWOSOWorld(igdeWObject &wrapper, const igdeGDCWorld &gdcWorld, const decString &prefix);
	
	/** \brief Clean up object wrapper sub world. */
	~igdeWOSOWorld() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Update parameters. */
	void UpdateParameters() override;
	
	/** \brief All sub components finished loading. */
	void OnAllSubObjectsFinishedLoading() override;
	
	/** \brief Update visibility. */
	void UpdateVisibility() override;
	
	/** \brief Visit. */
	void Visit(igdeWOSOVisitor &visitor) override;
	/*@}*/
	
	
protected:
	void AttachToCollider() override;
	void DetachFromCollider() override;
	
	
private:
	void pLoadWorld(const decString &path);
	void pUpdateWorld();
	void pDestroyWorld();
};

#endif
