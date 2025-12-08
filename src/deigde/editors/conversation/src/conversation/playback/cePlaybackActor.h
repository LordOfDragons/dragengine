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
	ceTextBoxText::Ref pTextBoxText;
	
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
	void SetSpeechLength(float length);
	/** Retrieves the elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	/** Sets the elapsed time. */
	void SetElapsedTime(float elapsed);
	/** Sets the text box. Used to handle text box text. */
	void SetTextBox(ceTextBox *textBox);
	/** Text box text or NULL if not owning one. */
	inline ceTextBoxText *GetTextBoxText() const{ return pTextBoxText; }
	/** Sets the text box text or NULL if not owning one. */
	void SetTextBoxText(ceTextBoxText *text);
	
	/** Determines if the speech is done. */
	bool IsSpeechDone() const;
	
	/** Reset. */
	void Reset();
	/** Update. */
	void Update(float elapsed);
	/*@}*/
};

#endif
