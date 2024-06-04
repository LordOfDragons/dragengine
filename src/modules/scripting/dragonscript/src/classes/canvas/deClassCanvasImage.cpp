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
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCanvas.h"
#include "deClassCanvasImage.h"
#include "../graphics/deClassImage.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasManager.h>

#include <libdscript/exceptions.h>



// Native Structure
struct sCImageNatDat{
	deCanvasImage *canvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCanvasImage::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCImage,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasImage::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasImage* )GetOwnerClass() )->GetDS();
	
	// clear ( important )
	nd.canvas = NULL;
	
	// super call
	deClassCanvas * const baseClass = ( deClassCanvas* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create canvas
	nd.canvas = ds.GetGameEngine()->GetCanvasManager()->CreateCanvasImage();
	baseClass->AssignCanvas( myself->GetRealObject(), nd.canvas );
}

// public func destructor()
deClassCanvasImage::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCImage,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCanvasImage::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	
	if( nd.canvas ){
		nd.canvas->FreeReference();
		nd.canvas = NULL;
	}
}



// Management all types
/////////////////////////

// public func Image getImage()
deClassCanvasImage::nfGetImage::nfGetImage( const sInitData &init ) : dsFunction( init.clsCImage,
"getImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassCanvasImage::nfGetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasImage* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, nd.canvas->GetImage() );
}

// public func void setImage( Image image )
deClassCanvasImage::nfSetImage::nfSetImage( const sInitData &init ) : dsFunction( init.clsCImage,
"setImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsImage ); // image
}
void deClassCanvasImage::nfSetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCanvasImage* )GetOwnerClass() )->GetDS();
	
	nd.canvas->SetImage( ds.GetClassImage()->GetImage( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func int getRepeatX()
deClassCanvasImage::nfGetRepeatX::nfGetRepeatX( const sInitData &init ) : dsFunction( init.clsCImage,
"getRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasImage::nfGetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatX() );
}

// public func void setRepeatX( int count )
deClassCanvasImage::nfSetRepeatX::nfSetRepeatX( const sInitData &init ) : dsFunction( init.clsCImage,
"setRepeatX", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasImage::nfSetRepeatX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatX( rt->GetValue( 0 )->GetInt() );
}

// public func int getRepeatY()
deClassCanvasImage::nfGetRepeatY::nfGetRepeatY( const sInitData &init ) : dsFunction( init.clsCImage,
"getRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}
void deClassCanvasImage::nfGetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	rt->PushInt( nd.canvas->GetRepeatY() );
}

// public func void setRepeatY( int count )
deClassCanvasImage::nfSetRepeatY::nfSetRepeatY( const sInitData &init ) : dsFunction( init.clsCImage,
"setRepeatY", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // count
}
void deClassCanvasImage::nfSetRepeatY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( myself ) );
	nd.canvas->SetRepeatY( rt->GetValue( 0 )->GetInt() );
}



// public func int hashCode()
deClassCanvasImage::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCImage, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCanvasImage::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasImage * const canvas = ( ( sCImageNatDat* )p_GetNativeData( myself ) )->canvas;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )canvas );
}

// public func bool equals( Object obj )
deClassCanvasImage::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCImage, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassCanvasImage::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCanvasImage * const canvas = ( ( sCImageNatDat* )p_GetNativeData( myself ) )->canvas;
	deClassCanvasImage * const clsCImage = ( deClassCanvasImage* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCImage ) ){
		rt->PushBool( false );
		
	}else{
		deCanvasImage * const otherCanvas = ( ( sCImageNatDat* )p_GetNativeData( obj ) )->canvas;
		rt->PushBool( canvas == otherCanvas );
	}
}



// Class deClassCanvasImage
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCanvasImage::deClassCanvasImage( deScriptingDragonScript &ds ) :
dsClass( "CanvasImage", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Canvas" );
	
	p_SetNativeDataSize( sizeof( sCImageNatDat ) );
}

deClassCanvasImage::~deClassCanvasImage(){
}



// Management
///////////////

void deClassCanvasImage::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCImage = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsImage = pDS.GetClassImage();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetImage( init ) );
	AddFunction( new nfSetImage( init ) );
	AddFunction( new nfGetRepeatX( init ) );
	AddFunction( new nfSetRepeatX( init ) );
	AddFunction( new nfGetRepeatY( init ) );
	AddFunction( new nfSetRepeatY( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCanvasImage *deClassCanvasImage::GetCanvas( dsRealObject *myself ) const {
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCImageNatDat* )p_GetNativeData( myself->GetBuffer() ) )->canvas;
}

void deClassCanvasImage::PushCanvas( dsRunTime *rt, deCanvasImage *canvas ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! canvas ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassCanvas * const baseClass = ( deClassCanvas* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sCImageNatDat &nd = *( ( sCImageNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.canvas = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		canvas->AddReference();
		nd.canvas = canvas;
		
		baseClass->AssignCanvas( rt->GetValue( 0 )->GetRealObject(), canvas );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
