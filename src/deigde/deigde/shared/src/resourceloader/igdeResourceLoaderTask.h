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

#ifndef _IGDERESOURCELOADERTASK_H_
#define _IGDERESOURCELOADERTASK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/loader/deResourceLoader.h>

class igdeResourceLoaderListener;
class deFileResource;
class deLogger;


/**
 * \brief Resource Loader Task.
 *
 * Task for loading a resource. Multiple listeners can be attached to
 * a task all of which are notified once the loading of the resource
 * finished successfully or with a failure.
 */
class DE_DLL_EXPORT igdeResourceLoaderTask : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeResourceLoaderTask> Ref;


private:
	decString pFilename;
	deResourceLoader::eResourceType pResourceType;
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	igdeResourceLoaderTask( const char *filename, deResourceLoader::eResourceType resourceType );
	
protected:
	/** \brief Clean up file fialog. */
	virtual ~igdeResourceLoaderTask();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Resource type. */
	inline deResourceLoader::eResourceType GetResourceType() const{ return pResourceType; }
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** \brief Add listener. */
	void AddListener( igdeResourceLoaderListener *listener );
	
	/** \brief Remove listener if present. */
	void RemoveListener( igdeResourceLoaderListener *listener );
	
	/**
	 * \brief Notify all that the loading finished successfully.
	 * 
	 * Processed listeners are removed after the call returns.
	 */
	void NotifyLoadingFinished( deLogger &logger, deFileResource *resource );
	
	/**
	 * \brief Notify all that the loading failed.
	 * 
	 * Processed listeners are removed after the call returns.
	 */
	void NotifyLoadingFailed( deLogger &logger );
	/*@}*/
};

#endif
