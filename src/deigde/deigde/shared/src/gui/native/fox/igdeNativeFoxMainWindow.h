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

#ifndef _IGDENATIVEFOXMAINWINDOW_H_
#define _IGDENATIVEFOXMAINWINDOW_H_

#include "foxtoolkit.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/math/decMath.h>


class igdeMainWindow;


/**
 * \brief FOX toolkit native main window.
 */
class igdeNativeFoxMainWindow : public FXMainWindow{
	FXDECLARE( igdeNativeFoxMainWindow )
protected:
	igdeNativeFoxMainWindow();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	igdeNativeFoxMainWindow( igdeMainWindow &owner );
	
	/** \brief Clean up container. */
	virtual ~igdeNativeFoxMainWindow();
	
	/** \brief Create native widget. */
	static igdeNativeFoxMainWindow* CreateNativeWidget( igdeMainWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void create();
	virtual void destroy();
	
	/** \brief System color. */
	decColor GetSystemColor( igdeEnvironment::eSystemColors color ) const;
	
	/** \brief Update window state. */
	void UpdateWindowState();
	
	/** \brief Update enabled state. */
	virtual void UpdateEnabled();
	
	/** \brief Update position. */
	virtual void UpdatePosition();
	
	/** \brief Update icon. */
	virtual void UpdateIcon();
	
	/** \brief Update window title. */
	virtual void UpdateTitle();
	
	/** \brief Update size. */
	virtual void UpdateSize();
	
	/** \brief Set window state to owner. */
	void SetWindowState();
	
	/** \brief Get main app font config. */
	static void GetAppFontConfig( igdeFont::sConfiguration &config );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onConfigure( FXObject*, FXSelector, void* );
	long onMap( FXObject*, FXSelector, void* );
	long onClose( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	long onMinimized( FXObject*, FXSelector, void* );
	long onRestored( FXObject*, FXSelector, void* );
	long onMaximized( FXObject*, FXSelector, void* );
	/*@}*/
	
private:
	igdeMainWindow *pOwner;
};

typedef igdeNativeFoxMainWindow igdeNativeMainWindow;

#endif
