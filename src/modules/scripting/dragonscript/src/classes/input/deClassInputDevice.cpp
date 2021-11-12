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

#include "deClassInputDevice.h"
#include "deClassInputDeviceAxis.h"
#include "deClassInputDeviceButton.h"
#include "deClassInputDeviceFeedback.h"
#include "deClassInputDeviceComponent.h"
#include "../graphics/deClassImage.h"
#include "../math/deClassMatrix.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassVector.h"
#include "../world/deClassModel.h"
#include "../world/deClassSkin.h"
#include "../world/deClassRig.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceAxis.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/input/deInputDeviceFeedback.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sInputDeviceNatDat{
	dedsInputDevice *device;
};



// Native functions
/////////////////////


// Constructors, Destructor
/////////////////////////////

// public func destructor()
deClassInputDevice::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsInputDevice, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassInputDevice::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) );
	if( ! nd.device ){
		return;
	}
	
	nd.device->FreeReference();
	nd.device = NULL;
}



// Management
///////////////

// public func InputEventSource getSource()
deClassInputDevice::nfGetSource::nfGetSource( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getSource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputEventSource ){
}
void deClassInputDevice::nfGetSource::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushValue( ( ( deClassInputDevice* )GetOwnerClass() )->GetClassInputEventSource()
		->GetVariable( device.GetDeviceSource() )->GetStaticValue() );
}

// public func String getIndex()
deClassInputDevice::nfGetIndex::nfGetIndex( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushInt( ( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device->GetDeviceIndex() );
}

// public func String getID()
deClassInputDevice::nfGetID::nfGetID( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDevice::nfGetID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushString( device.GetDevice()->GetID() );
}

// public func String getName()
deClassInputDevice::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDevice::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushString( device.GetDevice()->GetName() );
}

// public func InputDeviceType getType()
deClassInputDevice::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceType ){
}
void deClassInputDevice::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushValue( ( ( deClassInputDevice* )GetOwnerClass() )->GetClassInputDeviceType()
		->GetVariable( device.GetDevice()->GetType() )->GetStaticValue() );
}

// public func Model getDisplayModel()
deClassInputDevice::nfGetDisplayModel::nfGetDisplayModel( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplayModel", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModel ){
}
void deClassInputDevice::nfGetDisplayModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassModel()->PushModel( rt, device.GetDevice()->GetDisplayModel() );
}

// public func Skin getDisplaySkin()
deClassInputDevice::nfGetDisplaySkin::nfGetDisplaySkin( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplaySkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
}
void deClassInputDevice::nfGetDisplaySkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSkin()->PushSkin( rt, device.GetDevice()->GetDisplaySkin() );
}

// public func Image getDisplayImage()
deClassInputDevice::nfGetDisplayImage::nfGetDisplayImage( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplayImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassInputDevice::nfGetDisplayImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, device.GetDevice()->GetDisplayImage() );
}

// public func int getDisplayIconCount()
deClassInputDevice::nfGetDisplayIconCount::nfGetDisplayIconCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplayIconCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetDisplayIconCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushInt( device.GetDevice()->GetDisplayIconCount() );
}

