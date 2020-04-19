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

#ifndef _GDEVAOCOMPONENT_H_
#define _GDEVAOCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/component/deComponentReference.h>
#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/animator/deAnimatorInstanceReference.h>

class gdeViewActiveObject;
class gdeOCComponent;
class gdeOCComponentTexture;

class deComponentTexture;



/**
 * \brief Game definition active object component for edit view.
 */
class gdeVAOComponent : public deObject{
private:
	gdeViewActiveObject &pView;
	
	gdeOCComponent *pOCComponent;
	
	deColliderReference pCollider;
	deComponentReference pComponent;
	deAnimatorInstanceReference pAnimator;
	bool pPlayback;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object component. */
	gdeVAOComponent( gdeViewActiveObject &view, gdeOCComponent *occomponent );
	
protected:
	/**
	 * \brief Clean up active object component.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~gdeVAOComponent();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object class component. */
	inline gdeOCComponent *GetOCComponent() const{ return pOCComponent; }
	
	/** \brief Collider or \em NULL if not present. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Rebuild resources. */
	void RebuildResources();
	
	/** \brief Update texture. */
	void UpdateTexture( gdeOCComponentTexture *texture );
	
	/** \brief Selected object changed. */
	void SelectedObjectChanged();
	
	/** \brief Extends. */
	void GetExtends( decVector &minExtend, decVector &maxExtend ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateComponent();
	void pCreateComponentTextures();
	void pUpdateComponentTexture( const gdeOCComponentTexture *texture,
		deComponentTexture &engTexture, int engTextureIndex );
	void pCreateCollider();
	void pCreateAnimator();
	void pAttachComponent();
	
	void pReleaseResources();
};

#endif
 
