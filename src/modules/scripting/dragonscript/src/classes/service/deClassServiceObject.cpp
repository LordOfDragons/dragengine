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

#include <stdint.h>

#include "deClassServiceObject.h"
#include "../collider/deClassCollider.h"
#include "../file/deClassMemoryFile.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceObject.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sServiceObjectNatDat{
	deServiceObject *object;
};



// Native functions
/////////////////////

// func new()
deClassServiceObject::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsServiceObject, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassServiceObject::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	
	// clear (important)
	nd.object = nullptr;
	
	// create object
	nd.object = new deServiceObject;
}


// static func ServiceObject newBool(bool value)
deClassServiceObject::nfNewBool::nfNewBool( const sInitData &init ) :
dsFunction( init.clsServiceObject, "newBool", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsBool ); // value
}

void deClassServiceObject::nfNewBool::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	
	const bool value = rt->GetValue( 0 )->GetBool();
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewBool( value ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// static func ServiceObject newInt(int value)
deClassServiceObject::nfNewInteger::nfNewInteger( const sInitData &init ) :
dsFunction( init.clsServiceObject, "newInt", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsInteger ); // value
}

void deClassServiceObject::nfNewInteger::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	
	const int value = rt->GetValue( 0 )->GetInt();
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewInt( value ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// static func ServiceObject newFloat(float value)
deClassServiceObject::nfNewFloat::nfNewFloat( const sInitData &init ) :
dsFunction( init.clsServiceObject, "newFloat", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsFloat ); // value
}

void deClassServiceObject::nfNewFloat::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	
	const float value = rt->GetValue( 0 )->GetFloat();
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewFloat( ( double )value ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// static func ServiceObject newString(String value)
deClassServiceObject::nfNewString::nfNewString( const sInitData &init ) :
dsFunction( init.clsServiceObject, "nfNewString", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsString ); // value
}

void deClassServiceObject::nfNewString::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	
	const char * const value = rt->GetValue( 0 )->GetString();
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewString( value ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// static func ServiceObject newResource(Object value)
deClassServiceObject::nfNewResource::nfNewResource( const sInitData &init ) :
dsFunction( init.clsServiceObject, "newResource", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsObject ); // value
}

void deClassServiceObject::nfNewResource::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	deScriptingDragonScript &ds = clsServiceObject.GetDS();
	
	deResource * const value = ds.GetClassCollider()->GetResource( *rt->GetValue( 0 ) );
	if( ! value ){
		DSTHROW( dueNullPointer );
	}
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewResource( deResource::Ref( value ) ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// static func ServiceObject newData(MemoryFile value)
deClassServiceObject::nfNewData::nfNewData( const sInitData &init ) :
dsFunction( init.clsServiceObject, "newData", DSFT_FUNCTION,
DSTM_STATIC | DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsMemoryFile ); // value
}

void deClassServiceObject::nfNewData::RunFunction( dsRunTime *rt, dsValue* ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	deScriptingDragonScript &ds = clsServiceObject.GetDS();
	
	decMemoryFile * const value = ds.GetClassMemoryFile()->
		GetMemoryFile( rt->GetValue( 0 )->GetRealObject() );
	if( ! value ){
		DSTHROW( dueNullPointer );
	}
	
	const deServiceObject::Ref object( deServiceObject::Ref::New(
		deServiceObject::NewData( decMemoryFile::Ref( value ) ) ) );
	
	clsServiceObject.PushServiceObject( rt, object );
}


// new(ServiceObject copy, bool deep)
deClassServiceObject::nfNewCopy::nfNewCopy( const sInitData &init ) :
dsFunction( init.clsServiceObject, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsServiceObject ); // copy
	p_AddParameter( init.clsBool ); // deep
}

void deClassServiceObject::nfNewCopy::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	
	// clear (important)
	nd.object = nullptr;
	
	// create object
	deServiceObject * const copy = clsServiceObject.GetServiceObject( rt->GetValue( 0 )->GetRealObject() );
	const bool deep = rt->GetValue( 1 )->GetBool();
	if( ! copy ){
		DSTHROW( dueNullPointer );
	}
	
	nd.object = new deServiceObject( *copy, deep );
}


// public func destructor()
deClassServiceObject::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsServiceObject, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassServiceObject::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( nd.object ){
		nd.object->FreeReference();
		nd.object = nullptr;
	}
}



// Management
///////////////

// func ServiceObjectType getValueType()
deClassServiceObject::nfGetValueType::nfGetValueType( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getValueType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObjectType ){
}

void deClassServiceObject::nfGetValueType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushValue( ( ( deClassServiceObject* )GetOwnerClass() )->GetClassServiceObjectType()
		->GetVariable( nd.object->GetValueType() )->GetStaticValue() );
}



