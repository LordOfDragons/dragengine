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

#ifndef _CEPLAYBACKACTOR_H_
#define _CEPLAYBACKACTOR_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class ceTextBoxText;
class ceTextBox;



/**
 * \brief Playback Actor Class.
 * Handles the state of an actor during playback. Playback actors are independent of
 * conversation actors as they hold only the state no matter what kind of actor in
 * the end has to be used.
 */
class cePlaybackActor{
private:
	float pSpeechLength;
	float pElapsedTime;
	ceTextBox *pTextBox;
	ceTextBoxText *pTextBoxText;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new playback actor. */
	cePlaybackActor();
	/** Cleans up the playback actor. */
	~cePlaybackActor();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the length of the speech. */
	inline float GetSpeechLength() const{ return pSpeechLength; }
	/** Sets the speech length. */
	void SetSpeechLength( float length );
	/** Retrieves the elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	/** Sets the elapsed time. */
	void SetElapsedTime( float elapsed );
	/** Sets the text box. Used to handle text box text. */
	void SetTextBox( ceTextBox *textBox );
	/** Text box text or NULL if not owning one. */
	inline ceTextBoxText *GetTextBoxText() const{ return pTextBoxText; }
	/** Sets the text box text or NULL if not owning one. */
	void SetTextBoxText( ceTextBoxText *text );
	
	/** Determines if the speech is done. */
	bool IsSpeechDone() const;
	
	/** Reset. */
	void Reset();
	/** Update. */
	void Update( float elapsed );
	/*@}*/
};

#endif
