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

#include "igdeDialogMultilineValue.h"
#include "../igdeLabel.h"
#include "../igdeTextArea.h"
#include "../igdeUIHelper.h"
#include "../igdeContainer.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>



// Class igdeDialogMultilineValue
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogMultilineValue::igdeDialogMultilineValue(igdeEnvironment &environment,
	const char *title, const char *message, int rows, int columns) :
igdeDialog(environment, title)
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerFlow::Ref content(igdeContainerFlow::Ref::New(
		environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 10));
	
	helper.Label(content, message);
	helper.EditString(content, "", pEditValue, columns, rows, {});
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Discard");
	
	AddContent(content, buttonBar);
}

igdeDialogMultilineValue::~igdeDialogMultilineValue(){
}



// Management
///////////////

const decString &igdeDialogMultilineValue::GetValue() const{
	return pEditValue->GetText();
}

void igdeDialogMultilineValue::SetValue(const char *value){
	pEditValue->SetText(value);
}