// public func Image getDisplayIconAt( int index )
deClassInputDevice::nfGetDisplayIconAt::nfGetDisplayIconAt( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetDisplayIconAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, device.GetDevice()->GetDisplayIconAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func Image getLargestDisplayIconX( int maxWidth )
deClassInputDevice::nfGetLargestDisplayIconX::nfGetLargestDisplayIconX( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getLargestDisplayIconX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxWidth
}
void deClassInputDevice::nfGetLargestDisplayIconX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int count = device.GetDevice()->GetDisplayIconCount();
	const int maxWidth = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestWidth = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = device.GetDevice()->GetDisplayIconAt( i );
		if( icon->GetWidth() <= maxWidth && ( ! bestIcon || icon->GetWidth() >= bestWidth ) ){
			bestIcon = icon;
			bestWidth = icon->GetWidth();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func Image getLargestDisplayIconY( int maxHeight )
deClassInputDevice::nfGetLargestDisplayIconY::nfGetLargestDisplayIconY( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getLargestDisplayIconY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxHeight
}
void deClassInputDevice::nfGetLargestDisplayIconY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int count = device.GetDevice()->GetDisplayIconCount();
	const int maxHeight = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestHeight = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = device.GetDevice()->GetDisplayIconAt( i );
		if( icon->GetHeight() <= maxHeight && ( ! bestIcon || icon->GetHeight() >= bestHeight ) ){
			bestIcon = icon;
			bestHeight = icon->GetHeight();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func String getDisplayText()
deClassInputDevice::nfGetDisplayText::nfGetDisplayText( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDisplayText", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDevice::nfGetDisplayText::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	
	rt->PushString( device.GetDevice()->GetDisplayText() );
}



// public func int getAxisCount()
deClassInputDevice::nfGetAxisCount::nfGetAxisCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getAxisCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetAxisCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushInt( device.GetDevice()->GetAxisCount() );
}

// public func InputDeviceAxis getAxisAt( int index )
deClassInputDevice::nfGetAxisAt::nfGetAxisAt( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getAxisAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsIDAxis ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetAxisAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	const int axisIndex = rt->GetValue( 0 )->GetInt();
	ds.GetClassInputDeviceAxis()->PushAxis( rt, nd.device, axisIndex );
}



// public func int getButtonCount()
deClassInputDevice::nfGetButtonCount::nfGetButtonCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getButtonCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetButtonCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushInt( device.GetDevice()->GetButtonCount() );
}

// public func InputDeviceButton getButtonAt( int index )
deClassInputDevice::nfGetButtonAt::nfGetButtonAt( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getButtonAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsIDButton ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetButtonAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	const int buttonIndex = rt->GetValue( 0 )->GetInt();
	ds.GetClassInputDeviceButton()->PushButton( rt, nd.device, buttonIndex );
}



// public func int getFeedbackCount()
deClassInputDevice::nfGetFeedbackCount::nfGetFeedbackCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getFeedbackCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetFeedbackCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	
	rt->PushInt( device.GetDevice()->GetFeedbackCount() );
}

// public func InputDeviceFeedback getFeedbackAt( int index )
deClassInputDevice::nfGetFeedbackAt::nfGetFeedbackAt( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getFeedbackAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsIDFeedback ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetFeedbackAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	const int feedbackIndex = rt->GetValue( 0 )->GetInt();
	ds.GetClassInputDeviceFeedback()->PushFeedback( rt, nd.device, feedbackIndex );
}



// public func int getComponentCount()
deClassInputDevice::nfGetComponentCount::nfGetComponentCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getComponentCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetComponentCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	
	rt->PushInt( device.GetDevice()->GetComponentCount() );
}

// public func InputDeviceComponent getComponentAt( int index )
deClassInputDevice::nfGetComponentAt::nfGetComponentAt( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getComponentAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsIDComponent ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetComponentAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	const int componentIndex = rt->GetValue( 0 )->GetInt();
	ds.GetClassInputDeviceComponent()->PushComponent( rt, nd.device, componentIndex );
}



// public static func int buttonMatchingKeyCode( int keyCode )
deClassInputDevice::nfButtonMatchingKeyCode::nfButtonMatchingKeyCode( const sInitData &init ) :
dsFunction( init.clsInputDevice, "buttonMatchingKeyCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // keyCode
}
void deClassInputDevice::nfButtonMatchingKeyCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const deInputEvent::eKeyCodes keyCode =
		( deInputEvent::eKeyCodes )rt->GetValue( 0 )->GetInt();
	
	rt->PushInt( module.ButtonMatchingKeyCode(
		module.IndexOfDeviceWithID( device.GetDevice()->GetID() ), keyCode ) );
}

// public static func int buttonMatchingKeyChar( int keyCode )
deClassInputDevice::nfButtonMatchingKeyChar::nfButtonMatchingKeyChar( const sInitData &init ) :
dsFunction( init.clsInputDevice, "buttonMatchingKeyChar", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // keyChar
}
void deClassInputDevice::nfButtonMatchingKeyChar::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	int keyChar = rt->GetValue( 0 )->GetInt();
	
	rt->PushInt( module.ButtonMatchingKeyChar(
		module.IndexOfDeviceWithID( device.GetDevice()->GetID() ), keyChar ) );
}

