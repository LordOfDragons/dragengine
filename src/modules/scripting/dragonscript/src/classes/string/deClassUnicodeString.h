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
#ifndef _DECLASSUNICODESTRING_H_
#define _DECLASSUNICODESTRING_H_

// includes
#include <libdscript/libdscript.h>

class decString;
// predefinitions
class decUnicodeString;



// Unicode String Class
class deClassUnicodeString : public dsClass{
public:
	// constructor
	deClassUnicodeString();
	~deClassUnicodeString();
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	const decUnicodeString &GetUnicodeString( dsRealObject *myself ) const;
	const decString GetUTF8( dsRealObject *myself ) const;
	void PushUnicodeString( dsRunTime *rt, const decUnicodeString &string );
	void PushUTF8( dsRunTime *rt, const char *utf8 );
	// for functions only
	void PushUnicodeAsUTF8( dsRunTime *rt, dsValue *myself );
	
private:
	struct sInitData{
		dsClass *clsUS, *clsVoid, *clsInt, *clsFlt, *clsStr;
		dsClass *clsObj, *clsBool, *clsByte;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewFrom );
	DEF_NATFUNC( nfNewFromCharacter );
	DEF_NATFUNC( nfNewFromUTF8 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfGetCharacterAt );
	DEF_NATFUNC( nfGetLeft );
	DEF_NATFUNC( nfGetRight );
	DEF_NATFUNC( nfGetSubString );
	DEF_NATFUNC( nfReverse );
	
	DEF_NATFUNC( nfTrimLeft );
	DEF_NATFUNC( nfTrimRight );
	DEF_NATFUNC( nfTrimBoth );
	
	DEF_NATFUNC( nfFindChar );
	DEF_NATFUNC( nfFindCharReverse );
	
	DEF_NATFUNC( nfCompareTo );
	DEF_NATFUNC( nfCompareToNoCase );
	
	DEF_NATFUNC( nfToLower );
	DEF_NATFUNC( nfToUpper );
	DEF_NATFUNC( nfToInt );
	DEF_NATFUNC( nfToFloat );
	DEF_NATFUNC( nfToString );
	DEF_NATFUNC( nfToUTF8 );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpAddByte );
	DEF_NATFUNC( nfOpAddBool );
	DEF_NATFUNC( nfOpAddInt );
	DEF_NATFUNC( nfOpAddFloat );
	DEF_NATFUNC( nfOpAddObject );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfCompare );
#undef DEF_NATFUNC
};

// end of include only once
#endif
