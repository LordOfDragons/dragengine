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

#include "deClassVideo.h"
#include "deClassVideoPlayer.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <libdscript/exceptions.h>



// native structure
struct sVPNatDat{
	deVideoPlayer *videoPlayer;
};



// Native functions
/////////////////////

// public func new()
deClassVideoPlayer::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsVP,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVideoPlayer::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sVPNatDat &nd = *( ( sVPNatDat* )p_GetNativeData( myself ) );
	deClassVideoPlayer &clsVP = *( ( deClassVideoPlayer* )GetOwnerClass() );
	deVideoPlayerManager &vpmgr = *clsVP.GetDS()->GetGameEngine()->GetVideoPlayerManager();
	
	// clear (important)
	nd.videoPlayer = NULL;
	
	// create video player
	nd.videoPlayer = vpmgr.CreateVideoPlayer();
	if( ! nd.videoPlayer ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassVideoPlayer::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsVP,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVideoPlayer::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sVPNatDat &nd = *( ( sVPNatDat* )p_GetNativeData( myself ) );
	
	if( nd.videoPlayer ){
		nd.videoPlayer->FreeReference();
		nd.videoPlayer = NULL;
	}
}



// public func Video getVideo()
deClassVideoPlayer::nfGetVideo::nfGetVideo( const sInitData &init ) : dsFunction( init.clsVP,
"getVideo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVid ){
}
void deClassVideoPlayer::nfGetVideo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	deClassVideoPlayer &clsVP = *( ( deClassVideoPlayer* )GetOwnerClass() );
	
	clsVP.GetDS()->GetClassVideo()->PushVideo( rt, videoPlayer.GetVideo() );
}

// public func void setVideo( Video video )
deClassVideoPlayer::nfSetVideo::nfSetVideo( const sInitData &init ) : dsFunction( init.clsVP,
"setVideo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVid ); // video
}
void deClassVideoPlayer::nfSetVideo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	deClassVideoPlayer &clsVP = *( ( deClassVideoPlayer* )GetOwnerClass() );
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	
	if( object ){
		videoPlayer.SetVideo( clsVP.GetDS()->GetClassVideo()->GetVideo( object ) );
		
	}else{
		videoPlayer.SetVideo( NULL );
	}
}



// public func float getLooping()
deClassVideoPlayer::nfGetLooping::nfGetLooping( const sInitData &init ) : dsFunction( init.clsVP,
"getLooping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVideoPlayer::nfGetLooping::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushBool( videoPlayer.GetLooping() );
}

// public func void setLooping( bool looping )
deClassVideoPlayer::nfSetLooping::nfSetLooping( const sInitData &init ) : dsFunction( init.clsVP,
"setLooping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // looping
}
void deClassVideoPlayer::nfSetLooping::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.SetLooping( rt->GetValue( 0 )->GetBool() );
}

// public func float getPlayFrom()
deClassVideoPlayer::nfGetPlayFrom::nfGetPlayFrom( const sInitData &init ) : dsFunction( init.clsVP,
"getPlayFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVideoPlayer::nfGetPlayFrom::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushFloat( videoPlayer.GetPlayFrom() );
}

// public func float getPlayTo()
deClassVideoPlayer::nfGetPlayTo::nfGetPlayTo( const sInitData &init ) : dsFunction( init.clsVP,
"getPlayTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVideoPlayer::nfGetPlayTo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushFloat( videoPlayer.GetPlayTo() );
}

// public func void setPlayRange( float playFrom, float playTo )
deClassVideoPlayer::nfSetPlayRange::nfSetPlayRange( const sInitData &init ) : dsFunction( init.clsVP,
"setPlayRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // playFrom
	p_AddParameter( init.clsFlt ); // playTo
}
void deClassVideoPlayer::nfSetPlayRange::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.SetPlayRange( rt->GetValue( 0 )->GetFloat(), rt->GetValue( 1 )->GetFloat() );
}

// public func float getPlayPosition()
deClassVideoPlayer::nfGetPlayPosition::nfGetPlayPosition( const sInitData &init ) : dsFunction( init.clsVP,
"getPlayPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVideoPlayer::nfGetPlayPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushFloat( videoPlayer.GetPlayPosition() );
}

// public func void setPlayPosition( float position )
deClassVideoPlayer::nfSetPlayPosition::nfSetPlayPosition( const sInitData &init ) : dsFunction( init.clsVP,
"setPlayPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // position
}
void deClassVideoPlayer::nfSetPlayPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.SetPlayPosition( rt->GetValue( 0 )->GetFloat() );
}

// public func float getPlaySpeed()
deClassVideoPlayer::nfGetPlaySpeed::nfGetPlaySpeed( const sInitData &init ) : dsFunction( init.clsVP,
"getPlaySpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassVideoPlayer::nfGetPlaySpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushFloat( videoPlayer.GetPlaySpeed() );
}

