/* 
 * Drag[en]gine AI Module
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

#include "dedaiDeveloperMode.h"
#include "../deDEAIModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/exceptions.h>



// Class dedaiDeveloperMode
/////////////////////////////

// Constructor, destructor
////////////////////////////

dedaiDeveloperMode::dedaiDeveloperMode( deDEAIModule &deai ) :
pDEAI( deai ),

pEnabled( false ),

pShowSpaces( false ),
pShowSpaceLinks( false ),
pSpaceHilightCostType( -1 ),
pShowBlockers( false ),
pShowPath( false ),
pShowPathFaces( false ),

pQuickDebug( 0 ),

pUpdateTracker( 0 ){
	// debug
// 	pEnabled = true;
// 	pShowNavSpaces = true;
// 	pNavSpaceHilightCostType = 0;
// 	pShowNavPathFaces = true;
}

dedaiDeveloperMode::~dedaiDeveloperMode(){
}



// Management
///////////////

bool dedaiDeveloperMode::ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.MatchesArgumentAt( 0, "dm_enable" ) ){
		pCmdEnable( command, answer );
		return true;
		
	}else if( pEnabled ){
		if( command.MatchesArgumentAt( 0, "dm_help" ) ){
			pCmdHelp( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_spaces" ) ){
			pCmdShowSpaces( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_space_links" ) ){
			pCmdShowSpaceLinks( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_space_hilight_cost_type" ) ){
			pCmdSpaceHilightCostType( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_blockers" ) ){
			pCmdShowBlockers( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_path" ) ){
			pCmdShowPath( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_show_path_faces" ) ){
			pCmdShowPathFaces( command, answer );
			return true;
			
		}else if( command.MatchesArgumentAt( 0, "dm_quick_debug" ) ){
			pCmdQuickDebug( command, answer );
			return true;
		}
	}
	
	return false;
}



void dedaiDeveloperMode::TouchUpdateTracker(){
	pUpdateTracker++;
}



// Private functions
//////////////////////

void dedaiDeveloperMode::pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_help => Displays this help screen.\n" );
	answer.AppendFromUTF8( "dm_show_spaces [1|0] => Displays navigation spaces.\n" );
	answer.AppendFromUTF8( "dm_show_space_links [1|0] => Displays navigation space links.\n" );
	answer.AppendFromUTF8( "dm_space_hilight_cost_type [costType] => Hilight navigation space element of a certain cost type.\n" );
	answer.AppendFromUTF8( "dm_show_blockers [1|0] => Displays navigation blockers.\n" );
	answer.AppendFromUTF8( "dm_show_path [1|0] => Dispaly navigator path.\n" );
	answer.AppendFromUTF8( "dm_show_path_faces [1|0] => Dispaly navigator path faces.\n" );
	answer.AppendFromUTF8( "dm_quick_debug [number] => Quick debug.\n" );
}

void dedaiDeveloperMode::pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	answer.SetFromUTF8( "Developer mode enabled.\n" );
	if( ! pEnabled ){
		TouchUpdateTracker();
	}
	pEnabled = true;
}



void dedaiDeveloperMode::pCmdShowSpaces( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const bool newValue = ! command.MatchesArgumentAt( 1, "0" );
		if( newValue != pShowSpaces ){
			TouchUpdateTracker();
		}
		pShowSpaces = newValue;
	}
	
	decString text;
	text.Format( "dm_show_spaces = %i\n", pShowSpaces ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void dedaiDeveloperMode::pCmdShowSpaceLinks( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const bool newValue = ! command.MatchesArgumentAt( 1, "0" );
		if( newValue != pShowSpaceLinks ){
			TouchUpdateTracker();
		}
		pShowSpaceLinks = newValue;
	}
	
	decString text;
	text.Format( "dm_show_space_links = %i\n", pShowSpaceLinks ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void dedaiDeveloperMode::pCmdSpaceHilightCostType( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const int newValue = command.GetArgumentAt( 1 )->ToInt();
		if( newValue != pSpaceHilightCostType ){
			TouchUpdateTracker();
		}
		pSpaceHilightCostType = newValue;
	}
	
	decString text;
	text.Format( "dm_space_hilight_cost_type = %i\n", pSpaceHilightCostType );
	answer.AppendFromUTF8( text );
}

void dedaiDeveloperMode::pCmdShowBlockers( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const bool newValue = ! command.MatchesArgumentAt( 1, "0" );
		if( newValue != pShowBlockers ){
			TouchUpdateTracker();
		}
		pShowBlockers = newValue;
	}
	
	decString text;
	text.Format( "dm_show_blockers = %i\n", pShowBlockers ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void dedaiDeveloperMode::pCmdShowPath( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const bool newValue = ! command.MatchesArgumentAt( 1, "0" );
		if( newValue != pShowPath ){
			TouchUpdateTracker();
		}
		pShowPath = newValue;
	}
	
	decString text;
	text.Format( "dm_show_path = %i\n", pShowPath ? 1 : 0 );
	answer.AppendFromUTF8( text );
}

void dedaiDeveloperMode::pCmdShowPathFaces( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const bool newValue = ! command.MatchesArgumentAt( 1, "0" );
		if( newValue != pShowPathFaces ){
			TouchUpdateTracker();
		}
		pShowPathFaces = newValue;
	}
	
	decString text;
	text.Format( "dm_show_path_faces = %i\n", pShowPathFaces ? 1 : 0 );
	answer.AppendFromUTF8( text );
}



void dedaiDeveloperMode::pCmdQuickDebug( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const int newValue = command.GetArgumentAt( 1 )->ToInt();
		if( newValue != pQuickDebug ){
			TouchUpdateTracker();
		}
		pQuickDebug = newValue;
	}
	
	decString text;
	text.Format( "dm_quick_debug = %i\n", pQuickDebug );
	answer.AppendFromUTF8( text );
}
