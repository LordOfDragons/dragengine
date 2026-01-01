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

#ifndef _GDEVAOSNAPPOINT_H_
#define _GDEVAOSNAPPOINT_H_

#include "gdeVAOSubObject.h"
#include "../../gamedef/objectClass/snappoint/gdeOCSnapPoint.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/resources/debug/deDebugDrawer.h>

class igdeWCoordSysArrows;



/**
 * \brief Game definition active object snap point for edit view.
 */
class gdeVAOSnapPoint : public gdeVAOSubObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeVAOSnapPoint> Ref;
	
	
private:
	gdeOCSnapPoint::Ref pOCSnapPoint;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape::Ref pDDSCenter;
	igdeWCoordSysArrows *pDDSCoordSystem;
	igdeWDebugDrawerShape::Ref pDDSSnapDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object snap point. */
	gdeVAOSnapPoint(gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCSnapPoint *ocsnapPoint);
	
protected:
	/**
	 * \brief Clean up active object snap point.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	~gdeVAOSnapPoint() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class snap point. */
	inline const gdeOCSnapPoint::Ref &GetOCSnapPoint() const{ return pOCSnapPoint; }
	
	/** \brief Rebuild resources. */
	void RebuildResources() override;
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDebugDrawer();
	void pUpdateDDShapes();
	void pUpdateDDShapeColor();
	
	void pReleaseResources();
};

#endif
 
