/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deClassMath.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <libdscript/exceptions.h>

#define DEG_TO_RAD 0.017453292519943295
#define RAD_TO_DEG 57.295779513082323



// Native Functions
/////////////////////

// public static func float acos( float n )
deClassMath::nfacos::nfacos( const sInitData &init ) : dsFunction( init.clsMath, "acos", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // n
}
void deClassMath::nfacos::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )acos( rt->GetValue( 0 )->GetFloat() ) * RAD_TO_DEG );
}

// public static func float asin( float n )
deClassMath::nfasin::nfasin( const sInitData &init ) : dsFunction( init.clsMath, "asin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // n
}
void deClassMath::nfasin::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )asin( rt->GetValue( 0 )->GetFloat() ) * RAD_TO_DEG );
}

// public static func float atan( float n )
deClassMath::nfatan::nfatan( const sInitData &init ) : dsFunction( init.clsMath, "atan", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // n
}
void deClassMath::nfatan::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )atan( rt->GetValue( 0 )->GetFloat() ) * RAD_TO_DEG );
}

// public static func float atan2( float y, float x )
deClassMath::nfatan2::nfatan2( const sInitData &init ) : dsFunction( init.clsMath, "atan2", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // y
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nfatan2::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )atan2( rt->GetValue( 0 )->GetFloat(), rt->GetValue( 1 )->GetFloat() ) * RAD_TO_DEG );
}

// public static func float cos( float x )
deClassMath::nfcos::nfcos( const sInitData &init ) : dsFunction( init.clsMath, "cos", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}

void deClassMath::nfcos::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )cos( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD ) );
}

// public static func float cosh( float x )
deClassMath::nfcosh::nfcosh( const sInitData &init ) : dsFunction( init.clsMath, "cosh", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nfcosh::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )cosh( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD ) );
}

// public static func float sin( float x )
deClassMath::nfsin::nfsin( const sInitData &init ) : dsFunction( init.clsMath, "sin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nfsin::RunFunction( dsRunTime *rt, dsValue *myself ){
	// NOTE this is such a strange bug I have no idea where it comes from. if commented out code
	//      is run the application works. when valgrind is used it segfaults right at this line.
	//      when I change the code slightly in a way it is still exactly the same (and most
	//      probably is optimized to the same) valgrind stops segfaulting. really bizzare.
	//rt->PushFloat( ( float )sin( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD ) );
	const float input = rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD;
	rt->PushFloat( ( float )sin( input ) );
}

// public static func float sinh( float x )
deClassMath::nfsinh::nfsinh( const sInitData &init ) : dsFunction( init.clsMath, "sinh", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nfsinh::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )sinh( rt->GetValue(0)->GetFloat() * DEG_TO_RAD ) );
}

// public static func float tan( float x )
deClassMath::nftan::nftan( const sInitData &init ) : dsFunction( init.clsMath, "tan", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nftan::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )tan( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD ) );
}

// public static func float tanh( float x )
deClassMath::nftanh::nftanh( const sInitData &init ) : dsFunction( init.clsMath, "tanh", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
}
void deClassMath::nftanh::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )tanh( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD ) );
}



// public static func float degToRad( float degrees )
deClassMath::nfDegToRad::nfDegToRad( const sInitData &init ) : dsFunction( init.clsMath, "degToRad", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // degrees
}
void deClassMath::nfDegToRad::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( rt->GetValue( 0 )->GetFloat() * DEG_TO_RAD );
}

// public static func float radToDeg( float radians )
deClassMath::nfRadToDeg::nfRadToDeg( const sInitData &init ) : dsFunction( init.clsMath, "radToDeg", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // radians
}
void deClassMath::nfRadToDeg::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( rt->GetValue( 0 )->GetFloat() * RAD_TO_DEG );
}

