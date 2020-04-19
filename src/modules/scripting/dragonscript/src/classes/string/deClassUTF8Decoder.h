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
#ifndef _DECLASSUTF8DECODER_H_
#define _DECLASSUTF8DECODER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class decUTF8Decoder;



// UTF8 Decoder Class
class deClassUTF8Decoder : public dsClass{
public:
	// constructor
	deClassUTF8Decoder();
	~deClassUTF8Decoder();
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	decUTF8Decoder &GetUTF8Decoder( dsRealObject *myself ) const;
	
private:
	struct sInitData{
		dsClass *clsU8D, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetString );
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfHasReachedEnd );
	DEF_NATFUNC( nfSetString );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfDecodeNextCharacter );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
