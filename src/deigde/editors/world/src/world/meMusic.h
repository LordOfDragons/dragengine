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

#ifndef _MEMUSIC_H_
#define _MEMUSIC_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sound/deSpeaker.h>

class meWorld;


/**
 * Play music to evaluate music/sound volume balancing.
 */
class meMusic : public deObject{
public:
	typedef deTObjectReference<meMusic> Ref;
	
	
private:
	meWorld &pWorld;
	decString pPath;
	deSpeaker::Ref pSpeaker;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates music. */
	meMusic(meWorld &world);
	
protected:
	/** Clean up music. */
	~meMusic() override;
	
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent world. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** Music path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** Set music path. */
	void SetPath(const char *path);
	
	/** Volume. */
	float GetVolume() const;
	
	/** Set volume. */
	void SetVolume(float volume);
	
	/** Play. */
	void Play();
	
	/** Pause. */
	void Pause();
	
	/** Stop. */
	void Stop();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
