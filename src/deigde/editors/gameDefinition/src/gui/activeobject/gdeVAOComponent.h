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

#ifndef _GDEVAOCOMPONENT_H_
#define _GDEVAOCOMPONENT_H_

#include "gdeVAOSubObject.h"

#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>

class gdeOCComponent;
class gdeOCComponentTexture;

class deComponentTexture;



/**
 * \brief Game definition active object component for edit view.
 */
class gdeVAOComponent : public gdeVAOSubObject{
private:
	gdeOCComponent *pOCComponent;
	
	deCollider::Ref pCollider;
	deComponent::Ref pComponent;
	deAnimatorInstance::Ref pAnimator;
	bool pPlayback;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create active object component. */
	gdeVAOComponent( gdeViewActiveObject &view, const gdeObjectClass &objectClass,
		const decString &propertyPrefix, gdeOCComponent *occomponent );
	
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
	
	/** \brief Component or \em NULL if not present. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Collider or \em NULL if not present. */
	inline deCollider *GetCollider() const{ return pCollider; }
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Rebuild resources. */
	virtual void RebuildResources();
	
	/** \brief Update texture. */
	void UpdateTexture( gdeOCComponentTexture *texture );
	
	/** \brief Selected object changed. */
	virtual void SelectedObjectChanged();
	
	/** \brief Extends. */
	virtual void GetExtends( decVector &minExtend, decVector &maxExtend ) const;
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
 
