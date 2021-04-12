/* 
 * Drag[en]gine Animator Module
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
#include <string.h>
#include <unistd.h>

#include "dearAnimation.h"
#include "dearAnimationMove.h"
#include "../deDEAnimator.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Class dearAnimation
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearAnimation::dearAnimation( deDEAnimator *module, deAnimation *animation ){
	if( ! module || ! animation ){
		DETHROW( deeInvalidParam );
	}
	
	pModule = module;
	
	pAnimation = animation;
	
	pCreateMoves();
}

dearAnimation::~dearAnimation(){
}



// Management
///////////////

// #define DO_TIMING

#ifdef DO_TIMING
	#include <dragengine/common/utils/decTimer.h>
	static decTimer timerTotal;
	static decTimer timer;
	
	#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
	#define DEBUG_PRINT_TIMER(what)			pModule->LogInfoFormat( "Animation Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) ); timer.Reset()
	#define DEBUG_PRINT_TIMER_TOTAL(what)	pModule->LogInfoFormat( "Animation Timer-Total: %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0 ) )
#else
	#define DEBUG_RESET_TIMERS
	#define DEBUG_PRINT_TIMER(what)
	#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif



int dearAnimation::GetMoveCount() const{
	return pMoves.GetCount();
}

dearAnimationMove *dearAnimation::GetMoveAt( int index ) const{
	return ( dearAnimationMove* )pMoves.GetAt( index );
}

dearAnimationMove *dearAnimation::GetMoveNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pMoves.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		dearAnimationMove * const move = ( dearAnimationMove* )pMoves.GetAt( i );
		if( move->GetName() == name ){
			return move;
		}
	}
	
	return NULL;
}



// Private functions
//////////////////////

void dearAnimation::pCreateMoves(){
	const int count = pAnimation->GetMoveCount();
	if( count == 0 ){
		return;
	}
	
	pMoves.RemoveAll();
	
	deObjectReference move;
	int i;
	for( i=0; i<count; i++ ){
		move.TakeOver( new dearAnimationMove( *pAnimation->GetMove( i ) ) );
		pMoves.Add( move );
	}
}
