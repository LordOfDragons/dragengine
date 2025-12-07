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

#ifndef _AEGIZMOCONTROLLERIKPOSITION_H_
#define _AEGIZMOCONTROLLERIKPOSITION_H_

#include "../../visitors/aeElementVisitable.h"

#include <deigde/gui/gizmo/igdeGizmoMove.h>

class aeController;



/**
 * Synchronize game definition task.
 */
class aeGizmoControllerIKPosition : public igdeGizmoMove, aeElementVisitable{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeGizmoControllerIKPosition> Ref;
	
	
private:
	aeController &pController;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create gizmo. */
	aeGizmoControllerIKPosition(igdeEnvironment &environment, aeController &controller);
	
protected:
	/** Clean up gizmo. */
	virtual ~aeGizmoControllerIKPosition() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	virtual decDVector GetObjectPosition() override;
	virtual void SetObjectPosition(const decDVector &position) override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	virtual void VisitElement(aeElementVisitor &visitor) override;
	/*@}*/
};

#endif