// public static func float linearStep( float x, float lower, float upper )
deClassMath::nfLinearStep::nfLinearStep( const sInitData &init ) : dsFunction( init.clsMath, "linearStep", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
}
void deClassMath::nfLinearStep::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float lower = rt->GetValue( 1 )->GetFloat();
	const float upper = rt->GetValue( 2 )->GetFloat();
	
	rt->PushFloat( decMath::linearStep( x, lower, upper ) );
}

// public static func float linearStep( float x, float lower, float upper, float mapLower, float mapUpper )
deClassMath::nfLinearStep2::nfLinearStep2( const sInitData &init ) : dsFunction( init.clsMath,
"linearStep", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
	p_AddParameter( init.clsFlt ); // mapLower
	p_AddParameter( init.clsFlt ); // mapUpper
}
void deClassMath::nfLinearStep2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float lower = rt->GetValue( 1 )->GetFloat();
	const float upper = rt->GetValue( 2 )->GetFloat();
	const float mapLower = rt->GetValue( 3 )->GetFloat();
	const float mapUpper = rt->GetValue( 4 )->GetFloat();
	
	rt->PushFloat( decMath::linearStep( x, lower, upper, mapLower, mapUpper ) );
}

// public static func float smoothStep( float x, float lower, float upper )
deClassMath::nfSmoothStep::nfSmoothStep( const sInitData &init ) : dsFunction( init.clsMath, "smoothStep", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
}
void deClassMath::nfSmoothStep::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float lower = rt->GetValue( 1 )->GetFloat();
	const float upper = rt->GetValue( 2 )->GetFloat();
	
	rt->PushFloat( decMath::smoothStep( x, lower, upper ) );
}

// public static func float smoothStep( float x, float lower, float upper, float mapLower, float mapUpper )
deClassMath::nfSmoothStep2::nfSmoothStep2( const sInitData &init ) : dsFunction( init.clsMath, "smoothStep", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
	p_AddParameter( init.clsFlt ); // mapLower
	p_AddParameter( init.clsFlt ); // mapUpper
}
void deClassMath::nfSmoothStep2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float lower = rt->GetValue( 1 )->GetFloat();
	const float upper = rt->GetValue( 2 )->GetFloat();
	const float mapLower = rt->GetValue( 3 )->GetFloat();
	const float mapUpper = rt->GetValue( 4 )->GetFloat();
	
	rt->PushFloat( decMath::smoothStep( x, lower, upper, mapLower, mapUpper ) );
}

// public static func float normalize( float x, float lower, float upper )
deClassMath::nfNormalize::nfNormalize( const sInitData &init ) : dsFunction( init.clsMath,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
}
void deClassMath::nfNormalize::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float lower = rt->GetValue( 1 )->GetFloat();
	const float upper = rt->GetValue( 2 )->GetFloat();
	
	rt->PushFloat( decMath::normalize( x, lower, upper ) );
}

// public static func float normalize( float x, float upper )
deClassMath::nfNormalize2::nfNormalize2( const sInitData &init ) : dsFunction( init.clsMath,
"normalize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // x
	p_AddParameter( init.clsFlt ); // upper
}
void deClassMath::nfNormalize2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float x = rt->GetValue( 0 )->GetFloat();
	const float upper = rt->GetValue( 1 )->GetFloat();
	
	rt->PushFloat( decMath::normalize( x, upper ) );
}

// public static func float mix( float a, float b, float blend )
deClassMath::nfMix::nfMix( const sInitData &init ) : dsFunction( init.clsMath,
"mix", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // a
	p_AddParameter( init.clsFlt ); // b
	p_AddParameter( init.clsFlt ); // blend
}
void deClassMath::nfMix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float a = rt->GetValue( 0 )->GetFloat();
	const float b = rt->GetValue( 1 )->GetFloat();
	const float blend = rt->GetValue( 2 )->GetFloat();
	
	rt->PushFloat( decMath::mix( a, b, blend ) );
}

