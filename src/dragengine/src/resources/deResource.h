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

#ifndef _DERESOURCE_H_
#define _DERESOURCE_H_

#include "../deObject.h"

class deResourceManager;
class deEngine;
class decBaseFileReader;


/**
 * \brief Base resource class.
 * 
 * Provides the basic class for all resources. Each resource is assigned
 * the resource manager it originates from and a reference count. If you
 * want to add a reference to this resource do so using the AddReference
 * function. If you want to free the reference you have added before use
 * the FreeReference function and do not call 'delete' on the resource.
 * Once you receive as resource manager subclass the RemoveResource call
 * you have to use 'delete' to dispose of the resource and only then.
 */
class DE_DLL_EXPORT deResource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deResource> Ref;
	
	
private:
	deResourceManager *pResourceManager;
	deResource *pLLManagerPrev;
	deResource *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource. */
	deResource(deResourceManager *resourceManager);
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deResource() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Resource manager or NULL if resource is leaking. */
	inline deResourceManager *GetResourceManager() const{ return pResourceManager; }
	
	/**
	 * \brief Game engine object from resource manager.
	 * \throw deeInvalidAction if resource is leaking and has no resource manager anymore.
	 */
	deEngine *GetEngine() const;
	/*@}*/
	
	
	
	/** \name Resource manager linked list */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deResource *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For use by resource managers only.
	 */
	void SetLLManagerPrev(deResource *resource);
	
	/** \brief Next resource in the resource manager linked list. */
	inline deResource *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For use by resource managers only.
	 */
	void SetLLManagerNext(deResource *resource);
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For use by resource managers only.
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
