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
	"HTC VIVE Tracker" )
{
	pLoadTrackerDatabase();
}

deoxrDPHtcViveTracker::~deoxrDPHtcViveTracker(){
}



// Management
///////////////

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
				instance.GetOxr().LogInfoFormat( "VIVE Tracker %d: path='%s' rolePath='%s'",
					i, path.GetName().GetString(), pathRole.GetName().GetString() );
			}
			
			// remove devices of no more connected trackers
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
				
				if( i == count ){
					devices.Remove( tracker.device );
					tracker.device = nullptr;
				}
			}
			
			// add devices for newly connected trackers
			for( i=0; i<count; i++ ){
				Tracker * const tracker = pGetTrackerWith( trackerPaths[ i ].persistentPath );
				if( tracker ){
					if( ! tracker->device ){
						pAddDevice( *tracker );
					}
					
				}else{
					// we have never seen this tracker before. we have to add the tracker and
					// save the database to file. then we have to restart the VR system to
					// add the new action for the tracker
					pTrackers.Add( Tracker::Ref::New( new Tracker(
						deoxrPath( instance, trackerPaths[ i ].persistentPath ),
						pTrackers.GetCount() + 1 ) ) );
					pSaveTrackerDatabase();
					// instance.GetOxr().RequiresRestart();
					
					/* temp */ pAddDevice(*pGetTrackerWith(trackerPaths[i].persistentPath));
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
	decString name, localizedName;
	int i;
	
	for( i=0; i<count; i++ ){
		Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
		
		name.Format( "pose_tracker_%d", tracker.number );
		localizedName.Format( "Tracker %d", tracker.number );
		
		const XrPath subactionPath[ 1 ] = { tracker.path };
		tracker.action = actionSet.AddAction( deoxrAction::etInputPose,
			name, localizedName, subactionPath, 1 );
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
	
	// WARNING if an action is assigned SteamVR starts spamming connection events at high
	//         frequency alternating between connect/remove tracker. this causes devices
	//         to be added/removed many times per frame update causing slow-down and other
	//         problems. there is no remedy against this since steam reports to us
	//         connect/disconnect in short succession so we can not protect against this bug
	
#if 0
	const deoxrInstance &instance = GetInstance();
	const int bindingCount = 1/*10*/ * count;
	deoxrInstance::sSuggestBinding bindings[ bindingCount ];
	deoxrInstance::sSuggestBinding *b = bindings;
	decString name;
	
	int i;
	for( i=0; i<pTrackers.GetCount(); i++ ){
		const Tracker &tracker = *( ( Tracker* )pTrackers.GetAt( i ) );
		//const decString basePath( tracker.path.GetName() );
		const decString basePath( "/user/vive_tracker_htcx/role/handheld_object" );
		
		( b++ )->Set( tracker.action, deoxrPath( instance, basePath + "/input/grip/pose" ) );
		
		/*
		pAdd( b, deVROpenXR::eiaGripPress, basePath + "/input/squeeze/click" );
		
		pAdd( b, deVROpenXR::eiaTriggerPress, basePath + "/input/trigger/click" );
		pAdd( b, deVROpenXR::eiaTriggerAnalog, basePath + "/input/trigger/value" );
		
		pAdd( b, deVROpenXR::eiaButtonPrimaryPress, basePath + "/input/menu/click" );
		pAdd( b, deVROpenXR::eiaButtonSecondaryPress, basePath + "/input/system/click" );
		
		pAdd( b, deVROpenXR::eiaTrackpadAnalog, basePath + "/input/trackpad" );
		pAdd( b, deVROpenXR::eiaTrackpadPress, basePath + "/input/trackpad/click" );
		pAdd( b, deVROpenXR::eiaTrackpadTouch, basePath + "/input/trackpad/touch" );
		
		pAdd( b, deVROpenXR::eiaGripHaptic, basePath + "/output/haptic" );
		*/
	}
	
	GetInstance().SuggestBindings( GetPath(), bindings, bindingCount );
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
					DETHROW( deeInvalidFileFormat );
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
			writer.WriteDataTagInt( "number", tracker.number );
			writer.WriteClosingTag( "viveTracker" );
		}
		
		writer.WriteClosingTag( "viveTrackers" );
		
	}catch( const deException &e ){
		oxr.LogException( e );
	}
}

void deoxrDPHtcViveTracker::pAddDevice( Tracker &tracker ){
	deVROpenXR &oxr = GetInstance().GetOxr();
	tracker.device.TakeOver( new deoxrDevice( oxr, *this ) );
	
	decString id, name;
	name.Format( "Tracker %d", tracker.number );
	id.Format( "%str_%s", OXR_DEVID_PREFIX, pSerialFromPath( tracker.path ).GetString() );
	
	tracker.device->SetType( deInputDevice::edtVRTracker );
	tracker.device->SetName( name );
	tracker.device->SetActionPose( tracker.action );
	tracker.device->SetID( id );
	tracker.device->SetSpacePose( deoxrSpace::Ref::New( new deoxrSpace(
		*pGetSession(), tracker.action, tracker.path, decVector() ) ) );
	
	/*
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
	*/
	
	GetInstance().GetOxr().GetDevices().Add( tracker.device );
}
