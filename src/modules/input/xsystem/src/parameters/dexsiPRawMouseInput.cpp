/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "dexsiPRawMouseInput.h"
#include "../deXSystemInput.h"

#include <dragengine/common/exceptions.h>


// Class dexsiPRawMouseInput
//////////////////////////////

// Constructor, destructor
////////////////////////////

dexsiPRawMouseInput::dexsiPRawMouseInput(deXSystemInput &xsi) : dexsiParameterBool(xsi){
	pParameter.SetName("rawMouseInput");
	pParameter.SetDescription(
		"Use raw mouse input directly from the input device without operating system speed "
		"or acceleration applied. Affects only applications capturing mouse input.");
	pParameter.SetType(deModuleParameter::eptBoolean);
	pParameter.SetCategory(deModuleParameter::ecBasic);
	pParameter.SetDisplayName("Raw Mouse Input");
	pParameter.SetDefaultValue("1");
}


// Parameter Value
////////////////////

bool dexsiPRawMouseInput::GetParameterBool(){
	return pModule.GetEnableRawMouseInput();
}

void dexsiPRawMouseInput::SetParameterBool(bool value){
	pModule.SetEnableRawMouseInput(value);
}