// public func void setPlaySpeed( float playSpeed )
deClassVideoPlayer::nfSetPlaySpeed::nfSetPlaySpeed( const sInitData &init ) : dsFunction( init.clsVP,
"setPlaySpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // playSpeed
}
void deClassVideoPlayer::nfSetPlaySpeed::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.SetPlaySpeed( rt->GetValue( 0 )->GetFloat() );
}



// public func bool isPlaying()
deClassVideoPlayer::nfIsPlaying::nfIsPlaying( const sInitData &init ) : dsFunction( init.clsVP,
"isPlaying", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVideoPlayer::nfIsPlaying::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushBool( videoPlayer.GetPlaying() );
}

// public func bool isPaused()
deClassVideoPlayer::nfIsPaused::nfIsPaused( const sInitData &init ) : dsFunction( init.clsVP,
"isPaused", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVideoPlayer::nfIsPaused::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushBool( videoPlayer.GetPaused() );
}

// public func bool isStopped()
deClassVideoPlayer::nfIsStopped::nfIsStopped( const sInitData &init ) : dsFunction( init.clsVP,
"isStopped", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassVideoPlayer::nfIsStopped::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	rt->PushBool( videoPlayer.GetStopped() );
}

// public func void play()
deClassVideoPlayer::nfPlay::nfPlay( const sInitData &init ) : dsFunction( init.clsVP,
"play", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVideoPlayer::nfPlay::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.Play();
}

// public func void stop()
deClassVideoPlayer::nfStop::nfStop( const sInitData &init ) : dsFunction( init.clsVP,
"stop", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVideoPlayer::nfStop::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.Stop();
}

// public func void pause()
deClassVideoPlayer::nfPause::nfPause( const sInitData &init ) : dsFunction( init.clsVP,
"pause", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassVideoPlayer::nfPause::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.Pause();
}



// public func void update( float elapsed )
deClassVideoPlayer::nfUpdate::nfUpdate( const sInitData &init ) : dsFunction( init.clsVP,
"update", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // elapsed
}
void deClassVideoPlayer::nfUpdate::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer &videoPlayer = *( ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer );
	
	videoPlayer.Update( rt->GetValue( 0 )->GetFloat() );
}



// public func int hashCode()
deClassVideoPlayer::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsVP, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassVideoPlayer::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer *videoPlayer = ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer;
	
	rt->PushInt( ( int )( intptr_t )videoPlayer );
}

// public func bool equals( Object object )
deClassVideoPlayer::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsVP, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassVideoPlayer::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deVideoPlayer *videoPlayer = ( ( sVPNatDat* )p_GetNativeData( myself ) )->videoPlayer;
	deClassVideoPlayer *clsVP = ( deClassVideoPlayer* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsVP ) ){
		rt->PushBool( false );
		
	}else{
		deVideoPlayer *otherVideoPlayer = ( ( sVPNatDat* )p_GetNativeData( obj ) )->videoPlayer;
		
		rt->PushBool( videoPlayer == otherVideoPlayer );
	}
}



// Class deClassVideoPlayer
/////////////////////////////

// Constructor
////////////////

deClassVideoPlayer::deClassVideoPlayer( deScriptingDragonScript *ds ) :
dsClass( "VideoPlayer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	// prepare
	pDS = ds;
	
	// store information into parser info
	GetParserInfo()->SetParent( DENS_GUI );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sVPNatDat ) );
}

deClassVideoPlayer::~deClassVideoPlayer(){
}



// Management
///////////////

void deClassVideoPlayer::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsVP = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVid = pDS->GetClassVideo();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetVideo( init ) );
	AddFunction( new nfSetVideo( init ) );
	
	AddFunction( new nfGetLooping( init ) );
	AddFunction( new nfSetLooping( init ) );
	AddFunction( new nfGetPlayFrom( init ) );
	AddFunction( new nfGetPlayTo( init ) );
	AddFunction( new nfSetPlayRange( init ) );
	AddFunction( new nfGetPlayPosition( init ) );
	AddFunction( new nfSetPlayPosition( init ) );
	AddFunction( new nfGetPlaySpeed( init ) );
	AddFunction( new nfSetPlaySpeed( init ) );
	
	AddFunction( new nfIsPlaying( init ) );
	AddFunction( new nfIsPaused( init ) );
	AddFunction( new nfIsStopped( init ) );
	AddFunction( new nfPlay( init ) );
	AddFunction( new nfPause( init ) );
	AddFunction( new nfStop( init ) );
	
	AddFunction( new nfUpdate( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deVideoPlayer *deClassVideoPlayer::GetVideoPlayer( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sVPNatDat* )p_GetNativeData( myself->GetBuffer() ) )->videoPlayer;
}

void deClassVideoPlayer::PushVideoPlayer( dsRunTime *rt, deVideoPlayer *videoPlayer ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! videoPlayer ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sVPNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->videoPlayer = videoPlayer;
	videoPlayer->AddReference();
}