// public static func int buttonMatchingKeyCode( int keyCode, InputEventKeyLocation location )
deClassInputDevice::nfButtonMatchingKeyCode2::nfButtonMatchingKeyCode2( const sInitData &init ) :
dsFunction( init.clsInputDevice, "buttonMatchingKeyCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // keyCode
	p_AddParameter( init.clsInputEventKeyLocation ); // location
}
void deClassInputDevice::nfButtonMatchingKeyCode2::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const deInputEvent::eKeyCodes keyCode = ( deInputEvent::eKeyCodes )rt->GetValue( 0 )->GetInt();
	
	const deInputEvent::eKeyLocation location = ( deInputEvent::eKeyLocation )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushInt( module.ButtonMatchingKeyCode(
		module.IndexOfDeviceWithID( device.GetDevice()->GetID() ), keyCode, location ) );
}

// public static func int buttonMatchingKeyChar( int keyCode, InputEventKeyLocation location )
deClassInputDevice::nfButtonMatchingKeyChar2::nfButtonMatchingKeyChar2( const sInitData &init ) :
dsFunction( init.clsInputDevice, "buttonMatchingKeyChar", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // keyChar
	p_AddParameter( init.clsInputEventKeyLocation ); // location
}
void deClassInputDevice::nfButtonMatchingKeyChar2::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int keyChar = rt->GetValue( 0 )->GetInt();
	
	const deInputEvent::eKeyLocation location = ( deInputEvent::eKeyLocation )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 1 )->GetRealObject() );
	
	rt->PushInt( module.ButtonMatchingKeyChar(
		module.IndexOfDeviceWithID( device.GetDevice()->GetID() ), keyChar, location ) );
}



// public func InputDeviceBoneConfiguration getBoneConfiguration()
deClassInputDevice::nfGetBoneConfiguration::nfGetBoneConfiguration( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getBoneConfiguration", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceBoneConfiguration ){
}
void deClassInputDevice::nfGetBoneConfiguration::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	rt->PushValue( ( ( deClassInputDevice* )GetOwnerClass() )->GetClassInputDeviceBoneConfiguration()
		->GetVariable( device.GetDevice()->GetBoneConfiguration() )->GetStaticValue() );
}

// public func Vector getFingerTipOffset( int index )
deClassInputDevice::nfGetFingerTipOffset::nfGetFingerTipOffset( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getFingerTipOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDevice::nfGetFingerTipOffset::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int index = rt->GetValue( 0 )->GetInt();
	
	ds.GetClassVector()->PushVector( rt, device.GetDevice()->GetFingerTipOffset( index ) );
}

// public func Rig getHandRig()
deClassInputDevice::nfGetHandRig::nfGetHandRig( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getHandRig", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsRig ){
}
void deClassInputDevice::nfGetHandRig::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassRig()->PushRig( rt, device.GetDevice()->GetHandRig() );
}

// public func Model getVRModel()
deClassInputDevice::nfGetVRModel::nfGetVRModel( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getVRModel", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsModel ){
}
void deClassInputDevice::nfGetVRModel::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassModel()->PushModel( rt, device.GetDevice()->GetVRModel() );
}

// public func Skin getVRSkin()
deClassInputDevice::nfGetVRSkin::nfGetVRSkin( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getVRSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin ){
}
void deClassInputDevice::nfGetVRSkin::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSkin()->PushSkin( rt, device.GetDevice()->GetVRSkin() );
}



// public func bool isPresent()
deClassInputDevice::nfIsPresent::nfIsPresent( const sInitData &init ) :
dsFunction( init.clsInputDevice, "isPresent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassInputDevice::nfIsPresent::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	rt->PushBool( module.IndexOfDeviceWithID( device.GetDevice()->GetID() ) != -1 );
}



// public static func Vector getDevicePosePosition()
deClassInputDevice::nfGetDevicePosePosition::nfGetDevicePosePosition( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDevicePosePosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}
void deClassInputDevice::nfGetDevicePosePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, device.GetDevicePose().GetPosition() );
}

// public static func Quaternion getDevicePoseOrientation()
deClassInputDevice::nfGetDevicePoseOrientation::nfGetDevicePoseOrientation( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDevicePoseOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
}
void deClassInputDevice::nfGetDevicePoseOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, device.GetDevicePose().GetOrientation() );
}

// public static func Vector getDevicePoseLinearVelocity()
deClassInputDevice::nfGetDevicePoseLinearVelocity::nfGetDevicePoseLinearVelocity( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDevicePoseLinearVelocity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}
void deClassInputDevice::nfGetDevicePoseLinearVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, device.GetDevicePose().GetLinearVelocity() );
}

