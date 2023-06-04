/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ceWDSVAPreview.h"
#include "ceWindowDopeSheet.h"
#include "../ceWindowMain.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/playback/cePlayback.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasTextReference.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundDecoder.h>
#include <dragengine/resources/sound/deSoundDecoderReference.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>



// Actions
////////////

namespace {

class cMouseListener : public igdeMouseDragListener {
	enum eDragModes{
		edmNone,
		edmTime
	};
	
	ceWDSVAPreview &pVAPreview;
	eDragModes pDragMode;
	
public:
	cMouseListener( ceWDSVAPreview &lane ) : pVAPreview( lane ), pDragMode( edmNone ){ }
	
	virtual bool OnDragBegin(){
		ceCAActorSpeak * const actionSpeak = pVAPreview.GetWindow().GetActionASpeak();
		ceConversationAction * const action = pVAPreview.GetWindow().GetAction();
		
		pDragMode = edmNone;
		
		if( actionSpeak ){
			if( GetControl() ){
				pVAPreview.GetWindow().GetConversation()->GetPlayback()->PlaySingleAction( actionSpeak, 0.0f );
				
			}else{
				pVAPreview.SetCurTime( pVAPreview.GetWindow().GetTimeForX( GetDragOrigin().x ) );
				
				if( GetShift() ){
					pVAPreview.PlayVoiceAudio();
					
				}else{
					pDragMode = edmTime;
					return true;
				}
			}
			
		}else if( action ){
			if( GetControl() ){
				pVAPreview.GetWindow().GetConversation()->GetPlayback()->PlaySingleAction( action, 0.0f );
			}
		}
		
		return false;
	}
	
	virtual void OnDragUpdate(){
		switch( pDragMode ){
		case edmTime:
			pVAPreview.SetCurTime( pVAPreview.GetWindow().GetTimeForX( GetDragPosition().x ) );
			break;
			
		default:
			break;
		}
	}
	
	virtual void OnDragFinish( bool ){
		pDragMode = edmNone;
	}
	
	virtual void OnMouseWheeled( igdeWidget*, const decPoint &, const decPoint &change, int ){
		// maybe use this to "skip" back or ahead?
	}
	
	virtual void OnButtonPress( igdeWidget *widget, int button, const decPoint &position, int modifiers ){
		if( button != deInputEvent::embcRight ){
			igdeMouseDragListener::OnButtonPress( widget, button, position, modifiers );
			return;
		}
		
		/*
		igdeMenuCascadeReference contextMenu;
		contextMenu.TakeOver( new igdeMenuCascade( pVAPreview.GetWindow().GetEnvironment() ) );
		
		// TODO
		
		contextMenu->Popup( pVAPreview.GetWindow(), position );
		*/
	}
};

}



// Class ceWDSVAPreview
/////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSVAPreview::ceWDSVAPreview( ceWindowDopeSheet &dopeSheet ) :
pWindow( dopeSheet ),
pCurTime( 0.0f ),
pDirtyPreview( true ),
pPreviewSamples( NULL )
{
	pMouseKeyListener.TakeOver( new cMouseListener( *this ) );
}

ceWDSVAPreview::~ceWDSVAPreview(){
	if( pPreviewSamples ){
		delete [] pPreviewSamples;
	}
}



// Management
///////////////

