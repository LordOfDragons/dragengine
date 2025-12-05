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

#ifndef _MEATTACHEDDECAL_H_
#define _MEATTACHEDDECAL_H_

#include <dragengine/resources/decal/deDecal.h>

class meDecal;
class meObject;

class deEngine;
class deSkin;



/**
 * \brief Attached Decal.
 */
class meAttachedDecal{
private:
	deEngine *pEngine;
	deDecal::Ref pEngDecal;
	
	meDecal *pDecal;
	meObject *pParentObject;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create attached decal. */
	meAttachedDecal( deEngine *engine, meDecal *decal );
	
	/** \brief Clean up attached decal. */
	~meAttachedDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Engine decal or NULL. */
	inline deDecal *GetEngineDecal() const{ return pEngDecal; }
	
	/** \brief Parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	
	/** \brief Set parent object. */
	void SetParentObject( meObject *object );
	
	/** \brief Attach decal to parent. */
	void AttachToParent();
	
	/** \brief Remove decal from the parent. */
	void RemoveFromParent();
	/*@}*/
};

#endif
