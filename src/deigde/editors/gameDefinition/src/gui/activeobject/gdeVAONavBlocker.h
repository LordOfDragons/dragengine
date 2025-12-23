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

#ifndef _GDEVAONAVBLOCKER_H_
#define _GDEVAONAVBLOCKER_H_

#include "gdeVAOSubObject.h"
#include "../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/resources/debug/deDebugDrawer.h>



/**
 * \brief Active object view object class navigation blocker.
 */
class gdeVAONavBlocker : public gdeVAOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeVAONavBlocker> Ref;
	
	
private:
	gdeOCNavigationBlocker::Ref pOCNavBlocker;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape::Ref pDDSBlocker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object component. */
	gdeVAONavBlocker(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCNavigationBlocker *occomponent);
	
protected:
	/**
	 * \brief Clean up active object component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~gdeVAONavBlocker() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class navigation blocker. */
	inline const gdeOCNavigationBlocker::Ref &GetOCNavBlocker() const{ return pOCNavBlocker; }
	
	/** \brief Rebuild resources. */
	void RebuildResources() override;
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged() override;
	
	/** \brief Update debug drawer visibility. */
	void UpdateDDVisibility();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDebugDrawer();
	void pBuildDDSBlocker();
	void pUpdateDDShapeColor();
};

#endif
 
