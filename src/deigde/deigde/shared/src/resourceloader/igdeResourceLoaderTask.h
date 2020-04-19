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
class igdeResourceLoaderTask : public deObject{
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