// public static func float random()
deClassMath::nfRandom::nfRandom( const sInitData &init ) : dsFunction( init.clsMath,
"random", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
}
void deClassMath::nfRandom::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushFloat( ( float )rand() / ( float )RAND_MAX );
}

// public static func int random( int lower, int upper )
deClassMath::nfRandomInt::nfRandomInt( const sInitData &init ) : dsFunction( init.clsMath,
"random", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt ){
	p_AddParameter( init.clsInt ); // lower
	p_AddParameter( init.clsInt ); // upper
}
void deClassMath::nfRandomInt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const int lower = rt->GetValue( 0 )->GetInt();
	const int upper = rt->GetValue( 1 )->GetInt();
	
	if( upper > lower ){
		const float factor2 = ( float )rand() / ( float )RAND_MAX;
		const float factor1 = 1.0f - factor2;
		rt->PushInt( decMath::clamp( ( int )( ( float )lower * factor1
			+ ( float )( upper + 1 ) * factor2 ), lower, upper ) );
		
	}else{
		rt->PushInt( lower );
	}
}

// public static func float random( float lower, float upper )
deClassMath::nfRandomFloat::nfRandomFloat( const sInitData &init ) : dsFunction( init.clsMath,
"random", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFlt ){
	p_AddParameter( init.clsFlt ); // lower
	p_AddParameter( init.clsFlt ); // upper
}
void deClassMath::nfRandomFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float lower = rt->GetValue( 0 )->GetFloat();
	const float upper = rt->GetValue( 1 )->GetFloat();
	
	if( upper > lower ){
		const float factor2 = ( float )rand() / ( float )RAND_MAX;
		const float factor1 = 1.0f - factor2;
		rt->PushFloat( decMath::clamp( lower * factor1 + upper * factor2, lower, upper ) );
		
	}else{
		rt->PushFloat( lower );
	}
}

// public static func bool probability( float threshold )
deClassMath::nfProbability::nfProbability( const sInitData &init ) : dsFunction( init.clsMath,
"probability", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsFlt ); // threshold
}
void deClassMath::nfProbability::RunFunction( dsRunTime *rt, dsValue *myself ){
	const float threshold = rt->GetValue( 0 )->GetFloat();
	
	if( threshold <= 0.0f ){
		rt->PushBool( false );
		
	}else if( threshold >= 1.0f ){
		rt->PushBool( true );
		
	}else{
		rt->PushBool( ( ( float )rand() / ( float )RAND_MAX ) < threshold );
	}
}



// Class deClassMath
//////////////////////

// Constructor
////////////////

deClassMath::deClassMath( deScriptingDragonScript *ds ) : dsClass( "DEMath", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Math" );
	
	p_SetNativeDataSize( 0 );
}

deClassMath::~deClassMath(){
}



// Management
///////////////

void deClassMath::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsMath = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	
	// add functions
	AddFunction( new nfacos( init ) );
	AddFunction( new nfasin( init ) );
	AddFunction( new nfatan( init ) );
	AddFunction( new nfatan2( init ) );
	AddFunction( new nfcos( init ) );
	AddFunction( new nfcosh( init ) );
	AddFunction( new nfsin( init ) );
	AddFunction( new nfsinh( init ) );
	AddFunction( new nftan( init ) );
	AddFunction( new nftanh( init ) );
	
	AddFunction( new nfDegToRad( init ) );
	AddFunction( new nfRadToDeg( init ) );
	AddFunction( new nfLinearStep( init ) );
	AddFunction( new nfLinearStep2( init ) );
	AddFunction( new nfSmoothStep( init ) );
	AddFunction( new nfSmoothStep2( init ) );
	AddFunction( new nfNormalize( init ) );
	AddFunction( new nfNormalize2( init ) );
	AddFunction( new nfMix( init ) );
	AddFunction( new nfRandom( init ) );
	AddFunction( new nfRandomInt( init ) );
	AddFunction( new nfRandomFloat( init ) );
	AddFunction( new nfProbability( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}
