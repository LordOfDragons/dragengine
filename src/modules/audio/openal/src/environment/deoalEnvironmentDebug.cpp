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

#include "deoalEnvironment.h"
#include "deoalEnvironmentDebug.h"
#include "deoalEnvProbe.h"
#include "deoalEnvProbeList.h"
#include "deoalEnvProbeListener.h"
#include "raytrace/deoalRayTraceResult.h"
#include "raytrace/deoalRayTraceHitElement.h"
#include "raytrace/deoalSoundRay.h"
#include "raytrace/deoalSoundRaySegment.h"
#include "raytrace/deoalEnergyHistogram.h"
#include "raytrace/visitors/deoalWOVRayHitsElement.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATDebug.h"
#include "../component/deoalAComponent.h"
#include "../component/deoalAComponentTexture.h"
#include "../configuration/deoalConfiguration.h"
#include "../devmode/deoalDevMode.h"
#include "../extensions/efx.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalAMicrophone.h"
#include "../model/deoalAModel.h"
#include "../model/deoalModelFace.h"
#include "../skin/deoalSkin.h"
#include "../skin/deoalSkinTexture.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasVisitorIdentify.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalEnvironmentDebug
////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvironmentDebug::deoalEnvironmentDebug( const deoalEnvironment &environment ) :
pEnvironment( environment ),

pHistogramSize( 52 * 3, 50 ),

// 64 slots, 3 bands, 0.5s time => ~7.8ms slots
//   one presentation talks about 10ms windows. this would be 50 slots
pEnergyHistogram( 50, 3, 0.5f )
{
}

deoalEnvironmentDebug::~deoalEnvironmentDebug(){
}



// Manegement
///////////////

void deoalEnvironmentDebug::Print(){
	deoalATLogger &logger = pEnvironment.GetAudioThread().GetLogger();
	
	const decDVector &position = pEnvironment.GetPosition();
	const decVector &revReflPan = pEnvironment.GetReverbReflectionPan();
	const decVector &lateRevPan = pEnvironment.GetReverbLateReverbPan();
	
	logger.LogInfoFormat( "Environment: position(%.3f,%.3f,%.3f) range=%.3f",
		position.x, position.y, position.z, pEnvironment.GetRange() );
	
	logger.LogInfoFormat( "- Gain: low=%.3f medium=%.3f high=%.3f",
		pEnvironment.GetGainLow(), pEnvironment.GetGainMedium(),
		pEnvironment.GetGainHigh() );
	
	logger.LogInfoFormat( "- Band Pass: gain=%.3f gainLF=%.3f gainHF=%.3f",
		pEnvironment.GetBandPassGain(), pEnvironment.GetBandPassGainLF(),
		pEnvironment.GetBandPassGainHF() );
	
	logger.LogInfoFormat( "- EAX Reverb: gain=%.3f gainLF=%.3f gainHF=%.3f decayTime=%.3f "
		"decayLF=%.3f, decayHF=%.3f reflGain=%.3f reflDelay=%.3f lrevGain=%.3f lrevDelay=%.3f "
		"echoDelay=%.3f reflPan=(%.3f,%.3f,%.3f)[%.3f] lrevPan=(%.3f,%.3f,%.3f)[%.3f]",
		pEnvironment.GetReverbGain(), pEnvironment.GetReverbGainLF(),
		pEnvironment.GetReverbGainHF(), pEnvironment.GetReverbDecayTime(),
		pEnvironment.GetReverbDecayLFRatio(), pEnvironment.GetReverbDecayHFRatio(),
		pEnvironment.GetReverbReflectionGain(), pEnvironment.GetReverbReflectionDelay(),
		pEnvironment.GetReverbLateReverbGain(), pEnvironment.GetReverbLateReverbDelay(),
		pEnvironment.GetReverbEchoTime(), revReflPan.x, revReflPan.y, revReflPan.z,
		revReflPan.Length(), lateRevPan.x, lateRevPan.y, lateRevPan.z, lateRevPan.Length() );
}



