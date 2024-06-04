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
