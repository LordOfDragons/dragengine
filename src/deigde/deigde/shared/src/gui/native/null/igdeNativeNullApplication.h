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

#ifndef _IGDENATIVENULLAPPLICATION_H_
#define _IGDENATIVENULLAPPLICATION_H_

#include "../../../environment/igdeEnvironment.h"


class deException;
class igdeApplication;
class decUnicodeStringList;
#ifdef OS_W32
class decUnicodeArgumentList;
#endif


/**
 * Null native application.
 */
class igdeNativeNullApplication{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create application. */
	igdeNativeNullApplication( igdeApplication &owner );
	
	/** Clean up application. */
	virtual ~igdeNativeNullApplication();
	
	/** Create native application. */
	static igdeNativeNullApplication* CreateNativeApplication( igdeApplication &application );
	
	/** Destroy native application. */
	virtual void DestroyNativeApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Get arguments from OS specific startup routine. */
	#ifdef OS_UNIX
	static void GetOSStartUpArguments( decUnicodeStringList &arguments, int argCount, char **args );
	#elif defined OS_W32
	static void GetOSStartUpArguments( decUnicodeStringList &arguments,
		const decUnicodeArgumentList &windowsArguments );
	#endif
	
	/** Initialize application. */
	void Initialize( decUnicodeStringList &arguments );
	
	/** Run. */
	void Run();
	
	/** Quit. */
	void Quit();
	
	/** System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** Get main app font config. */
	void GetAppFontConfig( igdeFont::sConfiguration &config ) const;
	
	/** Show error. */
	void ShowError( const deException &exception ) const;
	
	/** Show window modal while keeping engine properly updating. */
	void RunModalWhileShown( igdeWindow &window );
	/*@}*/
	
	
	
private:
	igdeApplication &pOwner;
	bool pQuitRequested;
};

typedef igdeNativeNullApplication igdeNativeApplication;

#endif
