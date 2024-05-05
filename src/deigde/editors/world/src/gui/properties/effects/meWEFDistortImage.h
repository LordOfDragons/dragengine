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
#ifndef _MEWEFDISTORTIMAGE_H_
#define _MEWEFDISTORTIMAGE_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class deEffectDistortImage;
class deEngine;



/**
 * Panel for editing an overlay image effect.
 */
class meWEFDistortImage : public FXVerticalFrame{
	FXDECLARE( meWEFDistortImage )
protected:
	meWEFDistortImage();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_EDITIMAGE,
		ID_EDITSTRENGTHU,
		ID_EDITSTRENGTHV,
		ID_CHKENABLE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectDistortImage *pEffectDistort;
	deEngine *pEngine;
	
	FXTextField *pEditImage;
	FXTextField *pEditStrengthU;
	FXTextField *pEditStrengthV;
	FXCheckButton *pChkEnable;
	
public:
	// constructor, destructor
	meWEFDistortImage( deEffectDistortImage *effectDistort, meWindowEffects *windowEffects, FXComposite *container );
	virtual ~meWEFDistortImage();
	
	// management
	
	// events
	long onEditImageCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditStrengthUCommand( FXObject *sender, FXSelector selector, void *data );
	long onEditStrengthVCommand( FXObject *sender, FXSelector selector, void *data );
	long onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data );
};

// end of include only once
#endif

#endif
