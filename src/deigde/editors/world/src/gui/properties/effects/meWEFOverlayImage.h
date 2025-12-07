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
#ifndef _MEWEFOVERLAYIMAGE_H_
#define _MEWEFOVERLAYIMAGE_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class deEffectOverlayImage;
class deEngine;



/**
 * Panel for editing an overlay image effect.
 */
class meWEFOverlayImage : public FXVerticalFrame{
	FXDECLARE(meWEFOverlayImage)
protected:
	meWEFOverlayImage();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_EDITIMAGE,
		ID_EDITTRANSPARENCY,
		ID_EDITCORNER1X,
		ID_EDITCORNER1Y,
		ID_EDITCORNER2X,
		ID_EDITCORNER2Y,
		ID_EDITCORNER3X,
		ID_EDITCORNER3Y,
		ID_EDITCORNER4X,
		ID_EDITCORNER4Y,
		ID_CHKENABLE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectOverlayImage *pEffectOverlay;
	deEngine *pEngine;
	
	FXTextField *pEditImage;
	FXTextField *pEditTransparency;
	FXTextField *pEditCorner1X;
	FXTextField *pEditCorner1Y;
	FXTextField *pEditCorner2X;
	FXTextField *pEditCorner2Y;
	FXTextField *pEditCorner3X;
	FXTextField *pEditCorner3Y;
	FXTextField *pEditCorner4X;
	FXTextField *pEditCorner4Y;
	FXCheckButton *pChkEnable;
	
public:
	// constructor, destructor
	meWEFOverlayImage(deEffectOverlayImage *effectOverlay, meWindowEffects *windowEffects, FXComposite *container);
	virtual ~meWEFOverlayImage();
	
	// management
	
	// events
	long onEditImageCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditTransparencyCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner1XCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner1YCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner2XCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner2YCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner3XCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner3YCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner4XCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditCorner4YCommand(FXObject *sender, FXSelector selector, void *data);
	long onCheckEnableCommand(FXObject *sender, FXSelector selector, void *data);
};

// end of include only once
#endif

#endif
