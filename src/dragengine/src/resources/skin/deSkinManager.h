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

#ifndef _DESKINMANAGER_H_
#define _DESKINMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

// definitions
#define SKIN_NO_SKIN		"<no_skin>"

class deEngine;
class deSkin;
class deSkinBuilder;


/**
 * \brief Skin Resource Manager.
 *
 */
class DE_DLL_EXPORT deSkinManager : public deFileResourceManager{
private:
	deFileResourceList pSkins;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new skin resource manager linked to the given engine. */
	deSkinManager(deEngine *engine);
	
	/** \brief Clean up skin resource manager and reports leaking resources. */
	~deSkinManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of skins. */
	int GetSkinCount() const;
	
	/** \brief Root skin resource for iteration purpose. */
	deSkin *GetRootSkin() const;
	
	/** \brief Skin with the given filename or NULL if not loaded yet. */
	deSkin *GetSkinWith(const char *filename) const;
	
	/** \brief Skin with the given filename or NULL if not loaded yet. */
	deSkin *GetSkinWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create new skin. */
	deSkin *CreateSkin(const char *filename, deSkinBuilder &builder);
	
	/** \brief Create new skin. */
	deSkin *CreateSkin(deVirtualFileSystem *vfs, const char *filename, deSkinBuilder &builder);
	
	/** \brief Loads a skinfrom the given file relative to the given base path. */
	deSkin *LoadSkin(const char *filename, const char *basePath);
	
	/** \brief Loads a skinfrom the given file relative to the given base path. */
	deSkin *LoadSkin(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/** \brief Loads the default skin. */
	deSkin *LoadDefault();
	
	/**
	 * \brief Add loaded and fully prepared skin.
	 * 
	 * This method is to be used only by the resource loader to add an skin that has been
	 * loaded asynchronously.
	 */
	void AddLoadedSkin(deSkin *skin);
	
	/** \brief Load resources used in properties. */
	void LoadPropertyResources(deSkin &skin);
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	void SystemAudioLoad();
	void SystemAudioUnload();
	void SystemPhysicsLoad();
	void SystemPhysicsUnload();
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
