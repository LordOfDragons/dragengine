/* 
 * Drag[en]gine OpenAL Audio Module
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

#include <stdlib.h>

#include "deoalDevMode.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../configuration/deoalConfiguration.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalMicrophone.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Class deoalDevMode
///////////////////////

// Constructor, destructor
////////////////////////////

deoalDevMode::deoalDevMode( deAudioOpenAL &oal ) :
pOal( oal ),

pEnabled( false ),

pLogCalcEnvProbe( false ),
pShowModuleInfo( false ),
pShowSpeakerEnvInfo( false ),
pShowSpeakerEnvInfoAt( false ),
pShowSpeakerEnvInfoClosest( 0 ),
pShowActiveMicInfo( false ),
pCaptureMicRays( false ),
pCaptureMicRaysXRay( false ),
pShowAudioModels( false ),
pCaptureSpeakerDirectClosest( 0 ),
pVisualizeAudibleSpeakers( 0 ){
}

deoalDevMode::~deoalDevMode(){
}



// Management
///////////////

bool deoalDevMode::ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.MatchesArgumentAt( 0, "dm_enable" ) ){
		pCmdEnable( command, answer );
		return true;
	}
	if( ! pEnabled ){
		return false;
	}
	
	try{
		if( command.MatchesArgumentAt( 0, "dm_help" ) ){
			pCmdHelp( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_log_calc_envprobe" ) ){
			pCmdLogCalcEnvProbe( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_module_info" ) ){
			pCmdShowModuleInfo( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_speaker_env_info" ) ){
			pCmdShowSpeakerEnvInfo( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_speaker_env_info_at" ) ){
			pCmdShowSpeakerEnvInfoAt( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_speaker_env_info_closest" ) ){
			pCmdShowSpeakerEnvInfoClosest( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_active_mic_info" ) ){
			pCmdShowActiveMicInfo( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_capture_mic_rays" ) ){
			pCmdCaptureMicRays( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_audio_models" ) ){
			pCmdShowAudioModels( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_capture_speaker_direct_closest" ) ){
			pCmdCaptureSpeakerDirectClosest( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_visualize_audible_speakers" ) ){
			pCmdVisualizeAudibleSpeakers( command, answer );
			return true;
		}
		
	}catch( const deException &exception ){
		answer.AppendFromUTF8( exception.FormatOutput().Join( "\n" ) );
	}
	return false;
}

void deoalDevMode::ClearCaptureMicRays(){
	pCaptureMicRays = false;
}

void deoalDevMode::ClearCaptureSpeakerClosestDirect(){
	pCaptureSpeakerDirectClosest = 0;
}



// Private functions
//////////////////////

void deoalDevMode::pCmdHelp( const decUnicodeArgumentList &, decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_help => Displays this help screen.\n" );
	answer.AppendFromUTF8( "dm_log_calc_envprobe [0|1] => Log environment probe calculations.\n" );
	answer.AppendFromUTF8( "dm_show_module_info [0|1] => Show module information.\n" );
	answer.AppendFromUTF8( "dm_show_speaker_env_info [0|1] => Show speaker environment overlay information.\n" );
	answer.AppendFromUTF8( "dm_show_speaker_env_info_at {off | x y z} => "
		"Show only speaker environment overlay information at position.\n" );
	answer.AppendFromUTF8( "dm_show_speaker_env_info_closest <count> => "
		"Show closest <count> speaker environment overlay information.\n" );
	answer.AppendFromUTF8( "dm_show_active_mic_info [0|1] => Show active microphone overlay information.\n" );
	answer.AppendFromUTF8( "dm_capture_mic_rays [xray] => Capture microphone sound rays once (visualize).\n" );
	answer.AppendFromUTF8( "dm_show_audio_models [0|1] => Show audio models.\n" );
	answer.AppendFromUTF8( "dm_capture_speaker_direct_closest <number> => Capture speaker direct closest.\n" );
}

void deoalDevMode::pCmdEnable( const decUnicodeArgumentList &, decUnicodeString &answer ){
	answer.SetFromUTF8( "Developer mode is now enabled.\n" );
	pEnabled = true;
}



void deoalDevMode::pCmdLogCalcEnvProbe( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pLogCalcEnvProbe = command.GetArgumentAt( 1 )->ToInt() == 1;
	}
	
	decString text;
	text.Format( "dm_log_calc_envprobe = %d\n", pLogCalcEnvProbe ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdShowModuleInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowModuleInfo = command.GetArgumentAt( 1 )->ToInt() == 1;
	}
	
	decString text;
	text.Format( "dm_show_module_info = %d\n", pShowModuleInfo ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdShowSpeakerEnvInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowSpeakerEnvInfo = command.GetArgumentAt( 1 )->ToInt() == 1;
	}
	
	decString text;
	text.Format( "dm_show_speaker_env_info = %d\n", pShowSpeakerEnvInfo ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdShowSpeakerEnvInfoAt( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() > 1 ){
		if( command.GetArgumentAt( 1 )->ToUTF8() == "off" ){
			pShowSpeakerEnvInfoAt = false;
			
		}else{
			pShowSpeakerEnvInfoAtPosition.x = command.GetArgumentAt( 1 )->ToDouble();
			pShowSpeakerEnvInfoAtPosition.y = command.GetArgumentAt( 2 )->ToDouble();
			pShowSpeakerEnvInfoAtPosition.z = command.GetArgumentAt( 3 )->ToDouble();
			pShowSpeakerEnvInfoAt = true;
		}
	}
	
	decString text;
	
	if( pShowSpeakerEnvInfoAt ){
		text.Format( "dm_show_speaker_env_info_at = (%g,%g,%g)\n", pShowSpeakerEnvInfoAtPosition.x,
			pShowSpeakerEnvInfoAtPosition.y, pShowSpeakerEnvInfoAtPosition.z );
		
	}else{
		text = "dm_show_speaker_env_info_at = off\n";
	}
	
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdShowSpeakerEnvInfoClosest(
const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() > 1 ){
		pShowSpeakerEnvInfoClosest = decMath::max( command.GetArgumentAt( 1 )->ToUTF8().ToInt(), 0 );
	}
	
	decString text;
	text.Format( "dm_show_speaker_env_info_closest = %d\n", pShowSpeakerEnvInfoClosest );
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdShowActiveMicInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowActiveMicInfo = command.GetArgumentAt( 1 )->ToInt() == 1;
	}
	
	decString text;
	text.Format( "dm_show_active_mic_info = %d\n", pShowActiveMicInfo ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void deoalDevMode::pCmdCaptureMicRays( const decUnicodeArgumentList &command, decUnicodeString & ){
	pCaptureMicRays = true;
	pCaptureMicRaysXRay = false;
	pCaptureMicRaysVolume = false;
	
	const int count = command.GetArgumentCount();
	int i;
	for( i=1; i<count; i++ ){
		const decString arg( command.GetArgumentAt( i )->ToUTF8() );
		if( arg == "xray" ){
			pCaptureMicRaysXRay = true;
			
		}else if( arg == "volume" ){
			pCaptureMicRaysVolume = true;
		}
	}
}

void deoalDevMode::pCmdShowAudioModels( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const bool oldValue = pShowAudioModels;
	if( command.GetArgumentCount() == 2 ){
		pShowAudioModels = command.GetArgumentAt( 1 )->ToInt() == 1;
	}
	
	decString text;
	text.Format( "dm_show_audio_models = %d\n", pShowAudioModels ? 1 : 0 );
	answer.AppendFromUTF8( text );
	
	if( pShowAudioModels != oldValue ){
		pActiveWorldNotifyDevModeChanged();
	}
}

void deoalDevMode::pCmdCaptureSpeakerDirectClosest( const decUnicodeArgumentList &command, decUnicodeString & ){
	if( command.GetArgumentCount() == 2 ){
		pCaptureSpeakerDirectClosest = command.GetArgumentAt( 1 )->ToInt();
	}
}

void deoalDevMode::pCmdVisualizeAudibleSpeakers( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const int oldValue = pVisualizeAudibleSpeakers;
	if( command.GetArgumentCount() == 2 ){
		pVisualizeAudibleSpeakers = command.GetArgumentAt( 1 )->ToInt();
	}
	
	decString text;
	text.Format( "dm_visualize_audible_speakers = %d\n", pVisualizeAudibleSpeakers );
	answer.AppendFromUTF8( text );
	
	if( pVisualizeAudibleSpeakers != oldValue ){
		pActiveWorldNotifyDevModeChanged();
	}
}



void deoalDevMode::pActiveWorldNotifyDevModeChanged(){
	if( pOal.GetActiveMicrophone() && pOal.GetActiveMicrophone()->GetParentWorld() ){
		pOal.GetActiveMicrophone()->GetParentWorld()->DevModeChanged();
	}
}
