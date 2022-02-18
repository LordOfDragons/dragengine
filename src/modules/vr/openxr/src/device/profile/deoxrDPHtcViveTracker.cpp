/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoxrDPHtcViveTracker.h"
#include "../../deVROpenXR.h"
#include "../../deoxrInstance.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/xmlparser/decXmlParser.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlVisitor.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class deoxrDPHtcViveTracker::Tracker
/////////////////////////////////////////

deoxrDPHtcViveTracker::Tracker::Tracker( const deoxrPath &path, int number ) :
path( path ),
number( number ){
}

deoxrDPHtcViveTracker::Tracker::~Tracker(){
}


// Class deoxrDPHtcViveTracker
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDPHtcViveTracker::deoxrDPHtcViveTracker( deoxrInstance &instance ) :
deoxrDeviceProfile( instance,
	deoxrPath( instance, "/interaction_profiles/htc/vive_tracker_htcx" ),
	"HTC VIVE Tracker" ),
pTimeoutCheckAttached( 3.0f )
{
// 	pLoadTrackerDatabase();
}

deoxrDPHtcViveTracker::~deoxrDPHtcViveTracker(){
}



// Management
///////////////

void deoxrDPHtcViveTracker::OnSessionStateChanged(){
	if( GetInstance().GetOxr().GetSessionState() == XR_SESSION_STATE_FOCUSED ){
		pTimerCheckAttached.Reset();
		pTimeoutCheckAttached = 1.0f;
	}
}

void deoxrDPHtcViveTracker::OnActionsSynced(){
	if( GetInstance().GetOxr().GetSessionState() != XR_SESSION_STATE_FOCUSED ){
		return;
	}
	
	pTimeoutCheckAttached -= pTimerCheckAttached.GetElapsedTime();
	if( pTimeoutCheckAttached > 0.0f ){
		return;
	}
	
	pTimeoutCheckAttached = 1e6f; // 3.0f;
	CheckAttached();
}

