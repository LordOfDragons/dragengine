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

#ifndef _CEGESTURE_H_
#define _CEGESTURE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class ceConversation;


/*
NOTES

conversation has a default conversation. gestures are taken first from the default
conversation than from the actual conversation overwriting gestures with the same
name. for this an additional gesture list is stored in the conversation which
stores the final gesture list. actors use this gesture list to produce an animator
for gesture animations. not existing animation are not used which is no problem
with the controllers as the controller value is from 0 to the number of final
gestures. the actual animations can be fewer having only as many links as required.

=========
smalltalk in default conversation with variation depending on actor types.
=========
special npc-npc conversations directly stored in the receiving npc conversation.
=========

for camera shots store:
- start and end position (linear movement)
- start and end distance (distance of camera to position)
- start and end orientation (rotation around position using distance)
- start and end field-of-view (in degrees)
camera shots work like gestures in that they have a length and the camera shot
is scaled to match this length.

=========
in actor speak add another list containing <int,float> entries representing the actor
the current actor is looking at. this defines the up-down and left-right look values
for the actor animator. relative gestures like mustering the actor is done using
gestures ontop of this. problem left is how to get the two together as left-right
and up-down looking has to come after the gesture which is an own animator. best
way would be to force gesture animators to have up-down and left-right look controllers
as the actor animator has providing a reasonable looking around. this overwrites the
actor animator looking and should work.

=========

PlayerAction script class has an identifier (UniqueString) which is used to identify
what kind of PlayerAction is currently in use. This identifier can be used by the
conversation to determine what gestures to use as well as conditional playback of
certain conversation parts.

Every gesture refers to an animator. These animators are tailored for a specific
PlayerAction (required condition for selecting a gesture). They have the same
controller layout as the animator used in the PlayerAction of interest. While a
conversation is played back the PlayerAction applies the same processing to the
active gesture animator as to the PlayerAction animator. This means that playback
controllers are advanced by the elapsed time multiplied by a scaling factor to
match the playback speed to the gesture duration. All other controllers are set
to the same values. In addition the gesture animation has one additional controller
which is used to specify a random variation for the gesture. This value is set once
and not changed during the playback of this gesture. The variation value is provided
by the conversation playback object hence PlayerAction objects can simply set this
value during every frame update without having to worry about anything.
*/


/**
 * Gesture.
 */
class ceGesture : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceGesture> Ref;
	
	
	
private:
	ceConversation *pConversation;
	
	decString pName;
	decString pAnimator;
	bool pHold;
	float pDuration;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create gesture. */
	ceGesture( const char *name = "Gesture" );
	
	/** Clean up gesture. */
	virtual ~ceGesture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Conversation or \em NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** Set conversation or \em NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Name of animator to use. */
	inline const decString &GetAnimator() const{ return pAnimator; }
	
	/** Set name of animator to use. */
	void SetAnimator( const char *animator );
	
	/** Hold gesture after finishing it. */
	inline bool GetHold() const{ return pHold; }
	
	/** Set if gesture is hold after finishing it. */
	void SetHold( bool hold );
	
	/** Duration in seconds. */
	inline float GetDuration() const{ return pDuration; }
	
	/** Set duration in seconds. */
	void SetDuration( float duration );
	/*@}*/
};

#endif
