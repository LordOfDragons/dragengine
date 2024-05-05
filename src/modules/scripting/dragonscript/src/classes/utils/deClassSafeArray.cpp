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
#include <stdint.h>

#include "deClassSafeArray.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/collection/decPointerList.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassBlock.h>



struct sSafeArrayIterator{
	int next;
	int end;
};

struct sSafeArrayNatDat{
	decPointerList *array;
	decPointerList *iterators;
	
	int IndexOf( dsValue *value ) const{
		dsRealObject * const object = value->GetRealObject();
		const int count = array->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( ( ( dsValue* )array->GetAt( i ) )->GetRealObject() == object ){
				return i;
			}
		}
		
		return -1;
	};
	
	void RemoveIndex( dsRunTime &rt, int index ){
		// remove value pointer
		dsValue * const value = ( dsValue* )array->GetAt( index );
		array->RemoveFrom( index );
		
		// update iterators
		UpdateIteratorsRemove( index );
		
		// free value
		rt.FreeValue( value );
	}
	
	void UpdateIteratorsRemove( int removeIndex ){
		const int iterCount = iterators->GetCount();
		int i;
		
		for( i=0; i<iterCount; i++ ){
			sSafeArrayIterator &iter = *( ( sSafeArrayIterator* )iterators->GetAt( i ) );
			if( removeIndex < iter.next ){
				iter.next--;
			}
			if( removeIndex < iter.end ){
				iter.end--;
			}
		}
	}
};



// Constructors, Destructors
//////////////////////////////

// public func new()
deClassSafeArray::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSA,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSafeArray::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	nd.array = NULL;
	nd.iterators = NULL;
	
	try{
		nd.array = new decPointerList;
		nd.iterators = new decPointerList;
		
	}catch( ... ){
		if( nd.iterators ){
			delete nd.iterators;
			nd.iterators = NULL;
		}
		if( nd.array ){
			delete nd.array;
			nd.array = NULL;
		}
		throw;
	}
}

// public func destructor()
deClassSafeArray::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSA,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSafeArray::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	if( nd.iterators ){
		delete nd.iterators;
		nd.iterators = NULL;
	}
	
	if( nd.array ){
		int count = nd.array->GetCount();
		while( count > 0 ){
			count--;
			rt->FreeValue( ( dsValue* )nd.array->GetAt( count ) );
			nd.array->RemoveFrom( count );
		}
		delete nd.array;
		nd.array = NULL;
	}
}



// Management
///////////////

// public func int getCount()
deClassSafeArray::nfGetCount::nfGetCount( const sInitData &init ) : dsFunction( init.clsSA,
"getCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassSafeArray::nfGetCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.array->GetCount() );
}

// public func bool has( Object element )
deClassSafeArray::nfHas::nfHas( const sInitData &init ) : dsFunction( init.clsSA,
"has", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfHas::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsRealObject * const obj = rt->GetValue( 0 )->GetRealObject();
	
	if( obj ){
		rt->PushBool( nd.IndexOf( rt->GetValue( 0 ) ) != -1 );
		
	}else{
		rt->PushBool( false );
	}
}

// public func int indexOf( Object element )
deClassSafeArray::nfIndexOf::nfIndexOf( const sInitData &init ) : dsFunction( init.clsSA,
"indexOf", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfIndexOf::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.IndexOf( rt->GetValue( 0 ) ) );
}

// public func Object getAt( int index )
deClassSafeArray::nfGetAt::nfGetAt( const sInitData &init ) : dsFunction( init.clsSA,
"getAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsObj ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSafeArray::nfGetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	const int index = rt->GetValue( 0 )->GetInt();
	dsValue &value = *( ( dsValue* )nd.array->GetAt( index ) );
	rt->PushObject( value.GetRealObject() );
}

