/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEIMAGEMANAGER_H_
#define _DEIMAGEMANAGER_H_

#include "../../dragengine_configuration.h"

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

#ifdef OS_W32
#undef LoadImage
#endif

class deEngine;
class deImage;


/**
 * \brief Image Resource Manager.
 */
class deImageManager : public deFileResourceManager{
private:
	deFileResourceList pImages;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new image resource manager linked to the given engine. */
	deImageManager( deEngine *engine );
	
	/** \brief Clean up image resource manager and reports leaking resources. */
	virtual ~deImageManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of images. */
	int GetImageCount() const;
	
	/** \brief Root image resource for iteration purpose. */
	deImage *GetRootImage() const;
	
	/** \brief Image with the given filename or NULL if not loaded yet. */
	deImage *GetImageWith( const char *filename ) const;
	
	/** \brief Image with the given filename or NULL if not loaded yet. */
	deImage *GetImageWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new image with the given parameters. */
	deImage *CreateImage( int width, int height, int depth, int componentCount, int bitCount );
	
	/** \brief Load image from file relative to the base path. */
	deImage *LoadImage( const char *filename, const char *basePath );
	
	/** \brief Load image from file relative to the base path. */
	deImage *LoadImage( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/** \brief Loads default image. */
	deImage *LoadDefault();
	
	/** \brief Saves image to the given file. */
	void SaveImage( deImage *image, const char *filename );
	
	/** \brief Saves image to the given file. */
	void SaveImage( deVirtualFileSystem *vfs, deImage *image, const char *filename );
	
	/**
	 * \brief Add loaded and fully prepared image.
	 * 
	 * This method is to be used only by the resource loader to add an image that has
	 * been loaded asynchronously.
	 */
	void AddLoadedImage( deImage *image );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource);
	/*@}*/
};

#endif
