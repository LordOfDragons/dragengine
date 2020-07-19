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
#ifndef _DECLASSVIDEO_H_
#define _DECLASSVIDEO_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deVideo;
class deScriptingDragonScript;



/**
 * @brief Video Script Class.
 */
class deClassVideo : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassVideo( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	virtual ~deClassVideo();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the video or NULL if myself is NULL. */
	deVideo *GetVideo( dsRealObject *myself ) const;
	/** Pushes the video on the stack. Video can be NULL to push a null object. */
	void PushVideo( dsRunTime *rt, deVideo *video );
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsVid, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt, *clsRN, *clsPoint;
		
		dsClass *clsResNot;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetWidth );
	DEF_NATFUNC( nfGetHeight );
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfGetFrameCount );
	DEF_NATFUNC( nfGetFrameRate );
	DEF_NATFUNC( nfGetPlayTime );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
