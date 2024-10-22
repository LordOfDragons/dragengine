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

#include "deoalPMaxEnvSlots.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoalPMaxEnvSlots
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalPMaxEnvSlots::deoalPMaxEnvSlots( deAudioOpenAL &oal ) : deoalParameterInt( oal )
{
	SetName( "maxEnvs" );
	SetDescription( "Maximum count of environments to use for software auralization."
		" Larger values allow to simulate more different kinds of audio environments"
		" but is expensive. Smaller values runs faster but has less audio diversity."
		" If audio stutters or emits clicks try lowering this value."
		" This parameter is only used if 'AuralizationMode' is set to 'full'."
	);
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 2.0f );
	SetMaximumValue( 8.0f );
	SetValueStepSize( 1.0f );
	
	SetCategory( ecBasic );
	SetDisplayName( "Maximum Environments" );
	SetDefaultValue( "8" );
}

deoalPMaxEnvSlots::~deoalPMaxEnvSlots(){
}



// Management
///////////////

int deoalPMaxEnvSlots::GetParameterInt(){
	return pOal.GetConfiguration().GetMaxSharedEffectSlots();
}

void deoalPMaxEnvSlots::SetParameterInt( int value ){
	pOal.GetConfiguration().SetMaxSharedEffectSlots( value );
}