// public func void setAt( int index, Object element )
deClassSafeArray::nfSetAt::nfSetAt( const sInitData &init ) : dsFunction( init.clsSA,
"setAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfSetAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	const int index = rt->GetValue( 0 )->GetInt();
	dsValue * const valueTo = ( dsValue* )nd.array->GetAt( index );
	
	dsValue * const valueFrom = rt->GetValue( 1 );
	if( ! valueFrom->GetRealObject() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CopyValue( valueFrom, valueTo );
}



// public func void add( Object element )
deClassSafeArray::nfAdd::nfAdd( const sInitData &init ) : dsFunction( init.clsSA,
"add", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfAdd::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsRealObject * const obj = rt->GetValue( 0 )->GetRealObject();
	
	if( nd.IndexOf( rt->GetValue( 0 ) ) != -1 ){
		DSTHROW( dueInvalidParam );
	}
	
	dsValue *value = NULL;
	
	try{
		value = rt->CreateValue( rt->GetEngine()->GetClassObject() );
		rt->SetObject( value, obj );
		nd.array->Add( value );
		
	}catch( ... ){
		if( value ){
			rt->FreeValue( value );
			throw;
		}
	}
}

// public func void addIfAbsent( Object element )
deClassSafeArray::nfAddIfAbsent::nfAddIfAbsent( const sInitData &init ) : dsFunction( init.clsSA,
"addIfAbsent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfAddIfAbsent::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsRealObject * const obj = rt->GetValue( 0 )->GetRealObject();
	
	if( nd.IndexOf( rt->GetValue( 0 ) ) != -1 ){
		return;
	}
	
	dsValue *value = NULL;
	
	try{
		value = rt->CreateValue( rt->GetEngine()->GetClassObject() );
		rt->SetObject( value, obj );
		nd.array->Add( value );
		
	}catch( ... ){
		if( value ){
			rt->FreeValue( value );
			throw;
		}
	}
}

// public func void remove( Object element )
deClassSafeArray::nfRemove::nfRemove( const sInitData &init ) : dsFunction( init.clsSA,
"remove", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfRemove::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	const int index = nd.IndexOf( rt->GetValue( 0 ) );
	if( index == -1 ){
		DSTHROW( dueInvalidParam );
	}
	
	nd.RemoveIndex( *rt, index );
}

// public func void removeFrom( int index )
deClassSafeArray::nfRemoveFrom::nfRemoveFrom( const sInitData &init ) :
dsFunction( init.clsSA, "removeFrom", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // index
}
void deClassSafeArray::nfRemoveFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	const int index = rt->GetValue( 0 )->GetInt();
	if( index < 0 || index >= nd.array->GetCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	nd.RemoveIndex( *rt, index );
}

// public func void removeIfPresent( Object element )
deClassSafeArray::nfRemoveIfPresent::nfRemoveIfPresent( const sInitData &init ) : dsFunction( init.clsSA,
"removeIfPresent", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsObj ); // element
}
void deClassSafeArray::nfRemoveIfPresent::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	if( ! nd.array ){
		return;
	}
	
	const int index = nd.IndexOf( rt->GetValue( 0 ) );
	if( index == -1 ){
		return;
	}
	
	nd.RemoveIndex( *rt, index );
}

// public func void removeAll()
deClassSafeArray::nfRemoveAll::nfRemoveAll( const sInitData &init ) : dsFunction( init.clsSA,
"removeAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSafeArray::nfRemoveAll::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	if( ! nd.array ){
		return;
	}
	
	while( nd.array->GetCount() > 0 ){
		nd.RemoveIndex( *rt, nd.array->GetCount() - 1 );
	}
}

// public func void removeAll( Block ablock )
deClassSafeArray::nfRemoveAll2::nfRemoveAll2( const sInitData &init ) : dsFunction( init.clsSA,
"removeAll", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBlock ); // ablock
}
void deClassSafeArray::nfRemoveAll2::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsValue * const valueBlock = rt->GetValue( 0 );
	if( ! valueBlock->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	if( ! nd.array ){
		return;
	}
	
	const int funcIndexRun = ( ( dsClassBlock* )rt->GetEngine()->GetClassBlock() )->GetFuncIndexRun1();
	
	sSafeArrayIterator iterator;
	iterator.end = nd.array->GetCount();
	iterator.next = 0;
	
	nd.iterators->Add( &iterator );
	
	try{
		while( iterator.end > 0 ){
			// apply block to element to remove
			iterator.next = iterator.end - 1;
			
			rt->PushValue( ( dsValue* )nd.array->GetAt( iterator.next ) );
			rt->RunFunctionFast( valueBlock, funcIndexRun ); // Object run( Object )
			
			// check if the element is still existing. if the block removed object do nothing
			if( iterator.next == iterator.end - 1 ){
				nd.RemoveIndex( *rt, iterator.next );
			}
		}
		
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		
	}catch( ... ){
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		throw;
	}
}



