/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEVAOFORCEFIELD_H_
#define _GDEVAOFORCEFIELD_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/debug/deDebugDrawerReference.h>
#include <dragengine/resources/forcefield/deForceFieldReference.h>

class gdeViewActiveObject;
class gdeOCForceField;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;


/**
 * \brief Game definition active object force field for edit view.
 */
class gdeVAOForceField : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCForceField *pOCForceField;
	
	deForceFieldReference pForceField;
	
	deDebugDrawerReference pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object forceField. */
	gdeVAOForceField( gdeViewActiveObject &view, gdeOCForceField *ocfield );
	
protected:
	/**
	 * \brief Clean up active object forceField.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOForceField();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class particle emitter. */
	inline gdeOCForceField *GetOCForceField() const{ return pOCForceField; }
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Reattach resources. */
	void AttachResources();
	
	/** \brief Detach resources. */
	void DetachResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDebugDrawer();
	void pCreateForceField();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