// func bool isBoolean()
deClassServiceObject::nfIsBoolean::nfIsBoolean( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isBoolean", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsBoolean::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsBoolean() );
}


// func bool getBoolean()
deClassServiceObject::nfGetBoolean::nfGetBoolean( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getBoolean", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfGetBoolean::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->GetBoolean() );
}



// func bool isInteger()
deClassServiceObject::nfIsInteger::nfIsInteger( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isInteger", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsInteger::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsInteger() );
}


// func int getInteger()
deClassServiceObject::nfGetInteger::nfGetInteger( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getInteger", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassServiceObject::nfGetInteger::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushInt( nd.object->GetInteger() );
}



// func bool isFloat()
deClassServiceObject::nfIsFloat::nfIsFloat( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsFloat() );
}


// func float getFloat()
deClassServiceObject::nfGetFloat::nfGetFloat( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getFloat", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}

void deClassServiceObject::nfGetFloat::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushFloat( nd.object->GetFloat() );
}



// func bool isString()
deClassServiceObject::nfIsString::nfIsString( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsString() );
}


// func String getString()
deClassServiceObject::nfGetString::nfGetString( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getString", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}

void deClassServiceObject::nfGetString::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushString( nd.object->GetString() );
}



// func bool isResource()
deClassServiceObject::nfIsResource::nfIsResource( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isResource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsResource::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsResource() );
}


// func Object getResource()
deClassServiceObject::nfGetResource::nfGetResource( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getResource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsObject ){
}

void deClassServiceObject::nfGetResource::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = ( ( deClassServiceObject* )GetOwnerClass() )->GetDS();
	ds.GetClassCollider()->PushResource( *rt, nd.object->GetResource() );
}



// func bool isData()
deClassServiceObject::nfIsData::nfIsData( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsData::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsData() );
}


// func MemoryFile getData()
deClassServiceObject::nfGetData::nfGetData( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getData", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMemoryFile ){
}

void deClassServiceObject::nfGetData::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	deScriptingDragonScript &ds = ( ( deClassServiceObject* )GetOwnerClass() )->GetDS();
	ds.GetClassMemoryFile()->PushMemoryFile( rt, nd.object->GetData() );
}



// func bool isDictionary()
deClassServiceObject::nfIsDictionary::nfIsDictionary( const sInitData &init ) :
dsFunction( init.clsServiceObject, "isDictionary", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}

void deClassServiceObject::nfIsDictionary::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushBool( nd.object->IsDictionary() );
}


// func int getChildCount()
deClassServiceObject::nfGetChildCount::nfGetChildCount( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getChildCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassServiceObject::nfGetChildCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushInt( nd.object->GetChildCount() );
}


// func Array getChildrenKeys()
deClassServiceObject::nfGetChildrenKeys::nfGetChildrenKeys( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getChildrenKeys", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsArray ){
}

void deClassServiceObject::nfGetChildrenKeys::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	const deScriptingDragonScript &ds = ( ( deClassServiceObject* )GetOwnerClass() )->GetDS();
	const decStringList keys( nd.object->GetChildrenKeys() );
	const dsEngine &sengine = *ds.GetScriptEngine();
	const int count = keys.GetCount();
	int i;
	
	dsValue * const valueKeys = rt->CreateValue( sengine.GetClassArray() );
	
	try{
		rt->CreateObject( valueKeys, sengine.GetClassArray(), 0 );
		
		for( i=0; i<count; i++ ){
			rt->PushString( keys.GetAt( i ) );
			rt->RunFunction( valueKeys, "add", 1 );
		}
		
		rt->PushValue( valueKeys );
		rt->FreeValue( valueKeys );
		
	}catch( ... ){
		if( valueKeys ){
			rt->FreeValue( valueKeys );
		}
		throw;
	}
}


// func ServiceObject getChildAt(String key)
deClassServiceObject::nfGetChildAt::nfGetChildAt( const sInitData &init ) :
dsFunction( init.clsServiceObject, "getChildAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsServiceObject ){
	p_AddParameter( init.clsString ); // key
}

void deClassServiceObject::nfGetChildAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	const char * const key = rt->GetValue( 0 )->GetString();
	
	clsServiceObject.PushServiceObject( rt, nd.object->GetChildAt( key ) );
}


// func void addChild(String key, ServiceObject value)
deClassServiceObject::nfAddChild::nfAddChild( const sInitData &init ) :
dsFunction( init.clsServiceObject, "addChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // key
	p_AddParameter( init.clsServiceObject ); // value
}

