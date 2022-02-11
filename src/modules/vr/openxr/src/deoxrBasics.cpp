/* 
 * Drag[en]gine OpenXR
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

#include "deoxrBasics.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// #ifdef OXR_CHECKCOMMANDS

void deoxrDebugCheckCommand( XrResult result, deVROpenXR &oxr, const char *file, int line ){
	#define CASE_ERROR(e) \
		case e: \
			/* oxr.LogException( deeInvalidAction( file, line, #e ) ); */ \
			throw deeInvalidAction( file, line, #e );
	
	switch( result ){
	case XR_SUCCESS:
		break;
		
	// these are what they call "qualified success". this should not cause a failure
	case XR_TIMEOUT_EXPIRED:
	case XR_SESSION_LOSS_PENDING:
	case XR_EVENT_UNAVAILABLE:
	case XR_SPACE_BOUNDS_UNAVAILABLE:
	case XR_SESSION_NOT_FOCUSED:
	case XR_FRAME_DISCARDED:
		break;
	
	CASE_ERROR( XR_ERROR_VALIDATION_FAILURE )
	CASE_ERROR( XR_ERROR_RUNTIME_FAILURE )
	CASE_ERROR( XR_ERROR_OUT_OF_MEMORY )
	CASE_ERROR( XR_ERROR_API_VERSION_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_INITIALIZATION_FAILED )
	CASE_ERROR( XR_ERROR_FUNCTION_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_FEATURE_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_EXTENSION_NOT_PRESENT )
	CASE_ERROR( XR_ERROR_LIMIT_REACHED )
	CASE_ERROR( XR_ERROR_SIZE_INSUFFICIENT )
	CASE_ERROR( XR_ERROR_HANDLE_INVALID )
	CASE_ERROR( XR_ERROR_INSTANCE_LOST )
	CASE_ERROR( XR_ERROR_SESSION_RUNNING )
	CASE_ERROR( XR_ERROR_SESSION_NOT_RUNNING )
	CASE_ERROR( XR_ERROR_SESSION_LOST )
	CASE_ERROR( XR_ERROR_SYSTEM_INVALID )
	CASE_ERROR( XR_ERROR_PATH_INVALID )
	CASE_ERROR( XR_ERROR_PATH_COUNT_EXCEEDED )
	CASE_ERROR( XR_ERROR_PATH_FORMAT_INVALID )
	CASE_ERROR( XR_ERROR_PATH_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_LAYER_INVALID )
	CASE_ERROR( XR_ERROR_LAYER_LIMIT_EXCEEDED )
	CASE_ERROR( XR_ERROR_SWAPCHAIN_RECT_INVALID )
	CASE_ERROR( XR_ERROR_SWAPCHAIN_FORMAT_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_ACTION_TYPE_MISMATCH )
	CASE_ERROR( XR_ERROR_SESSION_NOT_READY )
	CASE_ERROR( XR_ERROR_SESSION_NOT_STOPPING )
	CASE_ERROR( XR_ERROR_TIME_INVALID )
	CASE_ERROR( XR_ERROR_REFERENCE_SPACE_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_FILE_ACCESS_ERROR )
	CASE_ERROR( XR_ERROR_FILE_CONTENTS_INVALID )
	CASE_ERROR( XR_ERROR_FORM_FACTOR_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_FORM_FACTOR_UNAVAILABLE )
	CASE_ERROR( XR_ERROR_API_LAYER_NOT_PRESENT )
	CASE_ERROR( XR_ERROR_CALL_ORDER_INVALID )
	CASE_ERROR( XR_ERROR_GRAPHICS_DEVICE_INVALID )
	CASE_ERROR( XR_ERROR_POSE_INVALID )
	CASE_ERROR( XR_ERROR_INDEX_OUT_OF_RANGE )
	CASE_ERROR( XR_ERROR_VIEW_CONFIGURATION_TYPE_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_ENVIRONMENT_BLEND_MODE_UNSUPPORTED )
	CASE_ERROR( XR_ERROR_NAME_DUPLICATED )
	CASE_ERROR( XR_ERROR_NAME_INVALID )
	CASE_ERROR( XR_ERROR_ACTIONSET_NOT_ATTACHED )
	CASE_ERROR( XR_ERROR_ACTIONSETS_ALREADY_ATTACHED )
	CASE_ERROR( XR_ERROR_LOCALIZED_NAME_DUPLICATED )
	CASE_ERROR( XR_ERROR_LOCALIZED_NAME_INVALID )
	CASE_ERROR( XR_ERROR_GRAPHICS_REQUIREMENTS_CALL_MISSING )
	CASE_ERROR( XR_ERROR_RUNTIME_UNAVAILABLE )
	CASE_ERROR( XR_ERROR_ANDROID_THREAD_SETTINGS_ID_INVALID_KHR )
	CASE_ERROR( XR_ERROR_ANDROID_THREAD_SETTINGS_FAILURE_KHR )
	CASE_ERROR( XR_ERROR_CREATE_SPATIAL_ANCHOR_FAILED_MSFT )
	CASE_ERROR( XR_ERROR_SECONDARY_VIEW_CONFIGURATION_TYPE_NOT_ENABLED_MSFT )
	CASE_ERROR( XR_ERROR_CONTROLLER_MODEL_KEY_INVALID_MSFT )
	CASE_ERROR( XR_ERROR_REPROJECTION_MODE_UNSUPPORTED_MSFT )
	CASE_ERROR( XR_ERROR_COMPUTE_NEW_SCENE_NOT_COMPLETED_MSFT )
	CASE_ERROR( XR_ERROR_SCENE_COMPONENT_ID_INVALID_MSFT )
	CASE_ERROR( XR_ERROR_SCENE_COMPONENT_TYPE_MISMATCH_MSFT )
	CASE_ERROR( XR_ERROR_SCENE_MESH_BUFFER_ID_INVALID_MSFT )
	CASE_ERROR( XR_ERROR_SCENE_COMPUTE_FEATURE_INCOMPATIBLE_MSFT )
	CASE_ERROR( XR_ERROR_SCENE_COMPUTE_CONSISTENCY_MISMATCH_MSFT )
	CASE_ERROR( XR_ERROR_DISPLAY_REFRESH_RATE_UNSUPPORTED_FB )
	CASE_ERROR( XR_ERROR_COLOR_SPACE_UNSUPPORTED_FB )
	CASE_ERROR( XR_ERROR_UNEXPECTED_STATE_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_FEATURE_ALREADY_CREATED_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_FEATURE_REQUIRED_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_NOT_PERMITTED_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_INSUFFICIENT_RESOURCES_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_UNKNOWN_PASSTHROUGH_FB )
	CASE_ERROR( XR_ERROR_RENDER_MODEL_KEY_INVALID_FB )
	CASE_ERROR( XR_RENDER_MODEL_UNAVAILABLE_FB )
	CASE_ERROR( XR_ERROR_MARKER_NOT_TRACKED_VARJO )
	CASE_ERROR( XR_ERROR_MARKER_ID_INVALID_VARJO )
	CASE_ERROR( XR_ERROR_SPATIAL_ANCHOR_NAME_NOT_FOUND_MSFT )
	CASE_ERROR( XR_ERROR_SPATIAL_ANCHOR_NAME_INVALID_MSFT )
	
	default:{
		decString message;
		message.Format( "Error %x (%d)", result, result );
		//oxr.LogException( deeInvalidAction( file, line, message ) );
		throw deeInvalidAction( file, line, message );
		}
	}
	
	#undef CASE_ERROR
}

// #endif
