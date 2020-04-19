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

#ifndef _DECLASSFILEREADER_H_
#define _DECLASSFILEREADER_H_

#include <libdscript/libdscript.h>

class decBaseFileReader;
class deScriptingDragonScript;



/**
 * @brief File Reader Script Class.
 */
class deClassFileReader : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassFileReader( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	virtual ~deClassFileReader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the fileReader or NULL if myself is NULL. */
	decBaseFileReader *GetFileReader( dsRealObject *myself ) const;
	/** Retrieves the stream version. */
	int GetStreamVersion( dsRealObject *myself ) const;
	/** Pushes the file reader on the stack. File reader can be NULL to push a null object. */
	void PushFileReader( dsRunTime *rt, decBaseFileReader *fileReader );
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsFRead, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfMovePosition );
	DEF_NATFUNC( nfSetPositionEnd );
	
	DEF_NATFUNC( nfGetStreamVersion );
	DEF_NATFUNC( nfSetStreamVersion );
	
	DEF_NATFUNC( nfEOF );
	DEF_NATFUNC( nfReadChar );
	DEF_NATFUNC( nfReadByte );
	DEF_NATFUNC( nfReadShort );
	DEF_NATFUNC( nfReadUShort );
	DEF_NATFUNC( nfReadInt );
	DEF_NATFUNC( nfReadUInt );
	DEF_NATFUNC( nfReadFloat );
	DEF_NATFUNC( nfReadString8 );
	DEF_NATFUNC( nfReadString16 );
	DEF_NATFUNC( nfReadString );
	DEF_NATFUNC( nfSkipString8 );
	DEF_NATFUNC( nfSkipString16 );
#undef DEF_NATFUNC
};

// end of include only once
#endif
