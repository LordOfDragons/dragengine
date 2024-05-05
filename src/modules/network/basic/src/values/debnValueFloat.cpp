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

#include "debnValueFloat.h"
#include "../deNetworkBasic.h"
#include "../half/half.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/network/deNetworkMessage.h>
#include <dragengine/resources/network/value/deNetworkValueFloat.h>



// Class debnValueFloat
/////////////////////////

// Constructor, destructor
////////////////////////////

debnValueFloat::debnValueFloat( deNetworkValueFloat &valueFloat ) :
pValueFloat( valueFloat ),
pLastValue( valueFloat.GetFloat() )
{
	switch( valueFloat.GetFormat() ){
	case deNetworkValue::evfFloat16:
		SetDataType( evtFloat16 );
		break;
		
	case deNetworkValue::evfFloat32:
		SetDataType( evtFloat32 );
		break;
		
	case deNetworkValue::evfFloat64:
		SetDataType( evtFloat64 );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

debnValueFloat::~debnValueFloat(){
}



// Management
///////////////

bool debnValueFloat::UpdateValue( bool force ){
	if( ! force && fabs( pValueFloat.GetFloat() - pLastValue ) <= pValueFloat.GetPrecision() ){
		return false;
	}
	
	pLastValue = pValueFloat.GetFloat();
	return true;
}

void debnValueFloat::ReadValue( decBaseFileReader &reader ){
	switch( GetDataType() ){
	case evtFloat16:
		pLastValue = halfToFloat( reader.ReadUShort() );
		break;
		
	case evtFloat32:
		pLastValue = reader.ReadFloat();
		break;
		
	case evtFloat64:
		pLastValue = reader.ReadDouble();
		break;
		
	default:
		return;
	}
	
	pValueFloat.SetFloat( pLastValue );
}

void debnValueFloat::WriteValue( decBaseFileWriter &writer ){
	switch( GetDataType() ){
	case evtFloat16:
		writer.WriteUShort( floatToHalf( ( float )pValueFloat.GetFloat() ) );
		break;
		
	case evtFloat32:
		writer.WriteFloat( ( float )pValueFloat.GetFloat() );
		break;
		
	case evtFloat64:
		writer.WriteDouble( pValueFloat.GetFloat() );
		break;
		
	default:
		break;
	}
}
