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

#include <stdlib.h>

#include "dewiDevice.h"
#include "dewiDeviceAxis.h"
#include "dewiDeviceWinRTController.h"
#include "deWindowsInput.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>



// Class dewiDeviceAxis
//////////////////////////

// Constructor, destructor
////////////////////////////

dewiDeviceAxis::dewiDeviceAxis( deWindowsInput &module ) :
pModule( module ),
pIndex( -1 ),
pType( deInputDeviceAxis::eatGeneric ),
pMinimum( -100 ),
pMaximum( 100 ),
pDeadZoneLower( 0 ),
pDeadZoneUpper( 0 ),
pFuzz( 0 ),
pFlat( 0 ),
pAbsolute( true ),
pWheelOtherAxis( false ),
pWheelChange( 0 ),

pValue( 0.0f ),
pChangedValue( 0.0f ),

pWICode( -1 ),
pWinRTReadingIndexAxis( -1 ),
pWinRTInverseAxis( false ),
pWinRTReadingIndexSwitch( -1 ),
pWinRTReadingDirectionSwitch( 0 ),
pIsBatteryLevel( false ){
}

dewiDeviceAxis::~dewiDeviceAxis(){
}



// Management
///////////////

void dewiDeviceAxis::SetIndex( int index ){
	pIndex = index;
}

void dewiDeviceAxis::SetID( const char *id ){
	pID = id;
}

void dewiDeviceAxis::SetName( const char *name ){
	pName = name;
}

void dewiDeviceAxis::SetType( deInputDeviceAxis::eAxisTypes type ){
	pType = type;
}



void dewiDeviceAxis::SetDisplayImages( const char *name ){
	pDisplayImage = NULL;
	pDisplayIcons.RemoveAll();
	
	if( ! name ){
		return;
	}
	
	deImageManager &imageManager = *pModule.GetGameEngine()->GetImageManager();
	deVirtualFileSystem * const vfs = &pModule.GetVFS();
	const char * const basePath = "/share/image/axis";
	decString filename;
	
	filename.Format( "%s/%s/image.png", basePath, name );
	pDisplayImage.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
	
	const int sizes[ 4 ] = {128, 64, 32, 16};
	deImageReference icon;
	int i;
	
	for( i=0; i<4; i++ ){
		filename.Format( "%s/%s/icon%d.png", basePath, name, sizes[ i ] );
		icon.TakeOver( imageManager.LoadImage( vfs, filename, "/" ) );
		pDisplayIcons.Add( ( deImage* )icon );
	}
}

void dewiDeviceAxis::SetDisplayText( const char *text ){
	pDisplayText = text;
}



void dewiDeviceAxis::SetMinimum( int minimum ){
	pMinimum = minimum;
	pUpdateDeadZone();
}

void dewiDeviceAxis::SetMaximum( int maximum ){
	pMaximum = maximum;
	pUpdateDeadZone();
}

void dewiDeviceAxis::SetFuzz( int fuzz ){
	pFuzz = fuzz;
}

void dewiDeviceAxis::SetFlat( int flat ){
	pFlat = flat;
	pUpdateDeadZone();
}

void dewiDeviceAxis::SetAbsolute( bool absolute ){
	pAbsolute = absolute;
}

void dewiDeviceAxis::SetWheelOtherAxis( bool otherAxis ){
	pWheelOtherAxis = otherAxis;
}

void dewiDeviceAxis::SetWheelChange( int change, int modifiers, DWORD eventTime ){
	pWheelChange = change;
	pLastModifiers = modifiers;
	pLastEventTime = eventTime;
}

void dewiDeviceAxis::IncrementWheelChange( int amount, int modifiers, DWORD eventTime ){
	pWheelChange += amount;
	pLastModifiers = modifiers;
	pLastEventTime = eventTime;
}



void dewiDeviceAxis::SetValue( float value ){
	if( pAbsolute ){
		value = decMath::clamp( value, -1.0f, 1.0f );
	}
	pValue = value;
}



void dewiDeviceAxis::SetWICode( int code ){
	pWICode = code;
}

void dewiDeviceAxis::SetWinRTReadingIndexAxis( int index ){
	pWinRTReadingIndexAxis = index;
}

void dewiDeviceAxis::SetWinRTInverseAxis( bool winRTInverseAxis ){
	pWinRTInverseAxis = winRTInverseAxis;
}

void dewiDeviceAxis::SetWinRTReadingIndexSwitch( int index ){
	pWinRTReadingIndexSwitch = index;
}

void dewiDeviceAxis::SetWinRTReadingDirectionSwitch( int direction ){
	pWinRTReadingDirectionSwitch = direction;
}

void dewiDeviceAxis::SetIsBatteryLevel( bool isBatteryLevel ){
	pIsBatteryLevel = isBatteryLevel;
}


void dewiDeviceAxis::GetInfo( deInputDeviceAxis &info ) const{
	int i;
	
	info.SetID( pID );
	info.SetName( pName );
	info.SetType( pType );
	
	info.SetDisplayImage( pDisplayImage );
	for( i=0; i<pDisplayIcons.GetCount(); i++ ){
		info.AddDisplayIcon( ( deImage* )pDisplayIcons.GetAt( i ) );
	}
	info.SetDisplayText( pDisplayText );
}