// public func void forEach( Block ablock )
deClassSafeArray::nfForEach::nfForEach( const sInitData &init ) : dsFunction( init.clsSA,
"forEach", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBlock ); // ablock
}
void deClassSafeArray::nfForEach::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsValue * const valueBlock = rt->GetValue( 0 );
	if( ! valueBlock->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	if( nd.array->GetCount() == 0 ){
		return;
	}
	
	const int funcIndexRun = ( ( dsClassBlock* )rt->GetEngine()->GetClassBlock() )->GetFuncIndexRun1();
	
	sSafeArrayIterator iterator;
	iterator.next = 0;
	iterator.end = nd.array->GetCount();
	
	// todo: optimize these calls by using fast call to the run method
	nd.iterators->Add( &iterator );
	
	try{
		while( iterator.next < iterator.end ){
			rt->PushValue( ( dsValue* )nd.array->GetAt( iterator.next ) );
			iterator.next++; // has to be done before running the method since next can be modified
			rt->RunFunctionFast( valueBlock, funcIndexRun ); // Object run( Object )
		}
		
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		
	}catch( ... ){
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		throw;
	}
}

// public func Object find( Block ablock )
deClassSafeArray::nfFind::nfFind( const sInitData &init ) : dsFunction( init.clsSA,
"find", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsObj ){
	p_AddParameter( init.clsBlock ); // ablock
}
void deClassSafeArray::nfFind::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSafeArrayNatDat &nd = *( ( sSafeArrayNatDat* )p_GetNativeData( myself ) );
	
	dsValue * const valueBlock = rt->GetValue( 0 );
	if( ! valueBlock->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	if( nd.array->GetCount() == 0 ){
		rt->PushObject( NULL, rt->GetEngine()->GetClassObject() );
		return;
	}
	
	sSafeArrayIterator iterator;
	iterator.next = 0;
	iterator.end = nd.array->GetCount();
	
	// NOTE the variable found is required. testing if iterator.next >= iterator.end fails for
	//      the last element in the array since iterator.next has to be incremented before
	//      doing the test where the match is found.
	const int funcIndexRun = ( ( dsClassBlock* )rt->GetEngine()->GetClassBlock() )->GetFuncIndexRun1();
	
	dsClass * const clsBool = rt->GetEngine()->GetClassBool();
	dsValue *tempValue = NULL;
	bool found = false;
	
	nd.iterators->Add( &iterator );
	
	try{
		tempValue = rt->CreateValue();
		
		while( iterator.next < iterator.end ){
			rt->CopyValue( ( dsValue* )nd.array->GetAt( iterator.next ), tempValue );
			
			rt->PushValue( tempValue );
			iterator.next++; // has to be done before running the method since next can be modified
			rt->RunFunctionFast( valueBlock, funcIndexRun ); // Object run( Object )
			if( rt->GetReturnValue()->GetType() != clsBool ){
				DSTHROW( dseInvalidCast );
			}
			
			if( rt->GetReturnBool() ){
				rt->PushValue( tempValue );
				found = true;
				break;
			}
			
		}
		
		if( ! found ){
			rt->PushObject( NULL, rt->GetEngine()->GetClassObject() );
		}
		
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		rt->FreeValue( tempValue );
		
	}catch( ... ){
		nd.iterators->RemoveFrom( nd.iterators->IndexOf( &iterator ) );
		if( tempValue ){
			rt->FreeValue( tempValue );
		}
		throw;
	}
}



// Class deClassSafeArray
///////////////////////////

// Constructor
////////////////

deClassSafeArray::deClassSafeArray( deScriptingDragonScript *ds ) :
dsClass( "SafeArray", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ){
	if( ! ds ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS = ds;
	
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSafeArrayNatDat ) );
}

deClassSafeArray::~deClassSafeArray(){
}



// Management
///////////////

void deClassSafeArray::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSA = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsBlock = engine->GetClassBlock();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCount( init ) );
	AddFunction( new nfHas( init ) );
	AddFunction( new nfIndexOf( init ) );
	AddFunction( new nfGetAt( init ) );
	AddFunction( new nfSetAt( init ) );
	AddFunction( new nfAdd( init ) );
	AddFunction( new nfAddIfAbsent( init ) );
	AddFunction( new nfRemove( init ) );
	AddFunction( new nfRemoveFrom( init ) );
	AddFunction( new nfRemoveIfPresent( init ) );
	AddFunction( new nfRemoveAll( init ) );
	AddFunction( new nfRemoveAll2( init ) );
	
	AddFunction( new nfForEach( init ) );
	AddFunction( new nfFind( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}
