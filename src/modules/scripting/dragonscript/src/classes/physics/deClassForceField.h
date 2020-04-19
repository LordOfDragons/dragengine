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

#ifndef _DEDSCLASSFORCEFIELD_H_
#define _DEDSCLASSFORCEFIELD_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deForceField;



/**
 * \brief Force field script class.
 */
class deClassForceField : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsForceFieldType;
	dsClass *pClsForceFieldApplication;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassForceField( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassForceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	
	
	/** \brief Force field or \em NULL if myself is \em NULL. */
	deForceField *GetForceField( dsRealObject *myself ) const;
	
	/** \brief Push force field which can be \em NULL. */
	void PushForceField( dsRunTime *rt, deForceField *forcefield );
	
	inline dsClass *GetClassForceFieldType() const{ return pClsForceFieldType; }
	inline dsClass *GetClassForceFieldApplication() const{ return pClsForceFieldApplication; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsFF;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsObject;
		
		dsClass *clsDVec;
		dsClass *clsVec;
		dsClass *clsQuat;
		dsClass *clsCF;
		dsClass *clsShapeList;
		dsClass *clsForceFieldType;
		dsClass *clsForceFieldApplication;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	
	DEF_NATFUNC( nfGetInfluenceArea );
	DEF_NATFUNC( nfSetInfluenceArea );
	DEF_NATFUNC( nfGetRadius );
	DEF_NATFUNC( nfSetRadius );
	DEF_NATFUNC( nfGetExponent );
	DEF_NATFUNC( nfSetExponent );
	
	DEF_NATFUNC( nfGetFieldType );
	DEF_NATFUNC( nfSetFieldType );
	DEF_NATFUNC( nfGetApplicationType );
	DEF_NATFUNC( nfSetApplicationType );
	DEF_NATFUNC( nfGetDirection );
	DEF_NATFUNC( nfSetDirection );
	DEF_NATFUNC( nfGetForce );
	DEF_NATFUNC( nfSetForce );
	DEF_NATFUNC( nfGetFluctuationDirection );
	DEF_NATFUNC( nfSetFluctuationDirection );
	DEF_NATFUNC( nfGetFluctuationForce );
	DEF_NATFUNC( nfSetFluctuationForce );
	
	DEF_NATFUNC( nfGetShape );
	DEF_NATFUNC( nfSetShape );
	DEF_NATFUNC( nfGetCollisionFilter );
	DEF_NATFUNC( nfSetCollisionFilter );
	DEF_NATFUNC( nfGetEnabled );
	DEF_NATFUNC( nfSetEnabled );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
