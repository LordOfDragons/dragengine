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
#include <stdint.h>
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassCaptureCanvas.h"
#include "deClassCanvasView.h"
#include "../graphics/deClassImage.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"

#include <libdscript/exceptions.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvas.h>
#include <dragengine/resources/canvas/capture/deCaptureCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Native Structure
struct sCapCanNatDat{
	deCaptureCanvas *captureCanvas;
};



// Native Functions
/////////////////////

// public func new()
deClassCaptureCanvas::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsCapCan,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCaptureCanvas::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sCapCanNatDat &nd = *( ( sCapCanNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassCaptureCanvas* )GetOwnerClass() )->GetDS();
	deCaptureCanvasManager &manager = *ds.GetGameEngine()->GetCaptureCanvasManager();
	
	// clear ( important )
	nd.captureCanvas = NULL;
	
	// create capture canvas
	nd.captureCanvas = manager.CreateCaptureCanvas();
}

// public func destructor()
deClassCaptureCanvas::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsCapCan,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCaptureCanvas::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sCapCanNatDat &nd = *( ( sCapCanNatDat* )p_GetNativeData( myself ) );
	
	if( nd.captureCanvas ){
		nd.captureCanvas->FreeReference();
		nd.captureCanvas = NULL;
	}
}



// Management
///////////////

// public func CanvasView getCanvasView()
deClassCaptureCanvas::nfGetCanvasView::nfGetCanvasView( const sInitData &init ) : dsFunction( init.clsCapCan,
"getCanvasView", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCView ){
}
void deClassCaptureCanvas::nfGetCanvasView::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	const deScriptingDragonScript &ds = ( ( deClassCaptureCanvas* )GetOwnerClass() )->GetDS();
	
	ds.GetClassCanvasView()->PushCanvas( rt, captureCanvas.GetCanvasView() );
}

// public func void setCanvasView( CanvasView canvasView )
deClassCaptureCanvas::nfSetCanvasView::nfSetCanvasView( const sInitData &init ) : dsFunction( init.clsCapCan,
"setCanvasView", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsCView ); // canvasView
}
void deClassCaptureCanvas::nfSetCanvasView::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	const deScriptingDragonScript &ds = ( ( deClassCaptureCanvas* )GetOwnerClass() )->GetDS();
	
	deCanvasView * const canvasView = ds.GetClassCanvasView()->GetCanvas( rt->GetValue( 0 )->GetRealObject() );
	
	captureCanvas.SetCanvasView( canvasView );
}

// public func Image getImage()
deClassCaptureCanvas::nfGetImage::nfGetImage( const sInitData &init ) : dsFunction( init.clsCapCan,
"getImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassCaptureCanvas::nfGetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	const deScriptingDragonScript &ds = ( ( deClassCaptureCanvas* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, captureCanvas.GetImage() );
}

// public func void setImage( int width, int height, int componentCount, int bitCount )
deClassCaptureCanvas::nfSetImage::nfSetImage( const sInitData &init ) : dsFunction( init.clsCapCan,
"setImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsInt ); // width
	p_AddParameter( init.clsInt ); // height
	p_AddParameter( init.clsInt ); // componentCount
	p_AddParameter( init.clsInt ); // bitCount
}
void deClassCaptureCanvas::nfSetImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	const deScriptingDragonScript &ds = ( ( deClassCaptureCanvas* )GetOwnerClass() )->GetDS();
	deImageManager &imageManager = *ds.GetGameEngine()->GetImageManager();
	
	const int width = rt->GetValue( 0 )->GetInt();
	const int height = rt->GetValue( 1 )->GetInt();
	const int componentCount = rt->GetValue( 2 )->GetInt();
	const int bitCount = rt->GetValue( 3 )->GetInt();
	
	deImage *image = NULL;
	
	try{
		image = imageManager.CreateImage( width, height, 1, componentCount, bitCount );
		captureCanvas.SetImage( image );
		image->FreeReference();
		
	}catch( ... ){
		if( image ){
			image->FreeReference();
		}
		throw;
	}
}

// public func void dropImage()
deClassCaptureCanvas::nfDropImage::nfDropImage( const sInitData &init ) : dsFunction( init.clsCapCan,
"dropImage", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassCaptureCanvas::nfDropImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	
	captureCanvas.SetImage( NULL );
}

// public func bool getCapture()
deClassCaptureCanvas::nfGetCapture::nfGetCapture( const sInitData &init ) : dsFunction( init.clsCapCan,
"getCapture", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassCaptureCanvas::nfGetCapture::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	
	rt->PushBool( captureCanvas.GetCapture() );
}

// public func void setCapture( bool capture )
deClassCaptureCanvas::nfSetCapture::nfSetCapture( const sInitData &init ) : dsFunction( init.clsCapCan,
"setCapture", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // capture
}
void deClassCaptureCanvas::nfSetCapture::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas &captureCanvas = *( ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas );
	
	captureCanvas.SetCapture( rt->GetValue( 0 )->GetBool() );
}



// public func int hashCode()
deClassCaptureCanvas::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsCapCan, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassCaptureCanvas::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas * const captureCanvas = ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas;
	// hash code = memory location
	rt->PushInt( ( int )( intptr_t )captureCanvas );
}

// public func bool equals( Object obj )
deClassCaptureCanvas::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsCapCan, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // obj
}
void deClassCaptureCanvas::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deCaptureCanvas * const captureCanvas = ( ( sCapCanNatDat* )p_GetNativeData( myself ) )->captureCanvas;
	deClassCaptureCanvas * const clsCapCan = ( deClassCaptureCanvas* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsCapCan ) ){
		rt->PushBool( false );
		
	}else{
		deCaptureCanvas * const otherCaptureCanvas = ( ( sCapCanNatDat* )p_GetNativeData( obj ) )->captureCanvas;
		rt->PushBool( captureCanvas == otherCaptureCanvas );
	}
}



// Class deClassCaptureCanvas
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassCaptureCanvas::deClassCaptureCanvas( deScriptingDragonScript &ds ) :
dsClass( "CaptureCanvas", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sCapCanNatDat ) );
}

deClassCaptureCanvas::~deClassCaptureCanvas(){
}



// Management
///////////////

void deClassCaptureCanvas::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsCapCan = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsObj = engine->GetClassObject();
	init.clsBool = engine->GetClassBool();
	init.clsCView = pDS.GetClassCanvasView();
	init.clsImage = pDS.GetClassImage();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetCanvasView( init ) );
	AddFunction( new nfSetCanvasView( init ) );
	AddFunction( new nfGetImage( init ) );
	AddFunction( new nfSetImage( init ) );
	AddFunction( new nfDropImage( init ) );
	AddFunction( new nfGetCapture( init ) );
	AddFunction( new nfSetCapture( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}



deCaptureCanvas *deClassCaptureCanvas::GetCaptureCanvas( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sCapCanNatDat* )p_GetNativeData( myself->GetBuffer() ) )->captureCanvas;
}

void deClassCaptureCanvas::PushCaptureCanvas( dsRunTime *rt, deCaptureCanvas *captureCanvas ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! captureCanvas ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sCapCanNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->captureCanvas = captureCanvas;
	captureCanvas->AddReference();
}
