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

#ifndef _CESTRIP_H_
#define _CESTRIP_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Conversation action strip.
 */
class ceStrip : public deObject{
private:
	decString pID;
	float pPause;
	float pDuration;
	
public:
	typedef deTObjectReference<ceStrip> Ref;
	typedef decTObjectOrderedSet<ceStrip> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new empty strip. */
	ceStrip();
	/** \brief Creates a new strip. */
	ceStrip(const char *id, float duration, float pause);
	/** \brief Creates a new strip. */
	ceStrip(const ceStrip &strip);
	/** \brief Cleans up the strip. */
protected:
	virtual ~ceStrip();
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the identifier. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the identifier. */
	void SetID(const char *id);
	/** \brief Retrieves the pause in seconds. */
	inline float GetPause() const{ return pPause; }
	/** \brief Sets the pause in seconds. */
	void SetPause(float pause);
	/** \brief Retrieves the duration in seconds. */
	inline float GetDuration() const{ return pDuration; }
	/** \brief Sets the duration in seconds. */
	void SetDuration(float duration);
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Set strip from another strip. */
	ceStrip &operator=(const ceStrip &strip);
	/** \brief Determines if two strips are equal. */
	bool operator==(const ceStrip &strip) const;
	/** \brief Determines if two strips are not equal. */
	bool operator!=(const ceStrip &strip) const;
	/*@}*/
};

#endif
