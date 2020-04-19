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

#ifndef _DEDSCLASSSKYBODY_H_
#define _DEDSCLASSSKYBODY_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSky;



/**
 * \brief Sky body script class.
 */
class deClassSkyBody : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkyBody( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSkyBody();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Push body. */
	void PushBody( dsRunTime *rt, deSky *sky, int layer, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkyBody;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsSky;
		dsClass *clsQuaternion;
		dsClass *clsVector2;
		dsClass *clsColor;
		dsClass *clsSkin;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSky );
	DEF_NATFUNC( nfGetLayerIndex );
	DEF_NATFUNC( nfGetBodyIndex );
	
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	DEF_NATFUNC( nfGetColor );
	DEF_NATFUNC( nfSetColor );
	DEF_NATFUNC( nfGetSkin );
	DEF_NATFUNC( nfSetSkin );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
