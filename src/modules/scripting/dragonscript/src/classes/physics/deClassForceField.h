/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
		dsClass *clsWorld;
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
	DEF_NATFUNC(nfGetParentWorld);
	
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
