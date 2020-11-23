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

#ifndef _IGDENATIVEBEOSAPPLICATION_H_
#define _IGDENATIVEBEOSAPPLICATION_H_

#include "beostoolkit.h"
#include "../../../environment/igdeEnvironment.h"
#include <dragengine/common/string/unicode/decUnicodeStringList.h>


class deException;
class igdeApplication;
class decUnicodeStringList;


/**
 * \brief BeOS toolkit native application.
 */
class igdeNativeBeOSApplication{
private:
	/** \brief Inner Application class required since some names clash. */
	class InnerApp : public BApplication{
	private:
		igdeNativeBeOSApplication &pApp;
		
	public:
		InnerApp( igdeNativeBeOSApplication &app );
		virtual ~InnerApp();
		
		virtual void ArgvReceived( int32 argc, char** argv );
		virtual void AboutRequested();
		virtual bool QuitRequested();
		virtual void ReadyToRun();
	};
	
	friend class InnerApp;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create application. */
	igdeNativeBeOSApplication( igdeApplication &owner );
	
	/** \brief Clean up application. */
	virtual ~igdeNativeBeOSApplication();
	
	/** \brief Create native application. */
	static igdeNativeBeOSApplication* CreateNativeApplication( igdeApplication &application );
	
	/** \brief Destroy native application. */
	virtual void DestroyNativeApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get arguments from OS specific startup routine. */
	static void GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args );
	
	/** \brief Initialize application. */
	void Initialize( decUnicodeStringList &arguments );
	
	/** \brief Run. */
	void Run();
	
	/** \brief Quit. */
	void Quit();
	
	/** \brief System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** \brief Get main app font config. */
	void GetAppFontConfig( igdeFont::sConfiguration &config ) const;
	
	/** \brief Show error. */
	void ShowError( const deException &exception ) const;
	
	/** \brief Show window modal while keeping engine properly updating. */
	void RunModalWhileShown( igdeWindow &window );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Disable updating during modal dialogs to about GDB lockups. */
	inline bool GetDisableModalUpdating() const{ return pDisableModalUpdating; }
	/*@}*/
	
	
	
private:
	igdeApplication &pOwner;
	InnerApp pInnerApp;
	bool pDisableModalUpdating;
	decUnicodeStringList pArguments;
};

typedef igdeNativeBeOSApplication igdeNativeApplication;

#endif
