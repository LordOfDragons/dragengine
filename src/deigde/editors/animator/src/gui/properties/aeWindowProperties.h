/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEWINDOWPROPERTIES_H_
#define _AEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/properties/igdeWPUndoHistoryReference.h>

class aeRule;
class aeWPAnimator;
class aeController;
class aeWindowMain;
class aeWPRule;
class aeWPController;
class aeWPLink;
class aeWPView;
class aeWPPlayground;
class aeAnimator;


/**
 * \brief Properties Panel.
 */
class aeWindowProperties : public igdeTabBook{
private:
	aeWindowMain &pWindowMain;
	
	aeWPAnimator *pPropAnimator;
	aeWPRule *pPropRule;
	aeWPLink *pPropLink;
	aeWPController *pPropController;
	aeWPView *pPropView;
	aeWPPlayground *pPropPlayground;
	igdeWPUndoHistoryReference pPanelUndoHistory;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWindowProperties( aeWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~aeWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Animator path changed. */
	void OnAnimatorPathChanged();
	/*@}*/
};

#endif