// public static func Vector getDevicePoseAngularVelocity()
deClassInputDevice::nfGetDevicePoseAngularVelocity::nfGetDevicePoseAngularVelocity( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDevicePoseAngularVelocity", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
}
void deClassInputDevice::nfGetDevicePoseAngularVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassVector()->PushVector( rt, device.GetDevicePose().GetAngularVelocity() * RAD2DEG );
}

// public static func Matrix getDevicePoseMatrix()
deClassInputDevice::nfGetDevicePoseMatrix::nfGetDevicePoseMatrix( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDevicePoseMatrix", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix ){
}
void deClassInputDevice::nfGetDevicePoseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	
	ds.GetClassMatrix()->PushMatrix( rt, decMatrix::CreateWorld(
		device.GetDevicePose().GetPosition(), device.GetDevicePose().GetOrientation() ) );
}



// public static func int getDeviceBonePoseCount()
deClassInputDevice::nfGetDeviceBonePoseCount::nfGetDeviceBonePoseCount( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDeviceBonePoseCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDevice::nfGetDeviceBonePoseCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	
	rt->PushInt( device.GetBonePoseCount() );
}

// public static func Vector getDeviceBonePosePosition( int bone, bool withController )
deClassInputDevice::nfGetDeviceBonePosePosition::nfGetDeviceBonePosePosition( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDeviceBonePosePosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVector ){
	p_AddParameter( init.clsInteger ); // bone
	p_AddParameter( init.clsBool ); // withController
}
void deClassInputDevice::nfGetDeviceBonePosePosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	const bool withController = rt->GetValue( 1 )->GetBool();
	
	ds.GetClassVector()->PushVector( rt, device.GetBonePoseAt( bone, withController ).GetPosition() );
}

// public static func Quaternion getDeviceBonePoseOrientation( int bone, bool withController )
deClassInputDevice::nfGetDeviceBonePoseOrientation::nfGetDeviceBonePoseOrientation( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDeviceBonePoseOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
	p_AddParameter( init.clsInteger ); // bone
	p_AddParameter( init.clsBool ); // withController
}
void deClassInputDevice::nfGetDeviceBonePoseOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	const bool withController = rt->GetValue( 1 )->GetBool();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, device.GetBonePoseAt( bone, withController ).GetOrientation() );
}

// public static func Matrix getDeviceBonePoseMatrix( int bone, bool withController )
deClassInputDevice::nfGetDeviceBonePoseMatrix::nfGetDeviceBonePoseMatrix( const sInitData &init ) :
dsFunction( init.clsInputDevice, "getDeviceBonePoseMatrix", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMatrix ){
	p_AddParameter( init.clsInteger ); // bone
	p_AddParameter( init.clsBool ); // withController
}
void deClassInputDevice::nfGetDeviceBonePoseMatrix::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice &device = *( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	const deScriptingDragonScript &ds = ( ( deClassInputDevice* )GetOwnerClass() )->GetDS();
	const int bone = rt->GetValue( 0 )->GetInt();
	const bool withController = rt->GetValue( 1 )->GetBool();
	const deInputDevicePose &pose = device.GetBonePoseAt( bone, withController );
	
	ds.GetClassMatrix()->PushMatrix( rt, decMatrix::CreateWorld( pose.GetPosition(), pose.GetOrientation() ) );
}



// public func int hashCode()
deClassInputDevice::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsInputDevice, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassInputDevice::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dedsInputDevice * const device = ( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	
	rt->PushInt( ( intptr_t )device );
}

// public func bool equals( Object obj )
deClassInputDevice::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsInputDevice, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassInputDevice::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	dedsInputDevice * const device = ( ( sInputDeviceNatDat* )p_GetNativeData( myself ) )->device;
	deClassInputDevice * const clsInputDevice = ( deClassInputDevice* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsInputDevice ) ){
		rt->PushBool( false );
		
	}else{
		dedsInputDevice * const other = ( ( sInputDeviceNatDat* )p_GetNativeData( obj ) )->device;
		rt->PushBool( device == other );
	}
}



// Class deClassInputDevice
/////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputDevice::deClassInputDevice( deScriptingDragonScript &ds ) :
dsClass( "InputDevice", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sInputDeviceNatDat ) );
}

deClassInputDevice::~deClassInputDevice(){
}