void deClassServiceObject::nfAddChild::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	DEASSERT_TRUE( nd.object->GetValueType() == deServiceObject::evtDictionary )
	
	deClassServiceObject &clsServiceObject = *( ( deClassServiceObject* )GetOwnerClass() );
	const char * const key = rt->GetValue( 0 )->GetString();
	deServiceObject * const value = clsServiceObject.GetServiceObject( rt->GetValue( 1 )->GetRealObject() );
	
	nd.object->AddChild( key, deServiceObject::Ref( value ) );
}


// func void removeChild(String key)
deClassServiceObject::nfRemoveChild::nfRemoveChild( const sInitData &init ) :
dsFunction( init.clsServiceObject, "removeChild", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsString ); // key
}

void deClassServiceObject::nfRemoveChild::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	DEASSERT_TRUE( nd.object->GetValueType() == deServiceObject::evtDictionary )
	
	const char * const key = rt->GetValue( 0 )->GetString();
	
	nd.object->RemoveChild( key );
}


// func void removeAllChildren()
deClassServiceObject::nfRemoveAllChildren::nfRemoveAllChildren( const sInitData &init ) :
dsFunction( init.clsServiceObject, "removeAllChildren", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}

void deClassServiceObject::nfRemoveAllChildren::RunFunction( dsRunTime*, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	DEASSERT_TRUE( nd.object->GetValueType() == deServiceObject::evtDictionary )
	
	nd.object->RemoveAllChildren();
}



// func int hashCode()
deClassServiceObject::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsServiceObject, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassServiceObject::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	rt->PushInt( ( int )( intptr_t )nd.object );
}


// func bool equals( Object obj )
deClassServiceObject::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsServiceObject, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}

void deClassServiceObject::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sServiceObjectNatDat &nd = *( ( sServiceObjectNatDat* )p_GetNativeData( myself ) );
	if( ! nd.object ){
		DSTHROW( dueNullPointer );
	}
	
	deClassServiceObject * const clsServiceObject = ( deClassServiceObject* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsServiceObject ) ){
		rt->PushBool( false );
		
	}else{
		deServiceObject * const other = ( ( sServiceObjectNatDat* )p_GetNativeData( obj ) )->object;
		rt->PushBool( nd.object == other );
	}
}


// Class deClassServiceObject
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassServiceObject::deClassServiceObject( deScriptingDragonScript &ds ) :
dsClass( "ServiceObject", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sServiceObjectNatDat ) );
}

deClassServiceObject::~deClassServiceObject(){
}



// Management
///////////////

void deClassServiceObject::CreateClassMembers( dsEngine *engine ){
	pClsServiceObjectType = engine->GetClass( "Dragengine.ServiceObjectType" );
	
	sInitData init;
	init.clsServiceObject = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsObject = engine->GetClassObject();
	init.clsArray = engine->GetClassArray();
	init.clsMemoryFile = pDS.GetClassMemoryFile();
	init.clsServiceObjectType = pClsServiceObjectType;
	
	AddFunction( new nfNew( init ) );
	AddFunction( new nfNewBool( init ) );
	AddFunction( new nfNewInteger( init ) );
	AddFunction( new nfNewFloat( init ) );
	AddFunction( new nfNewString( init ) );
	AddFunction( new nfNewResource( init ) );
	AddFunction( new nfNewData( init ) );
	AddFunction( new nfNewCopy( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetValueType( init ) );
	
	AddFunction( new nfIsBoolean( init ) );
	AddFunction( new nfGetBoolean( init ) );
	
	AddFunction( new nfIsInteger( init ) );
	AddFunction( new nfGetInteger( init ) );
	
	AddFunction( new nfIsFloat( init ) );
	AddFunction( new nfGetFloat( init ) );
	
	AddFunction( new nfIsString( init ) );
	AddFunction( new nfGetString( init ) );
	
	AddFunction( new nfIsResource( init ) );
	AddFunction( new nfGetResource( init ) );
	
	AddFunction( new nfIsData( init ) );
	AddFunction( new nfGetData( init ) );
	
	AddFunction( new nfIsDictionary( init ) );
	AddFunction( new nfGetChildCount( init ) );
	AddFunction( new nfGetChildrenKeys( init ) );
	AddFunction( new nfGetChildAt( init ) );
	AddFunction( new nfAddChild( init ) );
	AddFunction( new nfRemoveChild( init ) );
	AddFunction( new nfRemoveAllChildren( init ) );
	
	AddFunction( new nfHashCode( init ) );
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

deServiceObject *deClassServiceObject::GetServiceObject( dsRealObject *myself ) const{
	if( ! myself ){
		return nullptr;
	}
	
	return ( ( sServiceObjectNatDat* )p_GetNativeData( myself->GetBuffer() ) )->object;
}

void deClassServiceObject::PushServiceObject( dsRunTime *rt, deServiceObject *object ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! object ){
		rt->PushObject( nullptr, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sServiceObjectNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->object = object;
	object->AddReference();
}
