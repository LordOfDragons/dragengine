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

#ifndef _GDEVAOBILLBOARD_H_
#define _GDEVAOBILLBOARD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/billboard/deBillboardReference.h>
#include <dragengine/resources/collider/deColliderReference.h>

class gdeViewActiveObject;
class gdeOCBillboard;



/**
 * \brief Active object view object class billboard.
 */
class gdeVAOBillboard : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCBillboard *pOCBillboard;
	
	deBillboardReference pBillboard;
	deColliderReference pCollider;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object component. */
	gdeVAOBillboard( gdeViewActiveObject &view, gdeOCBillboard *ocbillboard );
	
protected:
	/**
	 * \brief Clean up active object component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOBillboard();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class billboard. */
	inline gdeOCBillboard *GetOCBillboard() const{ return pOCBillboard; }
	
	/** \brief Collider or \em NULL if not present. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Extends. */
	void GetExtends( decVector &minExtend, decVector &maxExtend ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateBillboard();
	void pCreateCollider();
	void pAttachBillboard();
	
	void pReleaseResources();
};

#endif
 
