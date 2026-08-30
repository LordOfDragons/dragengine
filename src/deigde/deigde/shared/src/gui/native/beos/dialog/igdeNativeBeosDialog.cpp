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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosDialog.h"


// Class igdeNativeBeosDialog
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeosDialog::igdeNativeBeosDialog(igdeDialog &owner) :
pOwnerDialog(owner){
}

igdeNativeBeosDialog::~igdeNativeBeosDialog() = default;

igdeNativeBeosDialog *igdeNativeBeosDialog::CreateNativeWidget(igdeDialog &owner, igdeWidget*){
	// DELint-Allow-NewWithoutRef
	return new igdeNativeBeosDialog(owner);
}

void igdeNativeBeosDialog::PostCreateNativeWidget(){
}

void igdeNativeBeosDialog::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeBeosDialog::UpdateEnabled(){
}

void igdeNativeBeosDialog::UpdatePosition(){
}

void igdeNativeBeosDialog::UpdateIcon(){
}

void igdeNativeBeosDialog::UpdateTitle(){
}

void igdeNativeBeosDialog::UpdateSize(){
}

void igdeNativeBeosDialog::RaiseAndActivate(){
}

void igdeNativeBeosDialog::ShowDialog(){
}

void igdeNativeBeosDialog::CloseDialog(bool){
}

void igdeNativeBeosDialog::OnFrameUpdate(){
}

int igdeNativeBeosDialog::DialogPadContent(const igdeGuiTheme&){
	return 0;
}

int igdeNativeBeosDialog::DialogPadButtons(const igdeGuiTheme&){
	return 0;
}

#endif
