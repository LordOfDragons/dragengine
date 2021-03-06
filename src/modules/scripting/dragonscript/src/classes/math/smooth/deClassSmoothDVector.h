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

#ifndef _DECLASSSMOOTHDVECTOR_H_
#define _DECLASSSMOOTHDVECTOR_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class decSmoothDVector;
class deScriptingDragonScript;



/**
 * \brief Smooth DVector script class.
 */
class deClassSmoothDVector : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \brief Create a new class. */
	deClassSmoothDVector( deScriptingDragonScript &ds );
	
	/** \brief Clean up the class. */
	virtual ~deClassSmoothDVector();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Smooth vector from object. */
	const decSmoothDVector &GetSmoothDVector( dsRealObject *myself ) const;
	
	/** \brief Push smooth vector. */
	void PushSmoothDVector( dsRunTime *rt, const decSmoothDVector &smoothDVector );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsSmDVector, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt, *clsDVec;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
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
	
	DEF_NATFUNC( nfGetValue );
	DEF_NATFUNC( nfSetValue );
	DEF_NATFUNC( nfGetGoal );
	DEF_NATFUNC( nfSetGoal );
	DEF_NATFUNC( nfGetAdjustTime );
	DEF_NATFUNC( nfSetAdjustTime );
	DEF_NATFUNC( nfGetAdjustRange );
	DEF_NATFUNC( nfSetAdjustRange );
	DEF_NATFUNC( nfGetChangeSpeed );
	DEF_NATFUNC( nfSetChangeSpeed );
	
	DEF_NATFUNC( nfReset );
	DEF_NATFUNC( nfUpdate );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
