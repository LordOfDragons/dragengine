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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPRenderDownScale.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPRenderDownScale
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPRenderDownScale::deoglPRenderDownScale( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "renderDownScale" );
	SetDescription( "Render at lower resolution the up-scale. Improves performance at the cost of quality." );
	SetType( eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "Render Down-Scale" );
	
	const deModuleParameter::SelectionEntry entries[ 4 ] = {
		{ "1", "1x", "Render at full resolution." },
		{ "2", "2x", "Render at half resolution then up-scale to full resolution." },
		{ "4", "4x", "Render at quarter resolution then up-scale to full resolution." },
		{ "8", "8x", "Render at 1/8 resolution then up-scale to full resolution." }
	};
	AddSelectionEntries( entries, 4 );
	SetDefaultValue( "1" );
}

deoglPRenderDownScale::~deoglPRenderDownScale(){
}



// Parameter Value
////////////////////

int deoglPRenderDownScale::GetParameterInt(){
	return pOgl.GetConfiguration().GetRenderDownScale();
}

void deoglPRenderDownScale::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetRenderDownScale( value );
}