void deoalEnvironmentDebug::Prepare( const deoalEnvProbe &envProbe,
const deoalEnvProbeListener listener ){
	pImpulseResponse = listener.GetImpulseResponse();
	pImpulseResponseScale = ( float )envProbe.GetRayCount();
}



enum eDebugInfo{
	ediPosition,
	ediRange,
	ediGain, 
	ediBandPassGain,
	ediEAXReverb,
	ediEAXDecayTime,
	ediEAXEchoTime,
	ediEAXReflectionGain,
	ediEAXReflectionDelay,
	ediEAXReflectionPan,
	ediEAXLateReverbeGain,
	ediEAXLateReverbeDelay,
	ediEAXLateReverbePan,
};

void deoalEnvironmentDebug::UpdateInfo( deDebugBlockInfo &debugInfo ){
	if( debugInfo.GetEntryCount() == 0 ){
		const decColor ct( 1.0f, 1.0f, 1.0f );
		const decColor ceax( 0.2f, 0.0f, 0.0f, 0.5f );
		const decColor ceaxRefl( 0.2f, 0.2f, 0.0f, 0.5f );
		const decColor ceaxLateRev( 0.2f, 0.0f, 0.2f, 0.5f );
		
		debugInfo.AddEntry( "Position", "" );
		debugInfo.AddEntry( "Range", "" );
		debugInfo.AddEntry( "Gain", "" );
		debugInfo.AddEntry( "Band Pass Gain", "" );
		debugInfo.AddEntry( "EAX Reverb", "", ct, ceax );
		debugInfo.AddEntry( "  Decay Time", "", ct, ceax );
		debugInfo.AddEntry( "  Echo Time", "", ct, ceax );
		debugInfo.AddEntry( "  Refl. Gain", "", ct, ceaxRefl );
		debugInfo.AddEntry( "  Refl. Delay", "", ct, ceaxRefl );
		debugInfo.AddEntry( "  Refl. Pan", "", ct, ceaxRefl );
		debugInfo.AddEntry( "  LateRev. Gain", "", ct, ceaxLateRev );
		debugInfo.AddEntry( "  LateRev. Delay", "", ct, ceaxLateRev );
		debugInfo.AddEntry( "  LateRev. Pan", "", ct, ceaxLateRev );
		
		deEngine &engine = *pEnvironment.GetAudioThread().GetOal().GetGameEngine();
		deCanvasImage::Ref canvasHistogram(deCanvasImage::Ref::New( engine.GetCanvasManager()->CreateCanvasImage() ));
		deImage::Ref imageHistogram(deImage::Ref::New(engine.GetImageManager()->CreateImage(
			pHistogramSize.x, pHistogramSize.y, 1, 4, 8 )));
		memset( imageHistogram->GetDataRGBA8(), 0, sizeof( sRGBA8 ) * ( pHistogramSize.x * pHistogramSize.y ) );
		canvasHistogram->SetImage( imageHistogram );
		canvasHistogram->SetSize( pHistogramSize );
		debugInfo.GetView()->AddCanvas( canvasHistogram );
	}
	
	
	
	const decDVector &position = pEnvironment.GetPosition();
	const decVector &revReflPan = pEnvironment.GetReverbReflectionPan();
	const decVector &lateRevPan = pEnvironment.GetReverbLateReverbPan();
	
	
	
	// generic
	decString text;
	text.Format( "(%.3f,%.3f,%.3f)", position.x, position.y, position.z );
	debugInfo.SetEntryText( ediPosition, text );
	
	text.Format( "%.1f", pEnvironment.GetRange() );
	debugInfo.SetEntryText( ediRange, text );
	
	text.Format( "(%.3f,%.3f,%.3f)", pEnvironment.GetGainLow(),
		pEnvironment.GetGainMedium(), pEnvironment.GetGainHigh() );
	debugInfo.SetEntryText( ediGain, text );
	debugInfo.SetEntryColor( ediGain, pColorGradient( ( pEnvironment.GetGainLow()
		+ pEnvironment.GetGainMedium() + pEnvironment.GetGainHigh() ) / 3.0f ) );
	
	
	// direct sound path
	text.Format( "(%.3f,%.3f,%.3f)", pEnvironment.GetBandPassGain(),
		pEnvironment.GetBandPassGainLF(), pEnvironment.GetBandPassGainHF() );
	debugInfo.SetEntryText( ediBandPassGain, text );
	debugInfo.SetEntryColor( ediBandPassGain, pColorGradient( pEnvironment.GetBandPassGain()
		+ ( pEnvironment.GetGainMedium() + pEnvironment.GetGainHigh() ) / 2.0f ) );
	
	
	// indirect sound path
	float decayTimeLow = pEnvironment.GetReverbDecayTime() * pEnvironment.GetReverbDecayLFRatio();
	float decayTimeHigh = pEnvironment.GetReverbDecayTime() * pEnvironment.GetReverbDecayHFRatio();
	float decayTimeMedium = ( decayTimeLow + decayTimeHigh ) * 0.5f;
	//text.Format( "(%.3f,%.3f,%.3f)", pReverbDecayTime, pReverbDecayLFRatio, pReverbDecayHFRatio );
	text.Format( "(%.3f,%.3f,%.3f)", decayTimeLow, decayTimeMedium, decayTimeHigh );
	debugInfo.SetEntryText( ediEAXDecayTime, text );
	
	text.Format( "%.3f", pEnvironment.GetReverbEchoTime() );
	debugInfo.SetEntryText( ediEAXEchoTime, text );
	
	float gainLow = pEnvironment.GetReverbGain() * pEnvironment.GetReverbGainLF()
		* pEnvironment.GetReverbReflectionGain();
	float gainHigh = pEnvironment.GetReverbGain() * pEnvironment.GetReverbGainHF()
		* pEnvironment.GetReverbReflectionGain();
	float gainMedium = ( gainLow + gainHigh ) * 0.5f;
	text.Format( "(%.3f,%.3f,%.3f)", gainLow, gainMedium, gainHigh );
	debugInfo.SetEntryText( ediEAXReflectionGain, text );
	debugInfo.SetEntryColor( ediEAXReflectionGain, pColorGradient( gainMedium ) );
	
	text.Format( "%.3f", pEnvironment.GetReverbReflectionDelay() );
	debugInfo.SetEntryText( ediEAXReflectionDelay, text );
	
	gainLow = pEnvironment.GetReverbGain() * pEnvironment.GetReverbGainLF()
		* pEnvironment.GetReverbLateReverbGain();
	gainHigh = pEnvironment.GetReverbGain() * pEnvironment.GetReverbGainHF()
		* pEnvironment.GetReverbLateReverbGain();
	gainMedium = ( gainLow + gainHigh ) * 0.5f;
	text.Format( "(%.3f,%.3f,%.3f)", gainLow, gainMedium, gainHigh );
	debugInfo.SetEntryText( ediEAXLateReverbeGain, text );
	debugInfo.SetEntryColor( ediEAXLateReverbeGain, pColorGradient( gainMedium ) );
	
	text.Format( "%.3f", pEnvironment.GetReverbLateReverbDelay() );
	debugInfo.SetEntryText( ediEAXLateReverbeDelay, text );
	
	if( revReflPan.IsZero() ){
		text = "(+000,+000) %0.000";
	}else{
		const float elevation = atan2f( revReflPan.y, revReflPan.z ) / DEG2RAD;
		const float azimuth = atan2f( -revReflPan.x, revReflPan.z ) / DEG2RAD;
		text.Format( "(%+04d,%+04d) %.3f", ( int )( azimuth + 0.5f ),
			( int )( elevation + 0.5f ), revReflPan.Length() );
	}
	debugInfo.SetEntryText( ediEAXReflectionPan, text );
	debugInfo.SetEntryColor( ediEAXReflectionPan, pColorGradient( revReflPan.Length() ) );
	
	if( lateRevPan.IsZero() ){
		text = "(+000,+000) %0.000";
	}else{
		const float elevation = atan2f( lateRevPan.y, lateRevPan.z ) / DEG2RAD;
		const float azimuth = atan2f( -lateRevPan.x, lateRevPan.z ) / DEG2RAD;
		text.Format( "(%+04d,%+04d) %.3f", ( int )( azimuth + 0.5f ),
			( int )( elevation + 0.5f ), lateRevPan.Length() );
	}
	debugInfo.SetEntryText( ediEAXLateReverbePan, text );
	debugInfo.SetEntryColor( ediEAXLateReverbePan, pColorGradient( lateRevPan.Length() ) );
	
	debugInfo.UpdateView();
	
	
	
//	pShowImpulseResponse( debugInfo );
	pShowEnergyHistogram( debugInfo );
}

