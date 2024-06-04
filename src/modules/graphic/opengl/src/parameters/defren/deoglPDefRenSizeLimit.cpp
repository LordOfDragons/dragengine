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

#include "deoglPDefRenSizeLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../rendering/defren/deoglDeferredRendering.h"

#include <dragengine/common/exceptions.h>



// Class deoglPDefRenSizeLimit
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPDefRenSizeLimit::deoglPDefRenSizeLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "defRenSizeLimit" );
	SetDescription( "Sets the size limit for deferred rendering. Some graphic cards or"
		" drivers have troubles with large render resolutions especially if used with"
		" deferred rendering. In this case limiting the internal rendering size can help."
		" To use the limit set defRenSizeLimit to a value larger than 0. If the rendering"
		" would be larger than this value the internal rendering is scaled proportionally"
		" so that neither the width nor the height is larger. Once finished rendering the"
		" image is scaled up to the desired size." );
	SetCategory( ecExpert );
}

deoglPDefRenSizeLimit::~deoglPDefRenSizeLimit(){
}



// Management
///////////////

int deoglPDefRenSizeLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetDefRenSizeLimit();
}

void deoglPDefRenSizeLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetDefRenSizeLimit( value );
}
