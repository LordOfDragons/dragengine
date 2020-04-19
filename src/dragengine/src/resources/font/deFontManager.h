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

#ifndef _DEFONTMANAGER_H_
#define _DEFONTMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deFont;
class deFontBuilder;


/**
 * \brief Font resource manager.
 */
class deFontManager : public deFileResourceManager{
private:
	deFileResourceList pFonts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font resource manager. */
	deFontManager( deEngine *engine );
	
	/** \brief Clean up font resource manager and reports leaking resources. */
	virtual ~deFontManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of fonts. */
	int GetFontCount() const;
	
	/** \brief Root font resource for iteration purpose. */
	deFont *GetRootFont() const;
	
	/** \brief Font with filename or NULL. */
	deFont *GetFontWith( const char *filename ) const;
	
	/** \brief Font with filename or NULL. */
	deFont *GetFontWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Create new font. */
	deFont *CreateFont( const char *filename, deFontBuilder &builder );
	
	/** \brief Create new font. */
	deFont *CreateFont( deVirtualFileSystem *vfs, const char *filename,
		deFontBuilder &builder );
	
	/** \brief Load font from file relative to base path. */
	deFont *LoadFont( const char *filename, const char *basePath );
	
	/** \brief Load font from file relative to base path. */
	deFont *LoadFont( deVirtualFileSystem *vfs, const char *filename, const char *basePath );
	
	/** \brief Load debug font. */
	deFont *LoadDebugFont();
	
	/**
	 * \brief Add loaded and prepared font.
	 * 
	 * \warning To be used only by deResourceLoader.
	 */
	void AddLoadedFont( deFont *font );
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Create graphic system peers. */
	virtual void SystemGraphicLoad();
	
	/** \brief Free graphic system peers. */
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	virtual void RemoveResource( deResource *resource );
	/*@}*/
	
	
	
private:
	void pLoadFontSources( deFont *font );
};

#endif