void deoalEnvironmentDebug::SoundRays( deDebugDrawer &debugDrawer, const deoalEnvProbe &envProbe ){
	debugDrawer.RemoveAllShapes();
	debugDrawer.SetPosition( pEnvironment.GetPosition() );
	
	const deoalSoundRayList &srlist = envProbe.GetSoundRayList();
	const int rayCount = srlist.GetRayCount();
	const float hsvFactor = 6.0f / ( float )( decMath::max( rayCount - 1, 1 ) );
	const float colorA = 0.5f;
	decColor color;
	int i, j;
	
	for( i=0; i<rayCount; i++ ){
		const deoalSoundRay &ray = srlist.GetRayAt( i );
		const int segmentCount = ray.GetSegmentCount();
		if( segmentCount == 0 ){
			continue;
		}
		
		// colorize rays. HSV to RGB (H=0..360, S=0..1, V=0..1):
		// c = v * s = 1  // because v=1 and s=1
		// t = h / 60  // with H=0..1 => t = h * 6
		// x = c * (1 - abs(mod(t, 2) - 1))
		//   = 1 - abs(mod(t, 2) - 1)
		// m = v - c = 0
		// switch(t):
		// case 0: rgb = (c+m, x+m, m) = (1, x, 0)
		// case 1: rgb = (x+m, c+m, m) = (x, 1, 0)
		// case 2: rgb = (m, c+m, x+m) = (0, 1, x)
		// case 3: rgb = (m, x+m, c+m) = (0, x, 1)
		// case 4: rgb = (x+m, m, c+m) = (x, 0, 1)
		// case 5: rgb = (c+m, m, x+m) = (1, 0, x)
		const float hsvT = hsvFactor * i;
		const float hsvX = 1.0f - fabsf( fmodf( hsvT, 2.0f ) - 1.0f );
		
		switch( ( int )hsvT ){
		case 0: color.Set( 1.0f, hsvX, 0.0f, colorA ); break;
		case 1: color.Set( hsvX, 1.0f, 0.0f, colorA ); break;
		case 2: color.Set( 0.0f, 1.0f, hsvX, colorA ); break;
		case 3: color.Set( 0.0f, hsvX, 1.0f, colorA ); break;
		case 4: color.Set( hsvX, 0.0f, 1.0f, colorA ); break;
		case 5:
		default: color.Set( 1.0f, 0.0f, hsvX, colorA );
		}
		
		deDebugDrawerShape * const shape = new deDebugDrawerShape;
		shape->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		shape->SetEdgeColor( color );
		
		const int firstSegment = ray.GetFirstSegment();
		
		for( j=0; j<segmentCount; j++ ){
			const deoalSoundRaySegment &segment = srlist.GetSegmentAt( firstSegment + j );
			
			deDebugDrawerShapeFace * const face = new deDebugDrawerShapeFace;
			face->AddVertex( segment.GetPosition() );
			face->AddVertex( segment.GetPosition() + segment.GetDirection() * segment.GetLength() );
			face->AddVertex( segment.GetPosition() );
			face->SetNormal( decVector( 0.0f, 0.0f, 1.0f ) );
			shape->AddFace( face );
		}
		
		debugDrawer.AddShape( shape );
	}
	
	debugDrawer.NotifyShapeGeometryChanged();
}



