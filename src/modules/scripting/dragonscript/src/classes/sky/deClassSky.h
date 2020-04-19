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

#ifndef _DECLASSSKY_H_
#define _DECLASSSKY_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSky;



/**
 * \brief Sky script class.
 */
class deClassSky : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSky( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Sky or \em NULL if myself is \em NULL. */
	deSky *GetSky( dsRealObject *myself ) const;
	
	/** \brief Push sky which can be \em NULL. */
	void PushSky( dsRunTime *rt, deSky *sky );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSky;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsColor;
		dsClass *clsSkyCtrl;
		dsClass *clsSkyLink;
		dsClass *clsSkyLayer;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetBgColor );
	DEF_NATFUNC( nfSetBgColor );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfSetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	
	DEF_NATFUNC( nfGetLinkCount );
	DEF_NATFUNC( nfSetLinkCount );
	DEF_NATFUNC( nfGetLinkAt );
	
	DEF_NATFUNC( nfGetLayerCount );
	DEF_NATFUNC( nfSetLayerCount );
	DEF_NATFUNC( nfGetLayerAt );
	
	DEF_NATFUNC( nfContentChanged );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