void deoxrDPHtcViveTracker::CheckAttached(){
	const deoxrInstance &instance = GetInstance();
	if( ! instance.SupportsExtension( deoxrInstance::extHTCXViveTrackerInteraction ) ){
		return;
	}
	
	deoxrDeviceManager &devices = GetInstance().GetOxr().GetDevices();
	
	// enumerate all connected trackers. unique path are of the shape:
	// "/devices/htc/vive_trackerlhr-{8-digit serial}"
	uint32_t count, i;
	int t;
	
	OXR_CHECK( instance.xrEnumerateViveTrackerPathsHTCX( instance.GetInstance(), 0, &count, nullptr ) );
	
	XrViveTrackerPathsHTCX *trackerPaths = nullptr;
	instance.GetOxr().LogInfo( "VIVE Trackers:" );
	
	if( count > 0 ){
		try{
			trackerPaths = new XrViveTrackerPathsHTCX[ count ];
			memset( trackerPaths, 0, sizeof( XrViveTrackerPathsHTCX ) * count );
			for( i=0; i<count; i++ ){
				trackerPaths[ i ].type = XR_TYPE_VIVE_TRACKER_PATHS_HTCX;
			}
			
			OXR_CHECK( instance.xrEnumerateViveTrackerPathsHTCX(
				instance.GetInstance(), count, &count, trackerPaths ) );
			
			for( i=0; i<count; i++ ){
				const deoxrPath path( instance, trackerPaths[ i ].persistentPath );
				const deoxrPath pathRole( instance, trackerPaths[ i ].rolePath );
				instance.GetOxr().LogInfoFormat( "- %d: path='%s' rolePath='%s'",
					i, path.GetName().GetString(), pathRole.GetName().GetString() );
			}
			
			// remove devices of no more connected trackers or trackers having changed role
			for( t=0; t<pTrackers.GetCount(); t++ ){
				Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( t ) );
				if( ! tracker.device ){
					continue;
				}
				
				for( i=0; i<count; i++ ){
					if( tracker.path == trackerPaths[ i ].persistentPath ){
						break;
					}
				}
				
				if( i == count || tracker.pathRole != trackerPaths[ i ].rolePath ){
					devices.Remove( tracker.device );
					tracker.device = nullptr;
					
					if( i < count && tracker.pathRole != trackerPaths[ i ].rolePath ){
						instance.GetOxr().LogInfoFormat(
							"VIVE Tracker changed role, request session restart: path='%s' rolePath='%s'",
							tracker.path.GetName().GetString(), tracker.pathRole.GetName().GetString() );
						
						instance.GetOxr().RequestRestartSession();
					}
				}
			}
			
			// add devices for newly connected trackers or trackers having change role path
			for( i=0; i<count; i++ ){
				Tracker * const tracker = pGetTrackerWith( trackerPaths[ i ].persistentPath );
				if( tracker ){
					// tracker is known and a device can be created for it. this happens if
					// the device has been activated before the engine started or the session
					// had been restarted after the device has been activated
					if( tracker->pathRole != trackerPaths[ i ].rolePath ){
						tracker->pathRole = deoxrPath( instance, trackerPaths[ i ].rolePath );
					}
					
					if( ! tracker->device ){
						// do not add device if the session is about to restart
						if( instance.GetOxr().GetRestartSession() ){
							continue;
						}
						
						pAddDevice( *tracker );
					}
				
				}else{
					// tracker has not been seen before the session started and has
					// been activated. we have to store the role and restart the
					// session to properly use it
					const Tracker::Ref newTracker( Tracker::Ref::New( new Tracker(
						deoxrPath( instance, trackerPaths[ i ].persistentPath ),
						pTrackers.GetCount() + 1 ) ) );
					
					newTracker->pathRole = deoxrPath( instance, trackerPaths[ i ].rolePath );
					
					pTrackers.Add( newTracker );
					
// 					pSaveTrackerDatabase();
					
					instance.GetOxr().LogInfoFormat(
						"VIVE Tracker first seen, request session restart: path='%s' rolePath='%s'",
						newTracker->path.GetName().GetString(), newTracker->pathRole.GetName().GetString() );
					
					instance.GetOxr().RequestRestartSession();
				}
			}
			
			delete [] trackerPaths;
			
		}catch( const deException & ){
			delete [] trackerPaths;
			throw;
		}
		
	}else{
		pRemoveAllDevices();
	}
}

void deoxrDPHtcViveTracker::CreateActions( deoxrActionSet &actionSet ){
	const int count = pTrackers.GetCount();
	decString name;
	int i;
	
	for( i=0; i<count; i++ ){
		Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
		
// 		if( ! tracker.pathRole ){
// 			continue;
// 		}
		
		name.Format( "pose_tracker_%s", pSerialFromPath( tracker.path ).GetString() );
		
		const XrPath subactionPath[ 2 ] = { tracker.path, tracker.pathRole };
		
		tracker.action = actionSet.AddAction( deoxrAction::etInputPose, name,
			pLocalizedNameForTracker( tracker ), subactionPath, tracker.pathRole ? 2 : 1 );
	}
}

