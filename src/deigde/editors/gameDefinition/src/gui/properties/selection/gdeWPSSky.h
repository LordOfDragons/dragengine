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

#ifndef _GDEWPSSKY_H_
#define _GDEWPSSKY_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeSky;
class gdeSkyController;
class gdeCategoryList;
class gdeWindowProperties;
class gdeWPSSkyListener;



/**
 * \brief Sky property panel.
 */
class gdeWPSSky : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSSkyListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeActionReference pActionControllerAdd;
	igdeActionReference pActionControllerRemove;
	igdeActionContextMenuReference pActionControllerMenu;
	
	igdeEditPathReference pEditPath;
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	
	igdeComboBoxReference pCBController;
	igdeButtonReference pBtnControllerMenu;
	igdeTextFieldReference pEditControllerName;
	igdeTextFieldReference pEditControllerValue;
	
	igdeComboBoxFilterReference pCBCategory;
	igdeButtonReference pBtnJumpToCategory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSSky( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active sky. */
	gdeSky *GetSky() const;
	
	/** \brief Active controller. */
	gdeSkyController *GetController() const;
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline igdeAction *GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline igdeActionContextMenu *GetActionControllerMenu() const{ return pActionControllerMenu; }
	
	
	
	/** \brief Update category list. */
	void UpdateCategoryList();
	
	/** \brief Update category list. */
	void UpdateCategoryList( const gdeCategoryList &list, const char *prefix );
	
	
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update controller. */
	void UpdateController();
	/*@}*/
};

#endif