void dewiDeviceAxis::SendEvents( dewiDevice &device ){
	if( pAbsolute ){
		if( fabsf( pChangedValue - pValue ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		pValue = pChangedValue;
		device.GetModule().AddAxisChanged( device.GetIndex(), pIndex, pValue, pLastEventTime );
		
	}else if( pType == deInputDeviceAxis::eatMouseWheel ){
		if( abs( pWheelChange ) < WHEEL_DELTA ){
			return;
		}
		
		const int change = pWheelChange / WHEEL_DELTA;
		pWheelChange = pWheelChange % WHEEL_DELTA;
		
		if( pWheelOtherAxis ){
			device.GetModule().AddMouseWheelChanged( device.GetIndex(), pIndex,
				change, 0, pLastModifiers, pLastEventTime );
			
		}else{
			device.GetModule().AddMouseWheelChanged( device.GetIndex(), pIndex,
				0, change, pLastModifiers, pLastEventTime );
		}
		
	}else{
		if( fabsf( pChangedValue ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		pValue = pChangedValue;
		pChangedValue = 0.0f;
		device.GetModule().AddAxisChanged( device.GetIndex(), pIndex, pValue, pLastEventTime );
	}
}

void dewiDeviceAxis::WinRTReading( dewiDeviceWinRTController &device ){
	// get reading depending on axis type
	int reading;
	
	if( pWinRTReadingIndexAxis != -1 ){
		double realReading = device.GetReadingAxis( pWinRTReadingIndexAxis );
		
		if( pWinRTInverseAxis ){
			realReading = 1.0 - realReading;
		}

		reading = ( int )decMath::linearStep( realReading, 0.0, 1.0, ( double )pMinimum, ( double )pMaximum );

	}else if( pWinRTReadingIndexSwitch != -1 ){
		switch( pWinRTReadingDirectionSwitch ){
		case 0:
			switch( device.GetReadingSwitch( pWinRTReadingIndexSwitch ) ){
			case wrgi::GameControllerSwitchPosition::Right:
			case wrgi::GameControllerSwitchPosition::UpRight:
			case wrgi::GameControllerSwitchPosition::DownRight:
				reading = pMaximum;
				break;

			case wrgi::GameControllerSwitchPosition::Left:
			case wrgi::GameControllerSwitchPosition::DownLeft:
			case wrgi::GameControllerSwitchPosition::UpLeft:
				reading = pMinimum;
				break;

			default:
				reading = ( pMinimum + pMaximum ) / 2;
			}
			break;

		case 1:
			switch( device.GetReadingSwitch( pWinRTReadingIndexSwitch ) ){
			case wrgi::GameControllerSwitchPosition::Up:
			case wrgi::GameControllerSwitchPosition::UpRight:
			case wrgi::GameControllerSwitchPosition::UpLeft:
				reading = pMaximum;
				break;

			case wrgi::GameControllerSwitchPosition::Down:
			case wrgi::GameControllerSwitchPosition::DownRight:
			case wrgi::GameControllerSwitchPosition::DownLeft:
				reading = pMinimum;
				break;

			default:
				reading = ( pMinimum + pMaximum ) / 2;
			}
			break;

		case -1:
			switch( device.GetReadingSwitch( pWinRTReadingIndexSwitch ) ){
			case wrgi::GameControllerSwitchPosition::Up:
			case wrgi::GameControllerSwitchPosition::UpRight:
			case wrgi::GameControllerSwitchPosition::UpLeft:
				reading = pMinimum;
				break;

			case wrgi::GameControllerSwitchPosition::Down:
			case wrgi::GameControllerSwitchPosition::DownRight:
			case wrgi::GameControllerSwitchPosition::DownLeft:
				reading = pMaximum;
				break;

			default:
				reading = ( pMinimum + pMaximum ) / 2;
			}
			break;

		default:
			reading = ( pMinimum + pMaximum ) / 2;
		}

	}else if(pIsBatteryLevel){
		reading = pMaximum;

		const wrdp::BatteryReport &report = device.GetBatteryReport();
		if(report){
			if( report.RemainingCapacityInMilliwattHours() && report.FullChargeCapacityInMilliwattHours() ){
				const float percentage = ( float )report.RemainingCapacityInMilliwattHours().GetInt32()
					/ ( float )report.FullChargeCapacityInMilliwattHours().GetInt32();
				reading = ( int )( percentage * ( float )pMaximum );
			}
		}

	}else{
		reading = ( pMinimum + pMaximum ) / 2;
	}

	// convert reading to value
	float value;

	if( reading < pDeadZoneLower ){
		value = decMath::linearStep( ( float )reading,
			( float )pMinimum, ( float )pDeadZoneLower, -1.0f, 0.0f );

	}else if( reading > pDeadZoneUpper ){
		value = decMath::linearStep( ( float )reading,
			( float )pDeadZoneUpper, ( float )pMaximum, 0.0f, 1.0f );

	}else{
		value = 0.0f;
	}

	// update
	if( pAbsolute ){
		pChangedValue = value;

	}else{
		pChangedValue += value;
	}

	if( fabsf( pChangedValue - pValue ) < FLOAT_SAFE_EPSILON ){
		return;
	}

	pLastEventTime = device.GetReadingTime();
	device.SetDirtyAxesValues( true );
}



// Private
////////////

void dewiDeviceAxis::pUpdateDeadZone(){
	const int center = ( pMinimum + pMaximum ) / 2;
	pDeadZoneLower = decMath::max( center - pFlat, pMinimum );
	pDeadZoneUpper = decMath::min( center + pFlat, pMaximum );
}
