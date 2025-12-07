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

#include "deoglPSSRReduction.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRReduction
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRReduction::deoglPSSRReduction(deGraphicOpenGl &ogl) :
deoglParameterInt(ogl)
{
	SetName("ssrReduction");
	SetDescription("Set Screen Space Reflection (SSR) reduction."
		" SSR is expensive especially on larger resolutions."
		" Rendering SSR at lower resolution then up-scaling improves performance.");
	SetType(deModuleParameter::eptSelection);
	SetDisplayName("SSR Reduction");
	SetCategory(ecAdvanced);
	
	const deModuleParameter::SelectionEntry entries[4] = {
		{"1", "1x", "Render SSR at full resolution. For strong graphic cards."},
		{"2", "2x", "Render SSR at half resolution then up-scale to full resolution."},
		{"4", "4x", "Render SSR at quarter resolution then up-scale to full resolution."},
		{"8", "8x", "Render SSR at 1/8 resolution then up-scale to full resolution."}
	};
	AddSelectionEntries(entries, 4);
	
	SetDefaultValue("1");
}

deoglPSSRReduction::~deoglPSSRReduction(){
}



// Parameter Value
////////////////////

int deoglPSSRReduction::GetParameterInt(){
	return pOgl.GetConfiguration().GetSSRReduction();
}

void deoglPSSRReduction::SetParameterInt(int value){
	pOgl.GetConfiguration().SetSSRReduction(value);
}
