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

#include "deglbDialogInputString.h"

deglbDialogInputString::deglbDialogInputString(const char* title, const char* label,
	const char *initialText, BMessenger target, int32 message) :
BWindow(BRect(0, 0, 300, 100), title, B_MODAL_WINDOW,
	B_NOT_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS | B_CLOSE_ON_ESCAPE),
pTarget(target),
pMessage(message)
{
	pInput = new BTextControl("input", label, initialText, nullptr);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pInput)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.AddGlue()
			.Add(new BButton("Cancel", new BMessage(MSG_CANCEL)))
			.Add(new BButton("OK", new BMessage(MSG_OK)))
		.End()
	.End();
	
	// Center on screen and focus text field
	CenterOnScreen();
	pInput->MakeFocus(true);
}


void deglbDialogInputString::MessageReceived(BMessage* msg){
	switch(msg->what){
	case MSG_OK:{
		BMessage reply(pMessage);
		reply.AddString("text", pInput->Text());
		pTarget.SendMessage(&reply);
		PostMessage(B_QUIT_REQUESTED);
		}break;
		
	case MSG_CANCEL:
		PostMessage(B_QUIT_REQUESTED);
		break;
		
	default:
		BWindow::MessageReceived(msg);
	}
}
