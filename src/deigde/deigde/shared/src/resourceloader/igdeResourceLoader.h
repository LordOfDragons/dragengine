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

#ifndef _IGDERESOURCELOADER_H_
#define _IGDERESOURCELOADER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/resources/loader/deResourceLoader.h>

class igdeEnvironment;
class igdeResourceLoaderTask;
class igdeResourceLoaderListener;



/**
 * \brief Resource Loader.
 * 
 * Loads resources asynchronously using the engine resource loader. Enhances
 * the engine resource loader using a listener system to notify the editor
 * components about resources having finished loading. The editor is responsible
 * to call the update function in a regular interval to allow the loader to
 * use dead time optimally. Best way is to use a timer or calling the update
 * function right before updating the screen.
 */
class DE_DLL_EXPORT igdeResourceLoader{
	igdeEnvironment &pEnvironment;
	decObjectOrderedSet pTasks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resource loader. */
	igdeResourceLoader( igdeEnvironment &environment );
	
	/** \brief Clean up resource loader. */
	~igdeResourceLoader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/**
	 * \brief Updates the loader.
	 * \details Call this in regular intervals to optimally using dead time.
	 */
	void Update();
	
	/**
	 * \brief Request a resource.
	 * 
	 * The listener is used to notify about the finishing of the loading. The resourceType
	 * has to be one of the etr* values defined in deResourceLoader. Ensure the listener
	 * expects a resource of the given type since the resource is handed over to the
	 * listener as a deFileResource object which has to be cast to the correct type.
	 */
	void RequestResource( const char *filename, deResourceLoader::eResourceType resourceType,
		igdeResourceLoaderListener *listener );
	/*@}*/
	
	
	
private:
	int pIndexOfTaskWith( const char *filename, deResourceLoader::eResourceType resourceType );
	void pAddTask( const char *filename, deResourceLoader::eResourceType resourceType );
};

#endif