void deoxrDPHtcViveTracker::SuggestBindings(){
	// Valid for top level user path:
	// - VIVE tracker persistent path (unspecified format, enumerate)
	// - /user/vive_tracker_htcx/role/<role-type> (find by assigned role)
	//   - XR_NULL_PATH
	//   - handheld_object
	//   - left_foot
	//   - right_foot
	//   - left_shoulder
	//   - right_shoulder
	//   - left_elbow
	//   - right_elbow
	//   - left_knee
	//   - right_knee
	//   - waist
	//   - chest
	//   - camera
	//   - keyboard
	// 
	// Supported component paths:
	// - /input/system/click (may not be available for application use)
	// - /input/menu/click
	// - /input/trigger/click
	// - /input/squeeze/click
	// - /input/trigger/value
	// - /input/trackpad/x
	// - /input/trackpad/y
	// - /input/trackpad/click
	// - /input/trackpad/touch
	// - /input/grip/pose
	// - /output/haptic
	
	const int count = pTrackers.GetCount();
	if( count == 0 ){
		return;
	}
	
	// NOTE if two or more trackers are assigned to the same role or trackers are set to
	//      disabled SteamVR starts spamming connection events at high frequency alternating
	//      between connect/remove tracker. this causes devices to be added/removed many
	//      times per frame update causing slow-down and other problems. there is no
	//      protection against this behavior
	
#if 1
	const deoxrInstance &instance = GetInstance();
	
	/*
	const int roleCount = 13;
	const char * const roles[ roleCount ] = { "handheld_object", "left_foot", "right_foot",
		"left_shoulder", "right_shoulder", "left_elbow", "right_elbow", "left_knee",
		"right_knee", "waist", "chest", "camera", "keyboard" };
	*/
	
	const int bindingCount = 10 * count;
// 	const int bindingCount = roleCount * count;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	
	int realBindingCount = 0;
	
	int i;
	for( i=0; i<pTrackers.GetCount(); i++ ){
		const Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
		if( ! tracker.action ){
			continue;
		}
		
		if( ! tracker.pathRole ){
			continue; // persistent path not accepted by SteamVR. returns unsupported path error
		}
		
// 		const decString basePath( tracker.path.GetName() );
		const decString basePath( tracker.pathRole.GetName() );
		
		/*
		const decString basePath( "/user/vive_tracker_htcx/role/" );
		int j;
		for( j=0; j<roleCount; j++ ){
			( b++ )->Set( tracker.action, deoxrPath( instance, basePath + roles[ j ] + "/input/grip/pose" ) );
		}
		*/
		
		( b++ )->Set( tracker.action, deoxrPath( instance, basePath + "/input/grip/pose" ) );
		
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/menu/click" );
		pAdd( b, deVROpenXR::eiaButtonSecondaryPress, basePath + "/input/system/click" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
		
		realBindingCount += 10;
	}
	
	GetInstance().SuggestBindings( GetPath(), bindings, realBindingCount );
#endif
}

void deoxrDPHtcViveTracker::ClearActions(){
	pRemoveAllDevices();
	
	const int count = pTrackers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( Tracker* )pTrackers.GetAt( i ) )->action = nullptr;
	}
}



// Private Functions
//////////////////////

deoxrDPHtcViveTracker::Tracker *deoxrDPHtcViveTracker::pGetTrackerWith( deoxrDevice *device ) const{
	const int count = pTrackers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		Tracker * const tracker = ( Tracker* )pTrackers.GetAt( i );
		if( tracker->device == device ){
			return tracker;
		}
	}
	
	return nullptr;
}

deoxrDPHtcViveTracker::Tracker *deoxrDPHtcViveTracker::pGetTrackerWith( XrPath path ) const{
	const int count = pTrackers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		Tracker * const tracker = ( Tracker* )pTrackers.GetAt( i );
		if( tracker->path == path ){
			return tracker;
		}
	}
	
	return nullptr;
}

decString deoxrDPHtcViveTracker::pSerialFromPath( const deoxrPath &path ) const{
	// persistent path are of the shape: "/devices/htc/vive_trackerlhr-{8-digits}"
	// the serial in OpenVR had been reported as "lhr-{8-digits}"
	// we simply take the last 12 characters
	return path.GetName().GetRight( 12 );
}

