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

#ifndef _IGDENATIVEFOXAPPLICATION_H_
#define _IGDENATIVEFOXAPPLICATION_H_

#include "foxtoolkit.h"
#include "../../../environment/igdeEnvironment.h"


class deException;
class igdeApplication;
class decUnicodeStringList;
#ifdef OS_W32
class decUnicodeArgumentList;
#endif


/**
 * \brief FOX toolkit native application.
 */
class igdeNativeFoxApplication : public FXApp{
	FXDECLARE( igdeNativeFoxApplication )
protected:
	igdeNativeFoxApplication();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create application. */
	igdeNativeFoxApplication( igdeApplication &owner );
	
	/** \brief Clean up application. */
	~igdeNativeFoxApplication();
	
	/** \brief Create native application. */
	static igdeNativeFoxApplication* CreateNativeApplication( igdeApplication &application );
	
	/** \brief Destroy native application. */
	virtual void DestroyNativeApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get arguments from OS specific startup routine. */
	#ifdef OS_UNIX
	static void GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args );
	#elif defined OS_W32
	static void GetOSStartUpArguments( decUnicodeStringList &arguments,
		const decUnicodeArgumentList &windowsArguments );
	#endif
	
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
	igdeApplication *pOwner;
	FXToolTip *pToolTip;
	bool pDisableModalUpdating;
	char **pFoxArgs;
	int pFoxArgCount;
};

typedef igdeNativeFoxApplication igdeNativeApplication;

#endif
