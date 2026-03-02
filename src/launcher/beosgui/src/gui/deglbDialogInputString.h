/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_DIALOGINPUTSTRING_H_
#define _DEGLB_DIALOGINPUTSTRING_H_

#include <Button.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <TextControl.h>
#include <Window.h>

class deglbDialogInputString : public BWindow {
private:
	enum {
		MSG_OK = 'ok__',
		MSG_CANCEL = 'cncl'
	};

public:
    deglbDialogInputString(const char* title, const char* label,
		const char *initialText, BMessenger target, int32 message);
	
    void MessageReceived(BMessage* msg) override;
	
private:
    BTextControl* pInput;
    BMessenger pTarget;
	int32 pMessage;
};

#endif
