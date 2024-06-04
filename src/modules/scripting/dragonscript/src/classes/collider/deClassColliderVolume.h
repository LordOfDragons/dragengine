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

#ifndef _DECLASSCOLLIDERVOLUME_H_
#define _DECLASSCOLLIDERVOLUME_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deColliderVolume;
class deScriptingDragonScript;



/**
 * \brief Collider script class.
 */
class deClassColliderVolume : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new script class. */
	deClassColliderVolume( deScriptingDragonScript &ds );
	/** \brief Clean up the script class. */
	virtual ~deClassColliderVolume();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve collider from a script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deColliderVolume *GetCollider( dsRealObject *myself ) const;
	
	/**
	 * \brief Push collider onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushCollider( dsRunTime *rt, deColliderVolume *collider );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsColVol, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsQuat;
		dsClass *clsVec, *clsObj, *clsBool, *clsDVec, *clsDMat, *clsShapeList;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetShape );
	DEF_NATFUNC( nfSetShape );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
