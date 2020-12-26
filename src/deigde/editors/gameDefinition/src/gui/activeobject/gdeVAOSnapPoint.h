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

#ifndef _GDEVAOSNAPPOINT_H_
#define _GDEVAOSNAPPOINT_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/debug/deDebugDrawerReference.h>

class gdeViewActiveObject;
class gdeOCSnapPoint;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;



/**
 * \brief Game definition active object snap point for edit view.
 */
class gdeVAOSnapPoint : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCSnapPoint *pOCSnapPoint;
	
	deDebugDrawerReference pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	igdeWDebugDrawerShape *pDDSSnapDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object snap point. */
	gdeVAOSnapPoint( gdeViewActiveObject &view, gdeOCSnapPoint *ocsnapPoint );
	
protected:
	/**
	 * \brief Clean up active object snap point.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOSnapPoint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class snap point. */
	inline gdeOCSnapPoint *GetOCSnapPoint() const{ return pOCSnapPoint; }
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDebugDrawer();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