// Private Functions
//////////////////////

void deoalEnvironmentDebug::pShowImpulseResponse( deDebugBlockInfo &debugInfo ){
	deCanvas *canvas = debugInfo.GetView()->GetRootCanvas();
	deCanvasVisitorIdentify identify;
	while( canvas ){
		canvas->Visit( identify );
		if( canvas->GetSize() == pHistogramSize && identify.IsImage() ){
			break;
		}
		canvas = canvas->GetLLViewNext();
	}
	if( ! canvas ){
		return;
	}
	
	deCanvasImage &canvasImage = identify.CastToImage();
	if( canvasImage.GetImage() ){
		deImage &image = *canvasImage.GetImage();
		sRGBA8 * const pixels = image.GetDataRGBA8();
		
		memset( pixels, 0, sizeof( sRGBA8 ) * ( pHistogramSize.x * pHistogramSize.y ) );
		
		const int count = pImpulseResponse.GetCount();
		int i, j, k;
		
		const int offsetBottom = pHistogramSize.x * ( pHistogramSize.y - 1 );
		const sRGB8 bandColors[ 3 ] = { { 255, 0, 0 }, { 0, 255, 0 }, { 128, 128, 255 } };
		const float timeScale = 50.0 / 0.5f; // 50 pixels on 0.5s
		
		for( i=0; i<3; i++ ){
			const int offsetBand = 52 * i;
			sRGBA8 * const bandPixels = pixels + offsetBand;
			const sRGB8 &bandColor = bandColors[ i ];
			
			for( j=0; j<count; j++ ){
				const deoalImpulseResponse::sImpulse &impulse = pImpulseResponse.GetAt( j );
				const int baseOffset = ( int )( impulse.time * timeScale + 0.5f );
				if( baseOffset < 0 || baseOffset > 49 || offsetBand + baseOffset >= pHistogramSize.x ){
					continue;
				}
				
				float gain;
				if( i == 0 ){
					gain = impulse.low;
				}else if( i == 1 ){
					gain = impulse.medium;
				}else{
					gain = impulse.high;
				}
				
				int offset = offsetBottom + baseOffset;
				const int height = decMath::clamp( ( int )( gain * pImpulseResponseScale
					* pHistogramSize.y - 0.5f ), 0, pHistogramSize.y - 1 );
				for( k=0; k<height; k++ ){
					sRGBA8 &pixel = bandPixels[ offset ];
					pixel.red = bandColor.red;
					pixel.green = bandColor.green;
					pixel.blue = bandColor.blue;
					pixel.alpha = 255;
					offset -= pHistogramSize.x;
				}
			}
			
			unsigned char fill[ 3 ] = { 192, 192, 192 };
			fill[ i ] = 255;
			for( j=0; j<pHistogramSize.y; j++ ){
				sRGBA8 * const row = pixels + ( pHistogramSize.x * j + offsetBand );
				for( k=0; k<50; k++ ){
					if( offsetBand + k >= pHistogramSize.x ){
						continue;
					}
					if( row[ k ].alpha == 255 ){
						continue;
					}
					
					row[ k ].red = fill[ 0 ];
					row[ k ].green = fill[ 1 ];
					row[ k ].blue = fill[ 2 ];
					row[ k ].alpha = 32;
				}
			}
		}
		
		image.NotifyImageDataChanged();
	}
	
	deCanvasView &view = *debugInfo.GetView();
	decPoint point = view.GetSize();
	point.y += debugInfo.GetSpacing();
	canvasImage.SetPosition( decPoint( debugInfo.GetPadding(), point.y ) );
	point.y += canvasImage.GetSize().y;
	view.SetSize( point );
}

