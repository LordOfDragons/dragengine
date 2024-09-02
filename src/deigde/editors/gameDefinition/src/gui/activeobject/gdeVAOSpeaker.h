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

#ifndef _GDEVAOSPEAKER_H_
#define _GDEVAOSPEAKER_H_

#include "gdeVAOSubObject.h"

#include <dragengine/resources/debug/deDebugDrawerReference.h>

class gdeOCSpeaker;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;



/**
 * \brief Game definition active object speaker for edit view.
 */
class gdeVAOSpeaker : public gdeVAOSubObject{
private:
	gdeOCSpeaker *pOCSpeaker;
	
	deDebugDrawerReference pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object speaker. */
	gdeVAOSpeaker( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCSpeaker *ocspeaker );
	
protected:
	/**
	 * \brief Clean up active object speaker.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOSpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class speaker. */
	inline gdeOCSpeaker *GetOCSpeaker() const{ return pOCSpeaker; }
	
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
 
