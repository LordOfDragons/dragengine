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

#ifndef _DEDSCLASSCURVEDISTANCEMAPPING_H_
#define _DEDSCLASSCURVEDISTANCEMAPPING_H_

#include <libdscript/libdscript.h>

class decCurveDistanceMapping;
class deScriptingDragonScript;



/**
 * \brief Curve distance mapping script class.
 */
class deClassCurveDistanceMapping : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassCurveDistanceMapping( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassCurveDistanceMapping();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dragonscript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Distance mapping. */
	decCurveDistanceMapping &GetMapping( dsRealObject *myself ) const;
	
	/** \brief Push curve. */
	void PushMapping( dsRunTime *rt, const decCurveDistanceMapping &mapping );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsCDistMap;
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsCBezier3D;
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
	
	DEF_NATFUNC( nfGetResolution );
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfMap );
	DEF_NATFUNC( nfClear );
	DEF_NATFUNC( nfInit );
	DEF_NATFUNC( nfInit2 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
