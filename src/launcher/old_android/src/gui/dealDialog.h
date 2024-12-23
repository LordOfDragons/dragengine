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

#ifndef _DEALDIALOG_H_
#define _DEALDIALOG_H_

#include "../deObject.h"
#include "../common/string/decString.h"

class dealDisplay;
class dealWidgetLayoutFlow;


/**
 * \brief Dialog displaying a layout widget in a display.
 * 
 * Only one dialog can be the root dialog covering the entire display with one
 * additional popup dialog not filling the entire view. 
 * 
 */
class dealDialog : public deObject{
private:
	dealDisplay &pDisplay;
	
	decString pTitle;
	dealWidgetLayoutFlow *pContent;
	dealDialog *pPopupDialog;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	dealDialog( dealDisplay &display, const char *title );
	
	/** \brief Clean up dialog. */
	virtual ~dealDialog();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	
	
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Content layout. */
	inline dealWidgetLayoutFlow *GetContent() const{ return pContent; }
	
	/** \brief Popup dialog or \em NULL if not set. */
	inline dealDialog *GetPopupDialog() const{ return pPopupDialog; }
	
	/** \brief Pop up or down dialog replacing the previous one if set. */
	void SetPopupDialog( dealDialog *dialog );
	
	
	
	/** \brief Dialog has been activated. */
	virtual void OnActivate();
	
	/** \brief Dialog has been deactivated. */
	virtual void OnDeactivate();
	
	/** \brief Frame update. */
	virtual void OnFrameUpdate();
	
	/**
	 * \brief Dialog to display on back key pressed or \em NULL to close application.
	 * \details Default implementation returns \em NULL.
	 */
	virtual dealDialog *BackKeyDialog();
	/*@}*/
};

#endif