// Management
///////////////

void deClassInputDevice::CreateClassMembers( dsEngine *engine ){
	pClsInputDeviceType = engine->GetClass( "Dragengine.InputDeviceType" );
	pClsInputDeviceBoneConfiguration = engine->GetClass( "Dragengine.InputDeviceBoneConfiguration" );
	pClsInputEventSource = engine->GetClass( "Dragengine.InputEventSource" );
	pClsInputEventKeyLocation = engine->GetClass( "Dragengine.InputEventKeyLocation" );
	
	sInitData init;
	init.clsInputDevice = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsImage = pDS.GetClassImage();
	init.clsModel = pDS.GetClassModel();
	init.clsSkin = pDS.GetClassSkin();
	init.clsIDAxis = pDS.GetClassInputDeviceAxis();
	init.clsIDButton = pDS.GetClassInputDeviceButton();
	init.clsIDFeedback = pDS.GetClassInputDeviceFeedback();
	init.clsIDComponent = pDS.GetClassInputDeviceComponent();
	init.clsInputDeviceType = pClsInputDeviceType;
	init.clsInputDeviceBoneConfiguration = pClsInputDeviceBoneConfiguration;
	init.clsInputEventSource = pClsInputEventSource;
	init.clsInputEventKeyLocation = pClsInputEventKeyLocation;
	init.clsVector = pDS.GetClassVector();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsMatrix = pDS.GetClassMatrix();
	init.clsRig = pDS.GetClassRig();
	
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSource( init ) );
	AddFunction( new nfGetIndex( init ) );
	AddFunction( new nfGetID( init ) );
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfGetDisplayModel( init ) );
	AddFunction( new nfGetDisplaySkin( init ) );
	AddFunction( new nfGetDisplayImage( init ) );
	AddFunction( new nfGetDisplayIconCount( init ) );
	AddFunction( new nfGetDisplayIconAt( init ) );
	AddFunction( new nfGetLargestDisplayIconX( init ) );
	AddFunction( new nfGetLargestDisplayIconY( init ) );
	AddFunction( new nfGetDisplayText( init ) );
	
	AddFunction( new nfGetAxisCount( init ) );
	AddFunction( new nfGetAxisAt( init ) );
	
	AddFunction( new nfGetButtonCount( init ) );
	AddFunction( new nfGetButtonAt( init ) );
	
	AddFunction( new nfGetFeedbackCount( init ) );
	AddFunction( new nfGetFeedbackAt( init ) );
	
	AddFunction( new nfGetComponentCount( init ) );
	AddFunction( new nfGetComponentAt( init ) );
	
	AddFunction( new nfButtonMatchingKeyCode( init ) );;
	AddFunction( new nfButtonMatchingKeyChar( init ) );;
	AddFunction( new nfButtonMatchingKeyCode2( init ) );;
	AddFunction( new nfButtonMatchingKeyChar2( init ) );;
	
	AddFunction( new nfGetBoneConfiguration( init ) );
	AddFunction( new nfGetFingerTipOffset( init ) );
	AddFunction( new nfGetHandRig( init ) );
	AddFunction( new nfGetVRModel( init ) );
	AddFunction( new nfGetVRSkin( init ) );
	
	AddFunction( new nfIsPresent( init ) );
	
	AddFunction( new nfGetDevicePosePosition( init ) );
	AddFunction( new nfGetDevicePoseOrientation( init ) );
	AddFunction( new nfGetDevicePoseLinearVelocity( init ) );
	AddFunction( new nfGetDevicePoseAngularVelocity( init ) );
	AddFunction( new nfGetDevicePoseMatrix( init ) );
	
	AddFunction( new nfGetDeviceBonePoseCount( init ) );
	AddFunction( new nfGetDeviceBonePosePosition( init ) );
	AddFunction( new nfGetDeviceBonePoseOrientation( init ) );
	AddFunction( new nfGetDeviceBonePoseMatrix( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

dedsInputDevice *deClassInputDevice::GetInputDevice( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sInputDeviceNatDat* )p_GetNativeData( myself->GetBuffer() ) )->device;
}

void deClassInputDevice::PushInputDevice( dsRunTime *rt, dedsInputDevice *device ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! device ){
		rt->PushObject( nullptr, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	sInputDeviceNatDat &nd = *( ( sInputDeviceNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.device = device;
	device->AddReference();
}
