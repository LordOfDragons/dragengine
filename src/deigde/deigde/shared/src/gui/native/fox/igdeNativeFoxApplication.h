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
	FXDECLARE(igdeNativeFoxApplication)
protected:
	igdeNativeFoxApplication();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create application. */
	igdeNativeFoxApplication(igdeApplication &owner);
	
	/** \brief Clean up application. */
	~igdeNativeFoxApplication();
	
	/** \brief Create native application. */
	static igdeNativeFoxApplication* CreateNativeApplication(igdeApplication &application);
	
	/** \brief Destroy native application. */
	virtual void DestroyNativeApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get arguments from OS specific startup routine. */
	#ifdef OS_UNIX
	static void GetOSStartUpArguments(decUnicodeStringList &arguments, int argCount, char **args);
	#elif defined OS_W32
	static void GetOSStartUpArguments(decUnicodeStringList &arguments,
		const decUnicodeArgumentList &windowsArguments);
	#endif
	
	/** \brief Initialize application. */
	void Initialize(decUnicodeStringList &arguments);
	
	/** \brief Run. */
	void Run();
	
	/** \brief Quit. */
	void Quit();
	
	/** \brief System color. */
	decColor GetSystemColor(igdeEnvironment::eSystemColors color) const;
	
	/** \brief Get main app font config. */
	void GetAppFontConfig(igdeFont::sConfiguration &config) const;
	
	/** \brief Show error. */
	void ShowError(const deException &exception) const;
	
	/** \brief Show window modal while keeping engine properly updating. */
	void RunModalWhileShown(igdeWindow &window);
	
	/**
	 * \brief Global display scaling factor for display.
	 * 
	 * Value of 100 represents scaling of 100%. Value step size is 25.
	 */
	int GetDisplayScaleFactor();
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
	int pDisplayScaleFactor;
	igdeFont::sConfiguration pAppFontConfig;
};

typedef igdeNativeFoxApplication igdeNativeApplication;

#endif
