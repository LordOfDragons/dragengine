/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// include only once
#ifndef _MEWINDOWEFFECTS_H_
#define _MEWINDOWEFFECTS_H_

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>



// predefinitions
class meWorld;
class meWindowMain;
class deEffect;



/**
 * Window to manage effects.
 */
class meWindowEffects : public FXTopWindow{
	FXDECLARE( meWindowEffects )
protected:
	meWindowEffects();
public:
	enum eFoxIDs{
		ID_SELF = FXTopWindow::ID_LAST,
		ID_CBEFFECTTYPES,
		ID_BTNADDEFFECT,
		ID_BTNREMOVEEFFECT,
		ID_LISTEFFECTS,
		ID_LAST
	};
	
private:
	meWindowMain *pWndMain;
	meWorld *pWorld;
	FXVerticalFrame *pFrameContent;
	FXComboBox *pCBEffectTypes;
	FXList *pListEffects;
	FXComposite *pPanel;
	
public:
	// constructor, destructor
	meWindowEffects( FXApp *app, meWindowMain *wndMain );
	virtual ~meWindowEffects();
	//virtual void create();
	
	// management
	inline meWindowMain *GetWindowMain() const{ return pWndMain; }
	void UpdateWorld();
	void UpdateEffectsList();
	const char *GetEffectTypeNameFor( int type ) const;
	void DisplayPanelFor( deEffect *effect );
	
	// events
	long onClose( FXObject *sender, FXSelector selector, void *data );
	long onBtnAddEffectCommand( FXObject *sender, FXSelector selector, void *data );
	long onListEffectsChanged( FXObject *sender, FXSelector selector, void *data );
};

// end of include only once
#endif

#endif
