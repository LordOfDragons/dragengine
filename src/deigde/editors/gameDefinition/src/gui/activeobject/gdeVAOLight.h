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

#ifndef _GDEVAOLIGHT_H_
#define _GDEVAOLIGHT_H_

#include "gdeVAOSubObject.h"

#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/light/deLight.h>

class gdeOCLight;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;

class deComponent;



/**
 * \brief Game definition active object light for edit view.
 * 
 * \todo Use DebugDrawerShape to visualize light cone.
 */
class gdeVAOLight : public gdeVAOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeVAOLight> Ref;
	
	
private:
	gdeOCLight::Ref pOCLight;
	
	deLight::Ref pLight;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object light. */
	gdeVAOLight(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCLight *oclight);
	
protected:
	/**
	 * \brief Clean up active object light.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~gdeVAOLight() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class light. */
	inline const gdeOCLight::Ref &GetOCLight() const{ return pOCLight; }
	
	/** \brief Update. */
	void Update(float elapsed);
	
	/** \brief Rebuild resources. */
	void RebuildResources() override;
	
	/** \brief Reattach resources. */
	void AttachResources();
	
	/** \brief Detach resources. */
	void DetachResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged() override;
	
	/** \brief Ignore component during shadow casting. */
	void ShadowIgnoreComponent(deComponent *component);
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
 
