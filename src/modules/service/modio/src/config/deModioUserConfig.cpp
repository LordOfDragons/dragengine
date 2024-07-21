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

#include "deModioUserConfig.h"
#include "../deModio.h"

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>


// Class deModioUserConfig
////////////////////////////

// Constructor, destructor
////////////////////////////

deModioUserConfig::deModioUserConfig( deModio &module, const decString &id ) :
pModule( module ),
pId( id ){
}

deModioUserConfig::deModioUserConfig( deModio &module, decBaseFileReader &reader ) :
pModule( module )
{
	const int version = reader.ReadByte();
	switch( version ){
	case 0:
		pReadFromFileV0( reader );
		break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "Unsupported version" );
	}
}

deModioUserConfig::~deModioUserConfig(){
}



// Management
///////////////

void deModioUserConfig::SetDisabledMods( const decStringSet &mods ){
	if( mods == pDisabledMods ){
		return;
	}
	
	pDisabledMods = mods;
	pModule.UserConfigChanged();
	
	if( pModule.GetCurUserId() == pId ){
		pModule.ActivateMods( pId );
	}
}

bool deModioUserConfig::GetModDisabled( const decString &id ) const{
	return pDisabledMods.Has( id );
}

void deModioUserConfig::SetModDisabled( const decString &id, bool disabled ){
	if( disabled ){
		if( pDisabledMods.Has( id ) ){
			return;
		}
		pDisabledMods.Add( id );
		
	}else{
		if( ! pDisabledMods.Has( id ) ){
			return;
		}
		pDisabledMods.Remove( id );
	}
	
	pModule.UserConfigChanged();
	
	if( disabled && pModule.GetCurUserId() == pId ){
		pModule.ActivateMods( pId );
	}
}



Modio::Rating deModioUserConfig::GetUserRating( const decString &id ) const{
	int rating = 0;
	if( ! pUserRatings.GetAt( id, &rating ) ){
		return Modio::Rating::Neutral;
	}
	
	switch( rating ){
	case 0:
		return Modio::Rating::Negative;
		
	case 1:
		return Modio::Rating::Positive;
		
	default:
		return Modio::Rating::Neutral;
	}
}

void deModioUserConfig::SetUserRating( const decString &id, Modio::Rating rating ){
	if( GetUserRating( id ) == rating ){
		return;
	}
	
	switch( rating ){
	case Modio::Rating::Negative:
		pUserRatings.SetAt( id, 0 );
		break;
		
	case Modio::Rating::Positive:
		pUserRatings.SetAt( id, 1 );
		break;
		
	case Modio::Rating::Neutral:
	default:
		pUserRatings.RemoveIfPresent( id );
		break;
	}
	
	pModule.UserConfigChanged();
}



void deModioUserConfig::WriteToFile( decBaseFileWriter &writer ){
	writer.WriteByte( 0 );
	writer.WriteString8( pId );
	
	int i, count = pDisabledMods.GetCount();
	writer.WriteInt( count );
	for( i=0; i<count; i++ ){
		writer.WriteString8( pDisabledMods.GetAt( i ) );
	}
	
	decStringList keys( pUserRatings.GetKeys() );
	count = keys.GetCount();
	writer.WriteInt( count );
	for( i=0; i<count; i++ ){
		const decString &key = keys.GetAt( i );
		writer.WriteString8( key );
		writer.WriteByte( ( uint8_t )pUserRatings.GetAt( key ) );
	}
}



// Private Functions
//////////////////////

void deModioUserConfig::pReadFromFileV0( decBaseFileReader &reader ){
	int i, count, intValue;
	decString key;
	
	pId = reader.ReadString8();
	
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		pDisabledMods.Add( reader.ReadString8() );
	}
	
	count = reader.ReadInt();
	for( i=0; i<count; i++ ){
		key = reader.ReadString8();
		intValue = reader.ReadByte();
		pUserRatings.SetAt( key, intValue );
	}
}
