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

#ifndef _DECLASSOCCLUSIONMESH_H_
#define _DECLASSOCCLUSIONMESH_H_

#include <libdscript/libdscript.h>

class deEngine;
class deOcclusionMesh;
class deScriptingDragonScript;



/**
 * @brief Occlusion Mesh Class.
 */
class deClassOcclusionMesh : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassOcclusionMesh( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	~deClassOcclusionMesh();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** Retrieves the occlusion mesh or NULL if object is NULL. */
	deOcclusionMesh *GetOcclusionMesh( dsRealObject *object ) const;
	/** Pushes an occlusion mesh on the stack (pushes nil if instance is NULL). */
	void PushOcclusionMesh( dsRunTime *rt, deOcclusionMesh *occlusionMesh );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsOccM, *clsVoid, *clsBool, *clsStr, *clsInt, *clsObj, *clsRN;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
#undef DEF_NATFUNC
};

#endif
