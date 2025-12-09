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
class DE_DLL_EXPORT deImageManager : public deFileResourceManager{
private:
	deFileResourceList pImages;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new image resource manager linked to the given engine. */
	deImageManager(deEngine *engine);
	
	/** \brief Clean up image resource manager and reports leaking resources. */
	~deImageManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of images. */
	int GetImageCount() const;
	
	/** \brief Root image resource for iteration purpose. */
	deImage *GetRootImage() const;
	
	/** \brief Image with the given filename or NULL if not loaded yet. */
	deImage *GetImageWith(const char *filename) const;
	
	/** \brief Image with the given filename or NULL if not loaded yet. */
	deImage *GetImageWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create new image with the given parameters. */
	deImage::Ref CreateImage(int width, int height, int depth, int componentCount, int bitCount);
	
	/** \brief Load image from file relative to the base path. */
	deImage::Ref LoadImage(const char *filename, const char *basePath);
	
	/** \brief Load image from file relative to the base path. */
	deImage::Ref LoadImage(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/** \brief Loads default image. */
	deImage::Ref LoadDefault();
	
	/** \brief Saves image to the given file. */
	void SaveImage(deImage *image, const char *filename);
	
	/** \brief Saves image to the given file. */
	void SaveImage(deVirtualFileSystem *vfs, deImage *image, const char *filename);
	
	/**
	 * \brief Add loaded and fully prepared image.
	 * 
	 * This method is to be used only by the resource loader to add an image that has
	 * been loaded asynchronously.
	 */
	void AddLoadedImage(deImage *image);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad() override;
	void SystemGraphicUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
