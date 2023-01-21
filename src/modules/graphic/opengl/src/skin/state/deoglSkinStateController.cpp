/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSkinStateController.h"
#include "deoglSkinState.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../../video/deoglSharedVideoPlayer.h"
#include "../../video/deoglSharedVideoPlayerList.h"
#include "../../video/deoglVideoPlayer.h"
#include "../../world/deoglWorld.h"
#include "../../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <dragengine/deEngine.h>



// Class deoglSkinStateController
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateController::deoglSkinStateController() :
pSharedVideoPlayers( NULL ),
pSharedVideoPlayerCount( 0 ),
pHasCalculatedProperties( false ){
}

deoglSkinStateController::~deoglSkinStateController(){
	SetVideoPlayerCount( 0 );
}



// Management
///////////////

void deoglSkinStateController::SetVideoPlayerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pSharedVideoPlayerCount ){
		return;
	}
	
	// video players
	pVideoPlayers.RemoveAll();
	while( pVideoPlayers.GetCount() < count ){
		pVideoPlayers.Add( NULL );
	}
	
	// shared video players
	if( pSharedVideoPlayers ){
		while( pSharedVideoPlayerCount > 0 ){
			pSharedVideoPlayerCount--;
			if( pSharedVideoPlayers[ pSharedVideoPlayerCount ] ){
				pSharedVideoPlayers[ pSharedVideoPlayerCount ]->FreeUsage();
				pSharedVideoPlayers[ pSharedVideoPlayerCount ] = NULL;
			}
		}
		
		delete [] pSharedVideoPlayers;
		pSharedVideoPlayers = NULL;
	}
	
	if( count > 0 ){
		pSharedVideoPlayers = new deoglSharedVideoPlayer*[ count ];
		
		for( pSharedVideoPlayerCount=0; pSharedVideoPlayerCount<count; pSharedVideoPlayerCount++ ){
			pSharedVideoPlayers[ pSharedVideoPlayerCount ] = NULL;
		}
	}
}

deVideoPlayer *deoglSkinStateController::GetVideoPlayerAt( int index ) const{
	return ( deVideoPlayer* )pVideoPlayers.GetAt( index );
}

