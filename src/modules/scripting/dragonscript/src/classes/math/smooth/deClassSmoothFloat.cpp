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

#include "deClassSmoothFloat.h"
#include "../../file/deClassFileReader.h"
#include "../../file/deClassFileWriter.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/math/smooth/decSmoothFloat.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>

#include <libdscript/exceptions.h>



// Native structure
/////////////////////

struct sSmFltNatDat{
	decSmoothFloat *smoothFloat;
};



// Native Functions
/////////////////////

// Constructors, Destructor
/////////////////////////////

// public func new()
deClassSmoothFloat::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSmFloat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSmoothFloat::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSmFltNatDat &nd = *( ( sSmFltNatDat* )p_GetNativeData( myself ) );
	
	nd.smoothFloat = NULL;
	
	nd.smoothFloat = new decSmoothFloat;
}

// public func new( SmoothFloat smoothFloat )
deClassSmoothFloat::nfNewCopy::nfNewCopy( const sInitData &init ) : dsFunction( init.clsSmFloat,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSmFloat ); // smoothFloat
}
void deClassSmoothFloat::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSmFltNatDat &nd = *( ( sSmFltNatDat* )p_GetNativeData( myself ) );
	deClassSmoothFloat &clsSmoothFloat = *( ( deClassSmoothFloat* )GetOwnerClass() );
	
	nd.smoothFloat = NULL;
	
	const decSmoothFloat &copy = clsSmoothFloat.GetSmoothFloat( rt->GetValue( 0 )->GetRealObject() );
	nd.smoothFloat = new decSmoothFloat( copy );
}


// public func destructor()
deClassSmoothFloat::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSmFloat,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSmoothFloat::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSmFltNatDat &nd = *( ( sSmFltNatDat* )p_GetNativeData( myself ) );
	
	if( nd.smoothFloat ){
		delete nd.smoothFloat;
		nd.smoothFloat = NULL;
	}
}



// Accessors
//////////////

// public func float getValue()
deClassSmoothFloat::nfGetValue::nfGetValue( const sInitData &init ) : dsFunction( init.clsSmFloat,
"getValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassSmoothFloat::nfGetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	rt->PushFloat( smoothFloat.GetValue() );
}

// public func void setValue( float value )
deClassSmoothFloat::nfSetValue::nfSetValue( const sInitData &init ) : dsFunction( init.clsSmFloat,
"setValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassSmoothFloat::nfSetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.SetValue( rt->GetValue( 0 )->GetFloat() );
}

// public func float getGoal()
deClassSmoothFloat::nfGetGoal::nfGetGoal( const sInitData &init ) : dsFunction( init.clsSmFloat,
"getGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassSmoothFloat::nfGetGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	rt->PushFloat( smoothFloat.GetGoal() );
}

// public func void setGoal( float goal )
deClassSmoothFloat::nfSetGoal::nfSetGoal( const sInitData &init ) : dsFunction( init.clsSmFloat,
"setGoal", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassSmoothFloat::nfSetGoal::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.SetGoal( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustTime()
deClassSmoothFloat::nfGetAdjustTime::nfGetAdjustTime( const sInitData &init ) : dsFunction( init.clsSmFloat,
"getAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassSmoothFloat::nfGetAdjustTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	rt->PushFloat( smoothFloat.GetAdjustTime() );
}

// public void setAdjustTime( float adjustTime )
deClassSmoothFloat::nfSetAdjustTime::nfSetAdjustTime( const sInitData &init ) : dsFunction( init.clsSmFloat,
"setAdjustTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassSmoothFloat::nfSetAdjustTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.SetAdjustTime( rt->GetValue( 0 )->GetFloat() );
}

// public func float getAdjustRange()
deClassSmoothFloat::nfGetAdjustRange::nfGetAdjustRange( const sInitData &init ) : dsFunction( init.clsSmFloat,
"getAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassSmoothFloat::nfGetAdjustRange::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	rt->PushFloat( smoothFloat.GetAdjustRange() );
}

// public func void setAdjustRange( float limit )
deClassSmoothFloat::nfSetAdjustRange::nfSetAdjustRange( const sInitData &init ) : dsFunction( init.clsSmFloat,
"setAdjustRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassSmoothFloat::nfSetAdjustRange::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.SetAdjustRange( rt->GetValue( 0 )->GetFloat() );
}

// public func float getChangeSpeed()
deClassSmoothFloat::nfGetChangeSpeed::nfGetChangeSpeed( const sInitData &init ) : dsFunction( init.clsSmFloat,
"getChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassSmoothFloat::nfGetChangeSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	rt->PushFloat( smoothFloat.GetChangeSpeed() );
}

// public func void setChangeSpeed( float changeSpeed )
deClassSmoothFloat::nfSetChangeSpeed::nfSetChangeSpeed( const sInitData &init ) : dsFunction( init.clsSmFloat,
"setChangeSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // value
}
void deClassSmoothFloat::nfSetChangeSpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.SetChangeSpeed( rt->GetValue( 0 )->GetFloat() );
}



// public func void reset()
deClassSmoothFloat::nfReset::nfReset( const sInitData &init ) : dsFunction( init.clsSmFloat,
"reset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSmoothFloat::nfReset::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.Reset();
}

// public func void update( float elapsed )
deClassSmoothFloat::nfUpdate::nfUpdate( const sInitData &init ) : dsFunction( init.clsSmFloat,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // elapsed
}
void deClassSmoothFloat::nfUpdate::RunFunction( dsRunTime *rt, dsValue *myself ){
	decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	
	smoothFloat.Update( rt->GetValue( 0 )->GetFloat() );
}



// File Handling
//////////////////

// static public func SmoothFloat readFromFile( FileReader reader )
deClassSmoothFloat::nfReadFromFile::nfReadFromFile( const sInitData &init ) : dsFunction( init.clsSmFloat,
"readFromFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSmFloat ){
	p_AddParameter( init.clsFileReader ); // reader
}
void deClassSmoothFloat::nfReadFromFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassSmoothFloat &clsSmoothFloat = *( ( deClassSmoothFloat* )GetOwnerClass() );
	const deClassFileReader &clsFileReader = *clsSmoothFloat.GetDS().GetClassFileReader();
	decBaseFileReader * const reader = clsFileReader.GetFileReader( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! reader ){
		DSTHROW( dueNullPointer );
	}
	
	decSmoothFloat smoothFloat;
	const int version = reader->ReadByte();
	
	switch( version ){
	case 0:
		smoothFloat.SetValue( reader->ReadFloat() );
		smoothFloat.SetGoal( reader->ReadFloat() );
		smoothFloat.SetAdjustRange( reader->ReadFloat() );
		smoothFloat.SetAdjustTime( reader->ReadFloat() );
		smoothFloat.SetChangeSpeed( reader->ReadFloat() );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	clsSmoothFloat.PushSmoothFloat( rt, smoothFloat );
}

// public func void writeToFile( FileWriter writer )
deClassSmoothFloat::nfWriteToFile::nfWriteToFile( const sInitData &init ) : dsFunction( init.clsSmFloat,
"writeToFile", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFileWriter ); // writer
}
void deClassSmoothFloat::nfWriteToFile::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	const deClassSmoothFloat &clsSmoothFloat = *( ( deClassSmoothFloat* )GetOwnerClass() );
	const deClassFileWriter &clsFileWriter = *clsSmoothFloat.GetDS().GetClassFileWriter();
	decBaseFileWriter * const writer = clsFileWriter.GetFileWriter( rt->GetValue( 0 )->GetRealObject() );
	
	if( ! writer ){
		DSTHROW( dueNullPointer );
	}
	
	writer->WriteByte( 0 ); // version 0
	writer->WriteFloat( smoothFloat.GetValue() );
	writer->WriteFloat( smoothFloat.GetGoal() );
	writer->WriteFloat( smoothFloat.GetAdjustRange() );
	writer->WriteFloat( smoothFloat.GetAdjustTime() );
	writer->WriteFloat( smoothFloat.GetChangeSpeed() );
}