void deoalEnvironmentDebug::pShowEnergyHistogram( deDebugBlockInfo &debugInfo ){
	deCanvas *canvas = debugInfo.GetView()->GetRootCanvas();
	deCanvasVisitorIdentify identify;
	while( canvas ){
		canvas->Visit( identify );
		if( canvas->GetSize() == pHistogramSize && identify.IsImage() ){
			break;
		}
		canvas = canvas->GetLLViewNext();
	}
	if( ! canvas ){
		return;
	}
	
	// build energy histogram
	pEnergyHistogram.Clear();
	
	const int impulseCount = pImpulseResponse.GetCount();
	int i;
	
	for( i=0; i<impulseCount; i++ ){
		const deoalImpulseResponse::sImpulse &impulse = pImpulseResponse.GetAt( i );
		const float entries[ 3 ] = { impulse.low, impulse.medium, impulse.high };
		pEnergyHistogram.AddMax( impulse.time, entries );
// 		pEnvironment.GetAudioThread().GetLogger().LogInfoFormat( "Impulse: time=%f energy=(%f,%f,%f)",
// 			impulse.time, impulse.low, impulse.medium, impulse.high );
		
		// NOTE correctly the impulse has to be smeared across the entire impinge time.
		//      the ray travels a certain time inside the spherical receiver. this yields
		//      the timeIn and timeOut values. the impulse would have to be inserted
		//      into the slots slotIn..slotOut corresponding to the timeIn..timeOut
		//      range with intensity I/(slotOut-slotIn+1), hence by the number of
		//      covered slots.
		//      
		//      this is important for the Integrate() method to work properly with
		//      deltaT of different sizes
	}
	
	// build information
	deCanvasImage &canvasImage = identify.CastToImage();
	if( canvasImage.GetImage() ){
		deImage &image = *canvasImage.GetImage();
		sRGBA8 * const pixels = image.GetDataRGBA8();
		
		memset( pixels, 0, sizeof( sRGBA8 ) * ( pHistogramSize.x * pHistogramSize.y ) );
		
		const int slotCount = decMath::min( pEnergyHistogram.GetSlotCount(), pHistogramSize.x );
		const int bandCount = decMath::min( pEnergyHistogram.GetBandCount(), 3 );
		int j, k;
		
		const float * const entries = pEnergyHistogram.GetEntries();
		const int offsetBottom = pHistogramSize.x * ( pHistogramSize.y - 1 );
		const sRGB8 bandColors[ 3 ] = { { 255, 0, 0 }, { 0, 255, 0 }, { 128, 128, 255 } };
		
		for( i=0; i<bandCount; i++ ){
			const int offsetBand = ( slotCount + 2 ) * i;
			sRGBA8 * const bandPixels = pixels + offsetBand;
			const float * const bandEntries = entries + slotCount * i;
			const sRGB8 &bandColor = bandColors[ i ];
			
			for( j=0; j<slotCount; j++ ){
				if( offsetBand + j >= pHistogramSize.x ){
					continue;
				}
				
				int offset = offsetBottom + j;
				const int height = decMath::clamp( ( int )( bandEntries[ j ]
					* pImpulseResponseScale * pHistogramSize.y - 0.5f ),
					0, pHistogramSize.y - 1 );
				for( k=0; k<height; k++ ){
					sRGBA8 &pixel = bandPixels[ offset ];
					pixel.red = bandColor.red;
					pixel.green = bandColor.green;
					pixel.blue = bandColor.blue;
					pixel.alpha = 255;
					offset -= pHistogramSize.x;
				}
			}
			
			unsigned char fill[ 3 ] = { 192, 192, 192 };
			fill[ i ] = 255;
			for( j=0; j<pHistogramSize.y; j++ ){
				sRGBA8 * const row = pixels + ( pHistogramSize.x * j + offsetBand );
				for( k=0; k<slotCount; k++ ){
					if( offsetBand + k >= pHistogramSize.x ){
						continue;
					}
					if( row[ k ].alpha == 255 ){
						continue;
					}
					
					row[ k ].red = fill[ 0 ];
					row[ k ].green = fill[ 1 ];
					row[ k ].blue = fill[ 2 ];
					row[ k ].alpha = 32;
				}
			}
			
			// line test
			if( true ){
				int lineX1 = 0;
				float largest = *bandEntries;
				for( j=1; j<slotCount; j++ ){
					if( bandEntries[ j ] > largest ){
						largest = bandEntries[ j ];
						lineX1 = j;
					}
				}
				
				int lineY1 = decMath::clamp( ( int )( largest * pImpulseResponseScale
					* pHistogramSize.y - 0.5f ), 0, pHistogramSize.y - 1 );
				int offset = offsetBottom - pHistogramSize.x * lineY1;
				for( j=0; j<=lineX1; j++ ){
					if( offsetBand + j >= pHistogramSize.x ){
						continue;
					}
					sRGBA8 &pixel = bandPixels[ offset + j ];
					if( pixel.alpha == 255 ){
						pixel.red = ( pixel.red + 255 ) / 2;
						pixel.green = ( pixel.green + 255 ) / 2;
						pixel.blue = ( pixel.blue + 255 ) / 2;
					}else{
						pixel.red = 255;
						pixel.green = 255;
						pixel.blue = 255;
						pixel.alpha = 128;
					}
				}
				
				int lineX2;
				for( lineX2=slotCount-1; lineX2>lineX1+1; lineX2-- ){
					if( bandEntries[ lineX2 ] > FLOAT_SAFE_EPSILON ){
						break;
					}
				}
				
				int lineY2 = decMath::clamp( ( int )( bandEntries[ lineX2 ]
					* pImpulseResponseScale * pHistogramSize.y - 0.5f ),
					0, pHistogramSize.y - 1 );
				const int diffY = lineY2 - lineY1;
				const int diffX = lineX2 - lineX1;
				for( j=lineX1+1; j<slotCount; j++ ){
					if( offsetBand + j >= pHistogramSize.x ){
						continue;
					}
					const int height = lineY1 + diffY * ( j - lineX1 ) / diffX;
					if( height < 0 || height >= pHistogramSize.y ){
						continue;
					}
					offset = offsetBottom - pHistogramSize.x * height;
					sRGBA8 &pixel = bandPixels[ offset + j ];
					if( pixel.alpha == 255 ){
						pixel.red = ( pixel.red + 255 ) / 2;
						pixel.green = ( pixel.green + 255 ) / 2;
						pixel.blue = ( pixel.blue + 255 ) / 2;
					}else{
						pixel.red = 255;
						pixel.green = 255;
						pixel.blue = 255;
						pixel.alpha = 128;
					}
				}
				
				
				
			}
		}
		
		image.NotifyImageDataChanged();
	}
	
	deCanvasView &view = *debugInfo.GetView();
	decPoint point = view.GetSize();
	point.y += debugInfo.GetSpacing();
	canvasImage.SetPosition( decPoint( debugInfo.GetPadding(), point.y ) );
	point.y += canvasImage.GetSize().y;
	view.SetSize( point );
	
	// integration test
	/*
	float integrations[3];
	pEnergyHistogram.Integrate(integrations);
	printf("Integrations: %f %f %f\n", integrations[0], integrations[1], integrations[2]);
	int i;
	decString text( "Energies:" );
	for( i=0; i<pEnergyHistogram.GetSlotCount(); i++ ){
		text.AppendFormat( " %f", pEnergyHistogram.GetEntries()[i] );
	}
	printf("%s\n", text.GetString());
	*/
}

decColor deoalEnvironmentDebug::pColorGradient( float value ) const{
	const decColor cmin( 1.0f, 0.0f, 0.0f );
	const decColor cmid( 1.0f, 1.0f, 0.0f );
	const decColor cmax( 0.0f, 1.0f, 0.0f );
	if( value < 0.5f ){
		return cmin * decMath::linearStep( value, 0.0f, 0.5f, 1.0f, 0.0f )
			+ cmid * decMath::linearStep( value, 0.0f, 0.5f, 0.0f, 1.0f );
		
	}else{
		return cmid * decMath::linearStep( value, 0.5f, 1.0f, 1.0f, 0.0f )
			+ cmax * decMath::linearStep( value, 0.5f, 1.0f, 0.0f, 1.0f );
	}
}