decString deoxrDPHtcViveTracker::pNameForTracker( const Tracker &tracker ) const{
	if( tracker.pathRole.GetName().EndsWith( "handheld_object" ) ){
		return "Tracker Hand Held";
		
	}else if( tracker.pathRole.GetName().EndsWith( "left_foot" ) ){
		return "Tracker Left Foot";
		
	}else if( tracker.pathRole.GetName().EndsWith( "right_foot" ) ){
		return "Tracker Right Foot";
		
	}else if( tracker.pathRole.GetName().EndsWith( "left_shoulder" ) ){
		return "Tracker Left Shoulder";
		
	}else if( tracker.pathRole.GetName().EndsWith( "right_shoulder" ) ){
		return "Tracker Right Shoulder";
		
	}else if( tracker.pathRole.GetName().EndsWith( "left_elbow" ) ){
		return "Tracker Left Elbow";
		
	}else if( tracker.pathRole.GetName().EndsWith( "right_elbow" ) ){
		return "Tracker Right Elbow";
		
	}else if( tracker.pathRole.GetName().EndsWith( "left_knee" ) ){
		return "Tracker Left Knee";
		
	}else if( tracker.pathRole.GetName().EndsWith( "right_knee" ) ){
		return "Tracker Right Knee";
		
	}else if( tracker.pathRole.GetName().EndsWith( "waist" ) ){
		return "Tracker Waist";
		
	}else if( tracker.pathRole.GetName().EndsWith( "chest" ) ){
		return "Tracker Chest";
		
	}else if( tracker.pathRole.GetName().EndsWith( "camera" ) ){
		return "Tracker Camera";
		
	}else if( tracker.pathRole.GetName().EndsWith( "keyboard" ) ){
		return "Tracker Keyboard";
		
	}else if( tracker.path.IsNotEmpty() ){
		decString name;
		name.Format( "Tracker %s", pSerialFromPath( tracker.path ).GetString() );
		return name;
		
	}else{
		decString name;
		name.Format( "Tracker #%d", tracker.number );
		return name;
	}
}

decString deoxrDPHtcViveTracker::pLocalizedNameForTracker( const Tracker &tracker ) const{
	decString name;
	
	if( tracker.path.IsNotEmpty() ){
		name.Format( "Tracker %s", pSerialFromPath( tracker.path ).GetString() );
		
	}else{
		name.Format( "Tracker #%d", tracker.number );
	}
	
	return name;
}

void deoxrDPHtcViveTracker::pRemoveAllDevices(){
	deoxrDeviceManager &devices = GetInstance().GetOxr().GetDevices();
	const int count = pTrackers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
		if( tracker.device ){
			devices.Remove( tracker.device );
			tracker.device = nullptr;
		}
	}
}

#define PATH_VIVETRACKER_XML "/config/vivetrackers.xml"

void deoxrDPHtcViveTracker::pLoadTrackerDatabase(){
	const deoxrInstance &instance = GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	oxr.LogInfoFormat( "Load VIVE Trackers..." );
	
	pRemoveAllDevices();
	pTrackers.RemoveAll();
	
	try{
		deVirtualFileSystem& vfs = oxr.GetVFS();
		
		const decPath filePath( decPath::CreatePathUnix( PATH_VIVETRACKER_XML ) );
		if( ! vfs.CanReadFile( filePath ) ){
			return;
		}
		
		const decBaseFileReader::Ref fileReader( vfs.OpenFileForReading( filePath ) );
		const decXmlDocument::Ref xmlDoc( decXmlDocument::Ref::New( new decXmlDocument ) );
		decXmlParser( oxr.GetGameEngine()->GetLogger() ).ParseXml( fileReader, xmlDoc );
		xmlDoc->StripComments();
		xmlDoc->CleanCharData();
		
		decXmlElementTag * const root = xmlDoc->GetRoot();
		if( ! root || root->GetName() != "viveTrackers" ){
			DETHROW( deeInvalidParam );
		}
		
		const int elementCount = root->GetElementCount();
		int i;
		for( i=0; i<elementCount; i++ ){
			decXmlElement * const element = root->GetElementAt( i );
			if( ! element->CanCastToElementTag() ){
				continue;
			}
			
			decXmlElementTag &tag = *element->CastToElementTag();
			if( tag.GetName() == "viveTracker" ){
				deoxrPath path;
				int number = 0;
				
				const int elementCount2 = tag.GetElementCount();
				int j;
				for( j=0; j<elementCount2; j++ ){
					decXmlElement * const element2 = tag.GetElementAt( j );
					if( ! element2->CanCastToElementTag() ){
						continue;
					}
					
					decXmlElementTag &tag2 = *element2->CastToElementTag();
					if( tag2.GetName() == "path" ){
						const decXmlCharacterData * const cdata = tag2.GetFirstData();
						if( cdata ){
							path = deoxrPath( instance, cdata->GetData() );
						}
						
					}else if( tag2.GetName() == "number" ){
						const decXmlCharacterData * const cdata = tag2.GetFirstData();
						if( cdata ){
							number = cdata->GetData().ToInt();
						}
					}
				}
				
				if( ! path || ! number ){
					oxr.LogWarnFormat( "Invalid entry in tracker database. Ignoring entry" );
					continue;
				}
				
				pTrackers.Add( Tracker::Ref::New( new Tracker( path, number ) ) );
			}
		}
		
	}catch( const deException &e ){
		oxr.LogException( e );
	}
}

