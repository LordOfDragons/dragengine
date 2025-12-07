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

#ifndef _DECLASSMODELBUILDER_H_
#define _DECLASSMODELBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Model builder script class.
 */
class deClassModelBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassModelBuilder(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassModelBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsModelBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsModel;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsPoint;
		dsClass *clsVector2;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfBuild);
	DEF_NATFUNC(nfBuildModel);
	DEF_NATFUNC(nfAddBone);
	DEF_NATFUNC(nfAddTexture);
	DEF_NATFUNC(nfAddLOD);
	DEF_NATFUNC(nfSetLodError);
	DEF_NATFUNC(nfAddTextureCoordinatesSet);
	DEF_NATFUNC(nfSetWeightCount);
	DEF_NATFUNC(nfSetWeightAt);
	DEF_NATFUNC(nfSetWeightGroupCount);
	DEF_NATFUNC(nfSetWeightGroupAt);
	DEF_NATFUNC(nfSetVertexCount);
	DEF_NATFUNC(nfSetVertexAt);
	DEF_NATFUNC(nfSetNormalCount);
	DEF_NATFUNC(nfSetTangentCount);
	DEF_NATFUNC(nfSetFaceCount);
	DEF_NATFUNC(nfSetFaceAt);
	DEF_NATFUNC(nfSetTextureCoordinateSetCount);
	DEF_NATFUNC(nfSetTextureCoordinateSetAtSetCount);
	DEF_NATFUNC(nfSetTextureCoordinateSetAtSetAt);
#undef DEF_NATFUNC
};

#endif
