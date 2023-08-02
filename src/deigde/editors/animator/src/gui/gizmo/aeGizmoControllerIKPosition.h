/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
	aeGizmoControllerIKPosition( igdeEnvironment &environment, aeController &controller );
	
protected:
	/** Clean up gizmo. */
	virtual ~aeGizmoControllerIKPosition() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	virtual decDVector GetObjectPosition() override;
	virtual void SetObjectPosition( const decDVector &position ) override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	virtual void VisitElement( aeElementVisitor &visitor ) override;
	/*@}*/
};

#endif
