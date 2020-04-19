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

#ifndef _DEDSCLASSNAVIGATION_H_
#define _DEDSCLASSNAVIGATION_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class dedsNavigationInfo;



/**
 * \brief Navigator information script class.
 */
class deClassNavigationInfo : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassNavigationInfo( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassNavigationInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	
	
	/** \brief Navigation information or \em NULL if myself is \em NULL. */
	dedsNavigationInfo *GetNavigationInfo( dsRealObject *myself ) const;
	
	/** \brief Pushe navigation info or \em NULL. */
	void PushNavigationInfo( dsRunTime *rt, dedsNavigationInfo *info );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNavInfo;
		
		dsClass *clsBool;
		dsClass *clsDVector;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetCostType );
	DEF_NATFUNC( nfSetCostType );
	
	DEF_NATFUNC( nfGetPathIndex );
	DEF_NATFUNC( nfSetPathIndex );
	DEF_NATFUNC( nfGetPathFactor );
	DEF_NATFUNC( nfSetPathFactor );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
