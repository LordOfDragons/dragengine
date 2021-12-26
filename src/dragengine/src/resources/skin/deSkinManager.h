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
	deSkinManager( deEngine *engine );
	
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
	deSkin *GetSkinWith( const char *filename ) const;
	
	/** \brief Skin with the given filename or NULL if not loaded yet. */
	deSkin *GetSkinWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new skin. */
	deSkin *CreateSkin( const char *filename, deSkinBuilder &builder );
	
	/** \brief Create new skin. */
	deSkin *CreateSkin( deVirtualFileSystem *vfs, const char *filename, deSkinBuilder &builder );
	
	/** \brief Loads a skinfrom the given file relative to the given base path. */
	deSkin *LoadSkin( const char *filename, const char *basePath );
	
	/** \brief Loads a skinfrom the given file relative to the given base path. */
	deSkin *LoadSkin( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/** \brief Loads the default skin. */
	deSkin *LoadDefault();
	
	/**
	 * \brief Add loaded and fully prepared skin.
	 * 
	 * This method is to be used only by the resource loader to add an skin that has been
	 * loaded asynchronously.
	 */
	void AddLoadedSkin( deSkin *skin );
	
	/** \brief Load resources used in properties. */
	void LoadPropertyResources( deSkin &skin );
	
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
