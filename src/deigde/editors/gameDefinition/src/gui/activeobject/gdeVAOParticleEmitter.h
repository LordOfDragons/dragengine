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

#ifndef _GDEVAOPARTICLEEMITTER_H_
#define _GDEVAOPARTICLEEMITTER_H_

#include "gdeVAOSubObject.h"

#include <dragengine/resources/debug/deDebugDrawerReference.h>
#include <dragengine/resources/particle/deParticleEmitterReference.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceReference.h>

class gdeOCParticleEmitter;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;



/**
 * \brief Game definition active object particleEmitter for edit view.
 */
class gdeVAOParticleEmitter : public gdeVAOSubObject{
private:
	gdeOCParticleEmitter *pOCParticleEmitter;
	
	deParticleEmitterReference pEmitter;
	deParticleEmitterInstanceReference pInstance;
	
	deDebugDrawerReference pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object particleEmitter. */
	gdeVAOParticleEmitter( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCParticleEmitter *ocemitter );
	
protected:
	/**
	 * \brief Clean up active object particleEmitter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class particle emitter. */
	inline gdeOCParticleEmitter *GetOCParticleEmitter() const{ return pOCParticleEmitter; }
	
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
	void pCreateParticleEmitter();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
