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

#ifndef _IGDERESOURCELOADERLISTENER_H_
#define _IGDERESOURCELOADERLISTENER_H_

#include <dragengine/deObject.h>

class igdeResourceLoaderTask;
class deFileResource;



/**
 * \brief Resource Loader Task Listener.
 *
 * Listener for a resource loader task. Notifies about the
 * successfull or failed loading of a resource.
 */
class igdeResourceLoaderListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeResourceLoaderListener();
	
protected:
	/** \brief Clean up listener. */
	virtual ~igdeResourceLoaderListener();
	/*@}*/
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** \brief Loading of the resource finished. */
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource );
	
	/** \brief Loading of the resource failed. */
	virtual void LoadingFailed( const igdeResourceLoaderTask &task );
	/*@}*/
};

#endif
