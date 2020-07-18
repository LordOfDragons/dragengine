/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpDeveloperMode.h"
#include "../dePhysicsBullet.h"
#include "../debug/debpDebug.h"
#include "../world/debpWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/collider/deCollider.h>

#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
//#include "Extras/BulletColladaConverter/ColladaConverter.h"



// Class debpDeveloperMode
////////////////////////////

// Constructor, destructor
////////////////////////////

debpDeveloperMode::debpDeveloperMode( dePhysicsBullet &bullet ) :
pBullet( bullet ),
pEnabled( false ),
pTakeSnapshot( false ),
pHighlightResponseType( -1 )
{
	(void)pBullet; // for future use
}

debpDeveloperMode::~debpDeveloperMode(){
}



// Management
///////////////

bool debpDeveloperMode::ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.MatchesArgumentAt( 0, "dm_enable" ) ){
		pCmdEnable( command, answer );
		return true;
	}
	
	if( ! pEnabled ){
		return false;
	}
	
	if( command.MatchesArgumentAt( 0, "dm_help" ) ){
		pCmdHelp( command, answer );
		return true;
		
	}else if( command.MatchesArgumentAt( 0, "dm_take_snapshot" ) ){
		pCmdTakeSnapshot( command, answer );
		return true;
		
	}else if( command.MatchesArgumentAt( 0, "dm_show_category" ) ){
		pCmdShowCategory( command, answer );
		return true;
		
	}else if( command.MatchesArgumentAt( 0, "dm_highlight_response_type" ) ){
		pCmdHighlightResponseType( command, answer );
		return true;
		
	}else if( command.MatchesArgumentAt( 0, "dm_debug" ) ){
		pCmdDebugEnable( command, answer );
		return true;
	}
	
	return false;
}



void debpDeveloperMode::TakeSnapshot( debpWorld *world ){
	if( pTakeSnapshot ){
		if( ! world ) DETHROW( deeInvalidParam );
		
		// determine the file to save to. currently this is a hack in that we store
		// the file into the shared path of the module which is of course in a real
		// world scenario not writeable but in our test environment it is. later on
		// the engine is going to provide us a function to obtain a debug output
		// file which is guaranteed to be unique and in a place the user has easy
		// access to.
		decPath path;
		
		path.SetFromNative( "/var/snapshot.dae" );
		
		// take the snapshot.
		// NOTE: this is not working at all. too many dependencies on external files
		//       and a problematic layout of the source files. taking snapshots is
		//       therefore stripped out for good until a feasible solution is found.
		
		//ColladaConverter converter( world->GetDynamicsWorld() );
		//converter.save( path.GetPathNative() );
		
		// reset
		pTakeSnapshot = false;
	}
}



// Private functions
//////////////////////

void debpDeveloperMode::pCmdHelp( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_help => Displays this help screen.\n" );
	answer.AppendFromUTF8( "dm_take_snapshot => Snapshot next world into a COLLADA file.\n" );
	answer.AppendFromUTF8( "dm_show_category => Show collision objects with collision category (comma-separated list of bit-numbers or 'off').\n" );
	answer.AppendFromUTF8( "dm_highlight_response_type => Highlight response type if dm_show_category is used.\n" );
	answer.AppendFromUTF8( "dm_debug {enable | disable} => Enable performance debugging.\n" );
}

void debpDeveloperMode::pCmdEnable( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pEnabled = true;
	answer.AppendFromUTF8( "Developer Mode is enabled" );
}

void debpDeveloperMode::pCmdTakeSnapshot( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pTakeSnapshot = true;
}

void debpDeveloperMode::pCmdShowCategory( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const decString newValue( command.GetArgumentAt( 1 )->ToUTF8() );
		
		pShowCategory.ClearMask();
		
		if( ! newValue.EqualsInsensitive( "off" ) ){
			const decStringList bits( newValue.Split( ',' ) );
			const int count = bits.GetCount();
			int i;
			for( i=0; i<count; i++ ){
				const int bit = bits.GetAt( i ).ToInt();
				if( bit >= 0 && bit < 64 ){
					pShowCategory.SetBit( bit );
				}
			}
		}
	}
	
	decString bitString;
	decString text;
	
	int i;
	for( i=0; i<63; i++ ){
		if( pShowCategory.IsBitSet( i ) ){
			if( ! bitString.IsEmpty() ){
				bitString.AppendCharacter( ',' );
			}
			bitString.AppendValue( i );
		}
	}
	if( bitString.IsEmpty() ){
		bitString = "off";
	}
	
	text.Format( "dm_show_category = %s\n", bitString.GetString() );
	answer.AppendFromUTF8( text );
}

void debpDeveloperMode::pCmdHighlightResponseType( const decUnicodeArgumentList &command,
decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		const decString newValue( command.GetArgumentAt( 1 )->ToUTF8() );
		
		if( newValue == "static" || newValue == "s" ){
			pHighlightResponseType = deCollider::ertStatic;
			
		}else if( newValue == "kinematic" || newValue == "k" ){
			pHighlightResponseType = deCollider::ertKinematic;
			
		}else if( newValue == "dynamic" || newValue == "d" ){
			pHighlightResponseType = deCollider::ertDynamic;
			
		}else if( newValue == "off" ){
			pHighlightResponseType = -1;
			
		}else{
			answer.AppendFromUTF8( "dm_highlight_response_type {off | (s)tatic | (k)inematic | (d)ynamic}" );
			return;
		}
	}
	
	switch( pHighlightResponseType ){
	case deCollider::ertStatic:
		answer.AppendFromUTF8( "dm_highlight_response_type = static" );
		break;
		
	case deCollider::ertKinematic:
		answer.AppendFromUTF8( "dm_highlight_response_type = kinematic" );
		break;
		
	case deCollider::ertDynamic:
		answer.AppendFromUTF8( "dm_highlight_response_type = dynamic" );
		break;
		
	default:
		answer.AppendFromUTF8( "dm_highlight_response_type = off" );
		break;
	}
}

void debpDeveloperMode::pCmdDebugEnable( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pBullet.GetDebug().SetEnabled( command.GetArgumentAt( 1 )->ToUTF8() == "enable" );
	}
	
	decString text;
	text.Format( "dm_debug = %s\n", pBullet.GetDebug().GetEnabled() ? "enabled" : "disabled" );
	answer.AppendFromUTF8( text );
}