deoglSharedVideoPlayer *deoglSkinStateController::GetSharedVideoPlayerAt( int index ) const{
	if( index < 0 || index >= pSharedVideoPlayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pSharedVideoPlayers[ index ];
}

void deoglSkinStateController::SetVideoPlayerAt( int index, deVideoPlayer *videoPlayer ){
	pVideoPlayers.SetAt( index, videoPlayer );
}

void deoglSkinStateController::SetSharedVideoPlayerAt( int index, deoglSharedVideoPlayer *videoPlayer ){
	if( index < 0 || index >= pSharedVideoPlayerCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( videoPlayer == pSharedVideoPlayers[ index ] ){
		return;
	}
	
	if( pSharedVideoPlayers[ index ] ){
		pSharedVideoPlayers[ index ]->FreeUsage();
	}
	
	pSharedVideoPlayers[ index ] = videoPlayer;
}



void deoglSkinStateController::SetHasCalculatedProperties( bool hasCalculatedProperties ){
	pHasCalculatedProperties = hasCalculatedProperties;
}



bool deoglSkinStateController::RequiresSyncEveryFrameUpdate() const{
	return pVideoPlayers.GetCount() > 0 || pHasCalculatedProperties;
}

bool deoglSkinStateController::RequiresPrepareRenderables() const{
	return pVideoPlayers.GetCount() > 0;
}

void deoglSkinStateController::Init( deoglSkinState &skinState, deoglRSkin *skin, deoglWorld *world ){
	if( ! skin || ! world ){
		skinState.SetVideoPlayerCount( 0 );
		SetVideoPlayerCount( 0 );
		pHasCalculatedProperties = false;
		return;
	}
	
	const int textureCount = skin->GetTextureCount();
	if( textureCount == 0 ){
		skinState.SetVideoPlayerCount( 0 );
		SetVideoPlayerCount( 0 );
		pHasCalculatedProperties = false;
		return;
	}
	
	skinState.SetVideoPlayerCount( skin->GetVideoPlayerCount() );
	SetVideoPlayerCount( skin->GetVideoPlayerCount() );
	
	int i;
	for( i=0; i<textureCount; i++ ){
		pUpdateSkinVideoPlayers( skinState, skin->GetTextureAt( i ), *world );
	}
	
	pHasCalculatedProperties = skin->GetCalculatedPropertyCount() > 0;
}

void deoglSkinStateController::Init( deoglSkinState &skinState, deoglRSkin *skin,
int textureIndex, deoglWorld *world ){
	if( ! skin || ! world ){
		skinState.SetVideoPlayerCount( 0 );
		SetVideoPlayerCount( 0 );
		pHasCalculatedProperties = false;
		return;
	}
	
	const int textureCount = skin->GetTextureCount();
	if( textureCount == 0 ){
		skinState.SetVideoPlayerCount( 0 );
		SetVideoPlayerCount( 0 );
		pHasCalculatedProperties = false;
		return;
	}
	
	skinState.SetVideoPlayerCount( skin->GetVideoPlayerCount() );
	SetVideoPlayerCount( skin->GetVideoPlayerCount() );
	pUpdateSkinVideoPlayers( skinState, skin->GetTextureAt( textureIndex ), *world );
	
	pHasCalculatedProperties = skin->GetCalculatedPropertyCount() > 0;
}

void deoglSkinStateController::ResetTime(){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for( i=0; i< count; i++ ){
		deVideoPlayer * const videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetAt( i );
		if( videoPlayer ){
			videoPlayer->SetPlayPosition( 0.0f );
		}
	}
}

void deoglSkinStateController::AdvanceTime( float timeStep ){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for( i=0; i< count; i++ ){
		deVideoPlayer * const videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetAt( i );
		if( videoPlayer ){
			videoPlayer->Update( timeStep );
		}
	}
}

void deoglSkinStateController::SyncToRender(){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for( i=0; i< count; i++ ){
		deVideoPlayer * const videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetAt( i );
		if( videoPlayer ){
			( ( deoglVideoPlayer* )videoPlayer->GetPeerGraphic() )->SyncToRender();
		}
	}
}

void deoglSkinStateController::Clear(){
	SetVideoPlayerCount( 0 );
	pHasCalculatedProperties = false;
}



// Private Functions
//////////////////////

void deoglSkinStateController::pUpdateSkinVideoPlayers( deoglSkinState &skinState,
deoglSkinTexture &texture, deoglWorld &world ){
	deoglSharedVideoPlayerList &svplist = world.GetSharedVideoPlayerList();
	int i;
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( ! texture.IsChannelEnabled( ( deoglSkinChannel::eChannelTypes )i ) ){
			continue;
		}
		
		const deoglSkinChannel &channel = *texture.GetChannelAt( ( deoglSkinChannel::eChannelTypes )i );
		deoglSharedVideoPlayer *sharedVideoPlayer = NULL;
		const int vpindex = channel.GetVideoPlayer();
		deVideoPlayer *videoPlayer = NULL;
		
		deVideo *oldVideo = NULL;
		if( vpindex != -1 ){
			if( channel.GetSharedVideoPlayer() ){
				sharedVideoPlayer = GetSharedVideoPlayerAt( vpindex );
				if( sharedVideoPlayer ){
					oldVideo = sharedVideoPlayer->GetVideoPlayer()->GetVideo();
				}
				
			}else{
				videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetAt( vpindex );
				if( videoPlayer ){
					oldVideo = videoPlayer->GetVideo();
				}
			}
		}
		
		if( oldVideo == channel.GetVideo() ){
			continue;
		}
		
		if( channel.GetVideo() ){
			if( channel.GetSharedVideoPlayer() ){
				sharedVideoPlayer = NULL;
				
				try{
					// TODO hosting component or other object has to provide the playback speed. until
					//      this is done use 1.0f always.
					sharedVideoPlayer = svplist.GetVideoPlayerFor( channel.GetVideo(), 0, 1.0f );
					SetSharedVideoPlayerAt( vpindex, sharedVideoPlayer );
					skinState.SetVideoPlayerAt( vpindex, ( ( deoglVideoPlayer* )sharedVideoPlayer->
						GetVideoPlayer()->GetPeerGraphic() )->GetRVideoPlayer() );
					
				}catch( const deException & ){
					if( sharedVideoPlayer ){
						sharedVideoPlayer->FreeUsage();
					}
					throw;
				}
				
			}else{
				if( videoPlayer ){
					videoPlayer->SetVideo( channel.GetVideo() );
					
				}else{
					try{
						videoPlayer = world.GetOgl().GetGameEngine()->GetVideoPlayerManager()->CreateVideoPlayer();
						videoPlayer->SetVideo( channel.GetVideo() );
						videoPlayer->SetLooping( true );
						videoPlayer->SetPlaySpeed( 1.0f );
						videoPlayer->Play();
						pVideoPlayers.SetAt( vpindex, videoPlayer );
						videoPlayer->FreeReference();
						
					}catch( const deException & ){
						if( videoPlayer ){
							videoPlayer->FreeReference();
						}
						throw;
					}
					
					skinState.SetVideoPlayerAt( vpindex, ( ( deoglVideoPlayer* )
						videoPlayer->GetPeerGraphic() )->GetRVideoPlayer() );
				}
			}
			
		}else{
			if( channel.GetSharedVideoPlayer() ){
				skinState.SetVideoPlayerAt( vpindex, NULL );
				
			}else{
				pVideoPlayers.SetAt( vpindex, NULL );
			}
			
			SetSharedVideoPlayerAt( vpindex, NULL );
		}
	}
}