void deoxrDPHtcViveTracker::pSaveTrackerDatabase(){
	deVROpenXR &oxr = GetInstance().GetOxr();
	oxr.LogInfoFormat( "Save VIVE Trackers..." );
	
	try{
		deVirtualFileSystem& vfs = oxr.GetVFS();
		
		const decBaseFileWriter::Ref fileWriter( vfs.OpenFileForWriting(
			decPath::CreatePathUnix( PATH_VIVETRACKER_XML ) ) );
		
		decXmlWriter writer( fileWriter );
		writer.WriteXMLDeclaration();
		
		writer.WriteOpeningTag( "viveTrackers" );
		
		const int count = pTrackers.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			const Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
			writer.WriteOpeningTag( "viveTracker" );
			writer.WriteDataTagString( "path", tracker.path.GetName() );
			writer.WriteDataTagString( "patRole", tracker.pathRole.GetName() );
			writer.WriteDataTagInt( "number", tracker.number );
			writer.WriteClosingTag( "viveTracker" );
		}
		
		writer.WriteClosingTag( "viveTrackers" );
		
	}catch( const deException &e ){
		oxr.LogException( e );
	}
}

void deoxrDPHtcViveTracker::pAddDevice( Tracker &tracker ){
	if( ! tracker.action /* || ! tracker.pathRole */ ){
		return;
	}
	
	deVROpenXR &oxr = GetInstance().GetOxr();
	tracker.device.TakeOver( new deoxrDevice( oxr, *this ) );
	
	decString id;
	id.Format( "%str_%s", OXR_DEVID_PREFIX, pSerialFromPath( tracker.path ).GetString() );
	
	tracker.device->SetType( deInputDevice::edtVRTracker );
	tracker.device->SetName( pNameForTracker( tracker ) );
	tracker.device->SetActionPose( tracker.action );
	tracker.device->SetID( id );
// 	tracker.device->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace(
// 		*pGetSession(), tracker.action, tracker.pathRole, decVector() ) ) );
	tracker.device->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace(
		*pGetSession(), tracker.action, tracker.path, decVector() ) ) );
// 	tracker.device->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace( *pGetSession(), tracker.action ) ) );
	
	deoxrDeviceComponent * const trigger = pAddComponentTrigger( tracker.device );
	pAddAxisTrigger( tracker.device, trigger );
	pAddButtonTrigger( tracker.device, trigger, false ); // has to be button 0
	
	pAddButton( tracker.device, ebaPrimary, eblHome, false ); // has to be button 1
	pAddButton( tracker.device, ebaSecondary, eblSystem, false ); // has to be button 2
	
	deoxrDeviceComponent * const grip = pAddComponentGrip( tracker.device );
	pAddButtonGrip( tracker.device, grip, false );
	
	deoxrDeviceComponent * const trackpad = pAddComponentTrackpad( tracker.device );
	pAddAxesTrackpad( tracker.device, trackpad );
	pAddButtonTrackpad( tracker.device, trackpad, true, true );
	
	GetInstance().GetOxr().GetDevices().Add( tracker.device );
}
