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

#ifndef _DERESOURCELIST_H_
#define _DERESOURCELIST_H_

#include "../dragengine_export.h"

class deResource;


/**
 * \brief Resource list.
 * 
 * The resource list class is the backend for resource management. This list
 * contains the main reference to the resource object. The list does not
 * call 'delete' on the resource object but removes the pointer to it.
 * The resource object calls 'delete' on itself thus never call it yourself
 * except one situation. If you have created a resource object and have not
 * added it to the resource list yet you have to use call 'delete' on it
 * yourself to avoid triggering RemoveResource on the resource manager.
 * 
 * \note Resources removed from the list are marked leaking. Basically this
 * clears their resource manager. 
 */
class DE_DLL_EXPORT deResourceList{
private:
	deResource *pRoot;
	deResource *pTail;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource list. */
	deResourceList();
	
	/** \brief Clean up resource list. */
	virtual ~deResourceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of resources. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Root resource. */
	inline deResource *GetRoot() const{ return pRoot; }
	
	/** \brief Tail resource. */
	inline deResource *GetTail() const{ return pTail; }
	
	/** \brief Resource is present. */
	bool Has( deResource *resource ) const;
	
	/** \brief Add resource. */
	void Add( deResource *resource );
	
	/** \brief Remove resource. */
	void Remove( deResource *resource );
	
	/** \brief Remove resource if present. */
	void RemoveIfPresent( deResource *resource );
	
	/** \brief Remove all resources. */
	void RemoveAll();
	/*@}*/
	
	
	
private:
	void pBareRemove( deResource *resource );
};

#endif
