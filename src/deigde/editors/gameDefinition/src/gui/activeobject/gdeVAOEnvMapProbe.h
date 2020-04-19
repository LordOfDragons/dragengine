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

#ifndef _GDEVAOENVMAPPROBE_H_
#define _GDEVAOENVMAPPROBE_H_

#include <dragengine/deObject.h>

class gdeViewActiveObject;
class gdeOCEnvMapProbe;
class igdeWDebugDrawerShape;

class deDebugDrawer;
class deEnvMapProbe;



/**
 * \brief Game definition active object environment map probe for edit view.
 */
class gdeVAOEnvMapProbe : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCEnvMapProbe *pOCEnvMapProbe;
	
	deEnvMapProbe *pEnvMapProbe;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWDebugDrawerShape *pDDSReflection;
	igdeWDebugDrawerShape *pDDSInfluence;
	igdeWDebugDrawerShape *pDDSMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object envMapProbe. */
	gdeVAOEnvMapProbe( gdeViewActiveObject &view, gdeOCEnvMapProbe *ocenvMapProbe );
	
protected:
	/**
	 * \brief Clean up active object envMapProbe.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOEnvMapProbe();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class envMapProbe. */
	inline gdeOCEnvMapProbe *GetOCEnvMapProbe() const{ return pOCEnvMapProbe; }
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Update debug drawer visibility. */
	void UpdateDDVisibility();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateEnvMapProbe();
	void pCreateDebugDrawer();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
