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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPLODMaxPixelError.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPLODMaxPixelError
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPLODMaxPixelError::deoglPLODMaxPixelError(deGraphicOpenGl &ogl) : deoglParameterInt(ogl){
	SetName("lodMaxPixelError");
	SetDescription("Maximum allowed error in pixels for LOD calculation."
		" Higher values chooses LOD geometry quicker improving performance but can increase popping.");
	SetType(deModuleParameter::eptSelection);
	SetCategory(ecAdvanced);
	SetDisplayName("LOD Max Pixel Error");
	
	const deModuleParameter::SelectionEntry entries[6] = {
		{"1", "1 Pixel", "Allow 1 Pixel error"},
		{"2", "2 Pixel", "Allow 2 Pixel error"},
		{"4", "4 Pixel", "Allow 4 Pixel error"},
		{"8", "8 Pixel", "Allow 8 Pixel error"},
		{"16", "16 Pixel", "Allow 16 Pixel error"},
		{"32", "32 Pixel", "Allow 32 Pixel error"}
	};
	AddSelectionEntries(entries, 6);
	SetDefaultValue("2");
}

deoglPLODMaxPixelError::~deoglPLODMaxPixelError(){
}



// Parameter Value
////////////////////

int deoglPLODMaxPixelError::GetParameterInt(){
	return pOgl.GetConfiguration().GetLODMaxPixelError();
}

void deoglPLODMaxPixelError::SetParameterInt(int value){
	pOgl.GetConfiguration().SetLODMaxPixelError(value);
}