void ceWDSVAPreview::SetCurTime( float time ){
	if( fabsf( time - pCurTime ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pCurTime = time;
	
	if( pSpeaker ){
		pSpeaker->Stop();
		
		const deSound * const sound = pSpeaker->GetSound();
		if( sound ){
			pSpeaker->SetPlayPosition( ( int )( pCurTime * ( float )sound->GetSampleRate() ),
				( int )( ( pCurTime + 1.0f ) * ( float )sound->GetSampleRate() ) );
		}
	}
	
	pWindow.OnTimeChanged();
}

void ceWDSVAPreview::SetSpeaker( deSpeaker *speaker ){
	if( pSpeaker == speaker ){
		return;
	}
	
	pSpeaker = speaker;
	
	OnActionChanged();
}



void ceWDSVAPreview::PlayVoiceAudio(){
	if( pSpeaker ){
		pSpeaker->Play();
	}
}



void ceWDSVAPreview::OnResize(){
	RebuildCanvas();
}

void ceWDSVAPreview::OnActionChanged(){
	if( pSpeaker ){
		ceCAActorSpeak * const action = pWindow.GetActionASpeak();
		pSpeaker->SetSound( action ? action->GetEngineSound() : NULL );
	}
	
	InvalidatePreview();
	RebuildCanvas();
}

void ceWDSVAPreview::InvalidatePreview(){
	if( pPreviewSamples ){
		delete [] pPreviewSamples;
		pPreviewSamples = NULL;
	}
	pDirtyPreview = true;
}



void ceWDSVAPreview::CreateCanvas(){
	deCanvasManager &canvasManager = *pWindow.GetEngine()->GetCanvasManager();
	pCanvas.TakeOver( canvasManager.CreateCanvasView() );
	
	pCanvasPreview.TakeOver( canvasManager.CreateCanvasImage() );
	pCanvasPreview->SetOrder( ( float )pCanvas->GetCanvasCount() );
	pCanvas->AddCanvas( pCanvasPreview );
}

void ceWDSVAPreview::UpdateCanvas(){
	if( ! pCanvas ){
		return;
	}
	
}

void ceWDSVAPreview::RebuildCanvas(){
	if( ! pCanvas ){
		return;
	}
	
	decPoint sizeDopeSheet( pWindow.GetSizeDopeSheet() );
	const int previewHeight = pWindow.GetVAPreviewHeight();
	const int width = sizeDopeSheet.x;
	
	pCanvas->SetPosition( decPoint( 0, 0 ) );
	pCanvas->SetSize( decPoint( width, previewHeight ) );
	
	// preview image
	pCanvasPreview->SetImage( NULL );
	pImagePreview.TakeOver( pWindow.GetEngine()->GetImageManager()->CreateImage( width, previewHeight, 1, 3, 8 ) );
	UpdateVAPreviewImage();
	pCanvasPreview->SetImage( pImagePreview );
}



// Private Functions
//////////////////////

void ceWDSVAPreview::UpdateVAPreviewImage(){
	const int width = pImagePreview->GetWidth();
	const int height = pImagePreview->GetHeight();
	const int silenceLine = height / 2;
	const sRGB8 colorBackground = { 255, 255, 255 };
	const sRGB8 colorWaveform = { 0, 0, 255 };
	const sRGB8 colorSilence = { 0, 0, 255 };
	sRGB8 * const pixels = pImagePreview->GetDataRGB8();
	
	// clear image
	const int pixelCount = width * height;
	int i;
	for( i=0; i<pixelCount; i++ ){
		pixels[ i ] = colorBackground;
	}
	
	// silence line
	sRGB8 * const pixelsSilenceLine = pixels + width * silenceLine;
	for( i=0; i<width; i++ ){
		pixelsSilenceLine[ i ] = colorSilence;
	}
	
	// wave form
	deSound * const sound = pSpeaker->GetSound();
	if( sound ){
		const int bytesPerSample = sound->GetBytesPerSample();
		const int sampleCount = sound->GetSampleCount();
		const int sampleRate = sound->GetSampleRate();
		
		// TODO we should do this asynchronous
		if( ! pPreviewSamples ){
			const int bufferSize = sampleCount * bytesPerSample * sound->GetChannelCount();
			pPreviewSamples = new char[ bufferSize ];
			deSoundDecoderReference decoder;
			
			try{
				decoder.TakeOver( pWindow.GetEngine()->GetSoundManager()->CreateDecoder( sound ) );
				decoder->ReadSamples( pPreviewSamples, bufferSize );
				
			}catch( const deException & ){
				// nothing we can do. leave the values in chaos
			}
		}
		
		const int lastSampleIndex = sampleCount - 1;
		int x;
		
		if( bytesPerSample == 1 ){
			const char * const samples = ( const char * )pPreviewSamples;
			const float factor = ( float )height / 255.0f;
			
			for( x=0; i<width; i++ ){
				const int sampleFrom = decMath::max( ( int )( pWindow.GetTimeForX( x ) * sampleRate + 0.5f ), 0 );
				const int sampleTo = decMath::min( ( int )( pWindow.GetTimeForX( x + 1 ) * sampleRate + 0.5f ), lastSampleIndex );
				
				int lowerValue = 0;
				int upperValue = 0;
				
				for( i=sampleFrom; i<sampleTo; i++ ){
					const int value = ( int )( factor * ( float )samples[ i ] );
					
					if( value < lowerValue ){
						lowerValue = value;
						
					}else if( value > upperValue ){
						upperValue = value;
					}
				}
				
				const int fromY = decMath::clamp( silenceLine + lowerValue, 0, height - 1 );
				const int toY = decMath::clamp( silenceLine + upperValue, 0, height - 1 );
				sRGB8 * const basePixels = pixels + x;
				for( i=fromY; i<=toY; i++ ){
					basePixels[ width * i ] = colorWaveform;
				}
			}
			
		}else if( bytesPerSample == 2 ){
			const short * const samples = ( const short * )pPreviewSamples;
			const float factor = ( float )height / 65536.0f;
			
			for( x=0; i<width; i++ ){
				const int sampleFrom = decMath::max( ( int )( pWindow.GetTimeForX( x ) * sampleRate + 0.5f ), 0 );
				const int sampleTo = decMath::min( ( int )( pWindow.GetTimeForX( x + 1 ) * sampleRate + 0.5f ), lastSampleIndex );
				
				int lowerValue = 0;
				int upperValue = 0;
				
				for( i=sampleFrom; i<sampleTo; i++ ){
					const int value = ( int )( factor * ( float )samples[ i ] );
					
					if( value < lowerValue ){
						lowerValue = value;
						
					}else if( value > upperValue ){
						upperValue = value;
					}
				}
				
				const int fromY = decMath::clamp( silenceLine + lowerValue, 0, height - 1 );
				const int toY = decMath::clamp( silenceLine + upperValue, 0, height - 1 );
				sRGB8 * const basePixels = pixels + x;
				for( i=fromY; i<=toY; i++ ){
					basePixels[ width * i ] = colorWaveform;
				}
			}
		}
	}
	
	// update image
	pImagePreview->NotifyImageDataChanged();
}
