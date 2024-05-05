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

#ifndef _IGDEDIALOGBROWSERSKY_H_
#define _IGDEDIALOGBROWSERSKY_H_

#include "igdeDialogBrowser.h"


class igdeGDSky;


/**
 * \brief Dialog for user to browse sky.
 */
class DE_DLL_EXPORT igdeDialogBrowserSky : public igdeDialogBrowser{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogBrowserSky( igdeEnvironment &environment,
		const char *title = "Select Object Class", bool canResize = true );
	
protected:
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowserSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected sky. */
	igdeGDSky *GetSelectedSky() const;
	
	/** \brief Set selected sky. */
	void SetSelectedSky( igdeGDSky *gdSky );
	
	
	
	/** \brief Convenience method to select sky. */
	static bool SelectSky( igdeWidget *owner, igdeGDSky* &sky, const char *title = "Select Sky" );
	
	/** \brief Convenience method to select sky. */
	static bool SelectSky( igdeWidget *owner, decString &sky, const char *title = "Select Sky" );
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor );
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener );
	virtual void GetSelectedItemInfo( decString &info );
};

#endif
