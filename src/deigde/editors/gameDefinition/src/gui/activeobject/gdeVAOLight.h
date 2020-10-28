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

#ifndef _GDEVAOLIGHT_H_
#define _GDEVAOLIGHT_H_

#include <dragengine/deObject.h>

class gdeViewActiveObject;
class gdeOCLight;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;

class deComponent;
class deDebugDrawer;
class deLight;



/**
 * \brief Game definition active object light for edit view.
 * 
 * \todo Use DebugDrawerShape to visualize light cone.
 */
class gdeVAOLight : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCLight *pOCLight;
	
	deLight *pLight;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object light. */
	gdeVAOLight( gdeViewActiveObject &view, gdeOCLight *oclight );
	
protected:
	/**
	 * \brief Clean up active object light.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOLight();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class light. */
	inline gdeOCLight *GetOCLight() const{ return pOCLight; }
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Reattach resources. */
	void AttachResources();
	
	/** \brief Detach resources. */
	void DetachResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Ignore component during shadow casting. */
	void ShadowIgnoreComponent( deComponent *component );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateLight();
	void pCreateDebugDrawer();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
