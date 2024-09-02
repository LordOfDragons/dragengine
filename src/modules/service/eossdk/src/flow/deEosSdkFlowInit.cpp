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

#include "deEosSdkFlowInit.h"
#include "../deEosSdkServiceEos.h"
#include "../deEosSdk.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/service/deServiceManager.h>


// Class deEosSdkFlowInit
///////////////////////////

static void fCreateDeviceIdCallback( const EOS_Connect_CreateDeviceIdCallbackInfo *data ){
	( ( deEosSdkFlowInit* )data->ClientData )->OnCreateDeviceIdCallback( *data );
}

static void fLoginCallback( const EOS_Connect_LoginCallbackInfo *data ){
	( ( deEosSdkFlowInit* )data->ClientData )->OnLoginCallback( *data );
}

// Constructor, destructor
////////////////////////////

deEosSdkFlowInit::deEosSdkFlowInit( deEosSdkServiceEos &service ) :
deEosSdkFlow( service, decUniqueID() )
{
	GetModule().LogInfo( "deEosSdkFlowInit: Init service" );
	CreateDeviceId();
}



// Management
///////////////

void deEosSdkFlowInit::CreateDeviceId(){
	EOS_Connect_CreateDeviceIdOptions options = {};
	options.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
	
	#ifdef OS_W32
	options.DeviceModel = "Windows";
	#elif defined OS_MACOS
	options.DeviceModel = "MacOS";
	#elif defined OS_BEOS
	options.DeviceModel = "BeOS";
	#elif defined OS_ANDROID
	options.DeviceModel = "Android";
	#elif defined OS_UNIX
	options.DeviceModel = "Unix";
	#else
	options.DeviceModel = "Unknown";
	#endif
	
	GetModule().LogInfo( "deEosSdkFlowInit: Create device id" );
	EOS_Connect_CreateDeviceId( pService.GetHandleConnect(), &options, this, fCreateDeviceIdCallback );
}

void deEosSdkFlowInit::ConnectLogin(){
	EOS_Connect_Credentials credentials = {};
	credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	credentials.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
	
	EOS_Connect_UserLoginInfo userLoginInfo = {};
	userLoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
	userLoginInfo.DisplayName = "Player";
	
	EOS_Connect_LoginOptions options = {};
	options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	options.Credentials = &credentials;
	options.UserLoginInfo = &userLoginInfo;
	
	GetModule().LogInfo( "deEosSdkFlowInit: Connect login" );
	EOS_Connect_Login( pService.GetHandleConnect(), &options, this, fLoginCallback );
}

void deEosSdkFlowInit::FinishFlowEvent( EOS_EResult res ){
	const deServiceObject::Ref event( deServiceObject::Ref::New( new deServiceObject ) );
	event->SetStringChildAt( "event", "initialized" );
	
	if( res == EOS_EResult::EOS_Success ){
		event->SetBoolChildAt( "success", true );
		
	}else{
		event->SetBoolChildAt( "success", false );
		event->SetStringChildAt( "error", EOS_EResult_ToString( res ) );
		event->SetStringChildAt( "message", EOS_EResult_ToString( res ) );
	}
	
	GetModule().GetGameEngine()->GetServiceManager()->QueueEventReceived( pService.GetService(), event );
	delete this;
}



void deEosSdkFlowInit::OnCreateDeviceIdCallback( const EOS_Connect_CreateDeviceIdCallbackInfo &data ){
	GetModule().LogInfoFormat( "deEosSdkFlowInit.OnInitCreateDeviceIdCallback: res=%d", ( int )data.ResultCode );
	switch( data.ResultCode ){
	case EOS_EResult::EOS_DuplicateNotAllowed:
		GetModule().LogInfo( "deEosSdkFlowInit: Device id already created." );
		ConnectLogin();
		break;
		
	case EOS_EResult::EOS_Success:
		GetModule().LogInfo( "deEosSdkFlowInit: Device id created." );
		ConnectLogin();
		break;
		
	default:
		FinishFlowEvent( data.ResultCode );
	}
}

void deEosSdkFlowInit::OnLoginCallback( const EOS_Connect_LoginCallbackInfo &data ){
	GetModule().LogInfoFormat( "deEosSdkFlowInit.OnInitLoginCallback: res=%d", ( int )data.ResultCode );
	if( data.ResultCode == EOS_EResult::EOS_Success ){
		GetModule().LogInfo( "deEosSdkFlowInit: Connected." );
		pService.productUserId = data.LocalUserId;
		FinishFlowEvent( EOS_EResult::EOS_Success );
		
	}else{
		GetModule().LogWarn( "deEosSdkFlowInit: Connect failed. Using device id account is not possible." );
		//FinishFlowEvent( data.ResultCode );
		FinishFlowEvent( EOS_EResult::EOS_Success );
	}
}