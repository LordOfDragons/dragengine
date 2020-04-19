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

#ifndef _DELANGUAGEPACKAMANAGER_H_
#define _DELANGUAGEPACKAMANAGER_H_ 

#include "../deFileResourceList.h"
#include "../deFileResourceManager.h"

class deEngine;
class deLanguagePack;
class deLanguagePackBuilder;


/**
 * \brief Language pack resource manager.
 */
class deLanguagePackManager : public deFileResourceManager{
private:
	deFileResourceList pLangPacks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack resource manager. */
	deLanguagePackManager( deEngine *engine );
	
	/** \brief Clean up language pack resource manager and reports leaking resources. */
	virtual ~deLanguagePackManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of language packs. */
	int GetLanguagePackCount() const;
	
	/** \brief Root language pack resource for iteration purpose. */
	deLanguagePack *GetRootLanguagePack() const;
	
	/** \brief Language pack with filename or NULL. */
	deLanguagePack *GetLanguagePackWith( const char *filename ) const;
	
	/** \brief Language pack with filename or NULL. */
	deLanguagePack *GetLanguagePackWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create language pack using builder. */
	deLanguagePack *CreateLanguagePack( const char *filename, deLanguagePackBuilder &builder );
	
	/** \brief Create language pack using builder. */
	deLanguagePack *CreateLanguagePack( deVirtualFileSystem *vfs,
		const char *filename, deLanguagePackBuilder &builder );
	
	/** \brief Load language pack relative to base path. */
	deLanguagePack *LoadLanguagePack( const char *filename, const char *basePath = "/" );
	
	/** \brief Load language pack relative to base path. */
	deLanguagePack *LoadLanguagePack( deVirtualFileSystem *vfs,
		const char *filename, const char *basePath = "/" );
	
	/**
	 * \brief Add loaded and prepared language pack.
	 * 
	 * \warning To be used only by deResourceLoader.
	 */
	void AddLoadedLanguagePack( deLanguagePack *languagePack );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * \warning For use by resource objects only.
	 */
	/*@{*/
	virtual void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
