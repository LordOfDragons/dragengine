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

#ifndef _DECLASSTEXMATRIX2_H_
#define _DECLASSTEXMATRIX2_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;



/**
 * \brief 3x2 Texture Matrix Script Class.
 */
class deClassTexMatrix2 : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new class. */
	deClassTexMatrix2( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassTexMatrix2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Texture matrix from an object.
	 * \throws dueNullPointer \em myself is \em NULL.
	 */
	const decTexMatrix2 &GetTexMatrix( dsRealObject *myself ) const;
	
	/** \brief Pushe texture matrix. */
	void PushTexMatrix( dsRunTime *rt, const decTexMatrix2 &matrix );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsTexMat, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsVec2, *clsFileReader, *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfNewTranslation );
	DEF_NATFUNC( nfNewTranslation2 );
	DEF_NATFUNC( nfNewScaling );
	DEF_NATFUNC( nfNewScaling2 );
	DEF_NATFUNC( nfNewRotation );
	DEF_NATFUNC( nfNewShear );
	DEF_NATFUNC( nfNewShear2 );
	DEF_NATFUNC( nfNewST );
	DEF_NATFUNC( nfNewST2 );
	DEF_NATFUNC( nfNewSRT );
	DEF_NATFUNC( nfNewSRT2 );
	DEF_NATFUNC( nfNewCenterSRT );
	DEF_NATFUNC( nfNewCenterSRT2 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfGet11 );
	DEF_NATFUNC( nfGet12 );
	DEF_NATFUNC( nfGet13 );
	DEF_NATFUNC( nfGet21 );
	DEF_NATFUNC( nfGet22 );
	DEF_NATFUNC( nfGet23 );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpSubtract );
	DEF_NATFUNC( nfOpScale );
	DEF_NATFUNC( nfOpDivide );
	DEF_NATFUNC( nfOpMultiply );
	DEF_NATFUNC( nfOpMultiply2 );
	DEF_NATFUNC( nfOpMultiply3 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfToString );
	DEF_NATFUNC( nfToStringPrecision );
#undef DEF_NATFUNC
};

#endif
