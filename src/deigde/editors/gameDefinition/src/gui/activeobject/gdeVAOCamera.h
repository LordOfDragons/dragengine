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

#ifndef _GDEVAOCAMERA_H_
#define _GDEVAOCAMERA_H_

#include <dragengine/deObject.h>

class gdeViewActiveObject;
class gdeOCCamera;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;

class deDebugDrawer;



/**
 * \brief Game definition active object camera for edit view.
 */
class gdeVAOCamera : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCCamera *pOCCamera;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object camera. */
	gdeVAOCamera( gdeViewActiveObject &view, gdeOCCamera *occamera );
	
protected:
	/**
	 * \brief Clean up active object camera.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOCamera();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class camera. */
	inline gdeOCCamera *GetOCCamera() const{ return pOCCamera; }
	
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
 
