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

#ifndef _DEDSCLASSSKYINSTANCE_H_
#define _DEDSCLASSSKYINSTANCE_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;
class deSkyInstance;



/**
 * \brief Sky instance script class.
 */
class deClassSkyInstance : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkyInstance( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSkyInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Instance or \em NULL if myself is \em NULL. */
	deSkyInstance *GetInstance( dsRealObject *myself ) const;
	
	/** \brief Push instance which can be \em NULL. */
	void PushInstance( dsRunTime *rt, deSkyInstance *instance );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkyInst;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsSky;
		dsClass *clsSkyCtrl;
		dsClass *clsLayerMask;
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
	DEF_NATFUNC( nfSetSky );
	DEF_NATFUNC( nfGetOrder );
	DEF_NATFUNC( nfSetOrder );
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	
	DEF_NATFUNC( nfGetControllerCount );
	DEF_NATFUNC( nfGetControllerAt );
	DEF_NATFUNC( nfGetControllerNamed );
	DEF_NATFUNC( nfIndexOfControllerNamed );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