// Special Functions
//////////////////////

// public func bool equals( Object other )
deClassSmoothFloat::nfEquals::nfEquals( const sInitData &init ) : dsFunction( init.clsSmFloat,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // other
}
void deClassSmoothFloat::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	deClassSmoothFloat * const clsSmoothFloat = ( deClassSmoothFloat* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsSmoothFloat ) ){
		rt->PushBool( false );
		
	}else{
		const decSmoothFloat &otherSmoothFloat = clsSmoothFloat->GetSmoothFloat( obj->GetRealObject() );
		rt->PushBool( smoothFloat == otherSmoothFloat );
	}
}

// public func String toString()
deClassSmoothFloat::nfToString::nfToString( const sInitData &init ) : dsFunction( init.clsSmFloat,
"toString", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsStr ){
}
void deClassSmoothFloat::nfToString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const decSmoothFloat &smoothFloat = *( ( sSmFltNatDat* )p_GetNativeData( myself ) )->smoothFloat;
	decString str;
	
	str.Format( "%g", smoothFloat.GetValue() );
	
	rt->PushString( str );
}



// Class deClassSmoothFloat
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassSmoothFloat::deClassSmoothFloat( deScriptingDragonScript &ds ) :
dsClass( "SmoothFloat", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSmFltNatDat ) );
}

deClassSmoothFloat::~deClassSmoothFloat(){
}



// Management
///////////////

void deClassSmoothFloat::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSmFloat = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsFileReader = pDS.GetClassFileReader();
	init.clsFileWriter = pDS.GetClassFileWriter();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetValue( init ) );
	AddFunction( new nfSetValue( init ) );
	AddFunction( new nfGetGoal( init ) );
	AddFunction( new nfSetGoal( init ) );
	AddFunction( new nfGetAdjustTime( init ) );
	AddFunction( new nfSetAdjustTime( init ) );
	AddFunction( new nfGetAdjustRange( init ) );
	AddFunction( new nfSetAdjustRange( init ) );
	AddFunction( new nfGetChangeSpeed( init ) );
	AddFunction( new nfSetChangeSpeed( init ) );
	
	AddFunction( new nfReset( init ) );
	AddFunction( new nfUpdate( init ) );
	
	AddFunction( new nfReadFromFile( init ) );
	AddFunction( new nfWriteToFile( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfToString( init ) );
}

const decSmoothFloat &deClassSmoothFloat::GetSmoothFloat( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueNullPointer );
	}
	
	return *( ( sSmFltNatDat* )p_GetNativeData( myself->GetBuffer() ) )->smoothFloat;
}

void deClassSmoothFloat::PushSmoothFloat( dsRunTime *rt, const decSmoothFloat &smoothFloat ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sSmFltNatDat &nd = *( ( sSmFltNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.smoothFloat = NULL;
	
	try{
		nd.smoothFloat = new decSmoothFloat( smoothFloat );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
