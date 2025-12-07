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

// include only once
#ifndef _DECLASSVIDEOPLAYER_H_
#define _DECLASSVIDEOPLAYER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deVideoPlayer;
class deScriptingDragonScript;



/**
 * @brief Video Player Script Class.
 */
class deClassVideoPlayer : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassVideoPlayer(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	virtual ~deClassVideoPlayer();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers(dsEngine *engine);
	/** Retrieves the video player or NULL if myself is NULL. */
	deVideoPlayer *GetVideoPlayer(dsRealObject *myself) const;
	/** Pushes the video player on the stack. Video can be NULL to push a null object. */
	void PushVideoPlayer(dsRunTime *rt, deVideoPlayer *videoPlayer);
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsVP, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsVid, *clsObj;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetVideo);
	DEF_NATFUNC(nfSetVideo);
	
	DEF_NATFUNC(nfGetLooping);
	DEF_NATFUNC(nfSetLooping);
	DEF_NATFUNC(nfGetPlayFrom);
	DEF_NATFUNC(nfGetPlayTo);
	DEF_NATFUNC(nfSetPlayRange);
	DEF_NATFUNC(nfGetPlayPosition);
	DEF_NATFUNC(nfSetPlayPosition);
	DEF_NATFUNC(nfGetPlaySpeed);
	DEF_NATFUNC(nfSetPlaySpeed);
	
	DEF_NATFUNC(nfIsPlaying);
	DEF_NATFUNC(nfIsPaused);
	DEF_NATFUNC(nfIsStopped);
	DEF_NATFUNC(nfPlay);
	DEF_NATFUNC(nfPause);
	DEF_NATFUNC(nfStop);
	
	DEF_NATFUNC(nfUpdate);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
