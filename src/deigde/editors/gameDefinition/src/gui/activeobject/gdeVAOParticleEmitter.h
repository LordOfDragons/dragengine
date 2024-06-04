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
 
