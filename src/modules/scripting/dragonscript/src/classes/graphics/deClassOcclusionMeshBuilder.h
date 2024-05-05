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

#ifndef _DECLASSOCCLUSIONMESHBUILDER_H_
#define _DECLASSOCCLUSIONMESHBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief OcclusionMesh builder script class.
 */
class deClassOcclusionMeshBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassOcclusionMeshBuilder( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassOcclusionMeshBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsOcclusionMeshBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsOcclusionMesh;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsPoint;
		dsClass *clsVector2;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfBuild );
	DEF_NATFUNC( nfBuildOcclusionMesh );
	DEF_NATFUNC( nfSetBoneCount );
	DEF_NATFUNC( nfSetBoneAt );
	DEF_NATFUNC( nfSetWeightCount );
	DEF_NATFUNC( nfSetWeightAt );
	DEF_NATFUNC( nfSetWeightGroupCount );
	DEF_NATFUNC( nfSetWeightGroupAt );
	DEF_NATFUNC( nfSetVertexCount );
	DEF_NATFUNC( nfSetVertexAt );
	DEF_NATFUNC( nfSetCornerCount );
	DEF_NATFUNC( nfSetCornerAt );
	DEF_NATFUNC( nfSetFaceCount );
	DEF_NATFUNC( nfSetFaceAt );
	DEF_NATFUNC( nfSetDoubleSidedFaceCount );
#undef DEF_NATFUNC
};

#endif
