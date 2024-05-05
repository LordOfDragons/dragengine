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

#ifndef _IGDEAPPLICATION_H_
#define _IGDEAPPLICATION_H_


#include "../environment/igdeEnvironment.h"
#include "igdeWidgetReference.h"

#include <dragengine/dragengine_configuration.h>
#include <dragengine/common/string/decString.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#ifdef OS_BEOS
#include <app/Application.h>
#endif


class decUnicodeStringList;
class igdeMainWindow;
class igdeWindow;


/**
 * \brief IGDE UI Application.
 */
class DE_DLL_EXPORT igdeApplication{
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create application. */
	igdeApplication();
	
	/** \brief Clean up application. */
	virtual ~igdeApplication();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	igdeMainWindow *GetMainWindow() const;
	
	/**
	 * \brief Run application.
	 * 
	 * Calls Initialize() then runs the application until it exits. After exit calls CleanUp()
	 * before returning. Caller has to exit entry point with OK return code unless an exception
	 * is thrown. In case of exception CleanUp() is called properly.
	 */
#ifdef OS_UNIX
	void Run( int argCount, char **args );
#elif defined OS_W32
	void Run();
#else
#error "Unsupported OS"
#endif
	
	/** \brief System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** \brief Get main app font config. */
	void GetAppFontConfig( igdeFont::sConfiguration &config );
	
	/** \brief Application instance. */
	static igdeApplication &app();
	
	/** \brief Show window modal while keeping engine properly updating. */
	void RunModalWhileShown( igdeWindow &window );
	
	/** \brief For internal use only. */
	inline void *GetNativeApplication() const{ return pNativeApplication; }
	/*@}*/
	
	
	
protected:
	/**
	 * \brief Initialize application.
	 * \returns true to run application or false to abord.
	 */
	virtual bool Initialize( const decUnicodeStringList &arguments ) = 0;
	
	/** \brief Set main window. */
	void SetMainWindow( igdeMainWindow *mainWindow, bool takeOver );
	
	/** \brief Clean up application. */
	virtual void CleanUp();
	
	
	
private:
	void pSharedRun( decUnicodeStringList &arguments );
	
	void *pNativeApplication;
	igdeWidgetReference pMainWindow;
	static igdeApplication *pApp;
};

#endif
