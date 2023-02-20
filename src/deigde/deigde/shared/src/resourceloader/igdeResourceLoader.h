/* 
 * Drag[en]gine IGDE
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
