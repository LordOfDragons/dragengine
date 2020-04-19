/* 
 * Drag[en]gine DragonScript Script Module
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
	deClassVideoPlayer( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	virtual ~deClassVideoPlayer();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the video player or NULL if myself is NULL. */
	deVideoPlayer *GetVideoPlayer( dsRealObject *myself ) const;
	/** Pushes the video player on the stack. Video can be NULL to push a null object. */
	void PushVideoPlayer( dsRunTime *rt, deVideoPlayer *videoPlayer );
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsVP, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsVid, *clsObj;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetVideo );
	DEF_NATFUNC( nfSetVideo );
	
	DEF_NATFUNC( nfGetLooping );
	DEF_NATFUNC( nfSetLooping );
	DEF_NATFUNC( nfGetPlayFrom );
	DEF_NATFUNC( nfGetPlayTo );
	DEF_NATFUNC( nfSetPlayRange );
	DEF_NATFUNC( nfGetPlayPosition );
	DEF_NATFUNC( nfSetPlayPosition );
	DEF_NATFUNC( nfGetPlaySpeed );
	DEF_NATFUNC( nfSetPlaySpeed );
	
	DEF_NATFUNC( nfIsPlaying );
	DEF_NATFUNC( nfIsPaused );
	DEF_NATFUNC( nfIsStopped );
	DEF_NATFUNC( nfPlay );
	DEF_NATFUNC( nfPause );
	DEF_NATFUNC( nfStop );
	
	DEF_NATFUNC( nfUpdate );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
