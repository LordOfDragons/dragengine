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

#ifndef _DECLASSSKINBUILDER_H_
#define _DECLASSSKINBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Skin builder script class.
 */
class deClassSkinBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkinPropertyNodeCombineMode;
	dsClass *pClsSkinPropertyMappedInputType;
	dsClass *pClsSkinMappedInputType;
	dsClass *pClsSkinMappedRenderableComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkinBuilder(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassSkinBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	inline dsClass *GetClassSkinPropertyNodeCombineMode() const{return pClsSkinPropertyNodeCombineMode;}
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkinBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsSkin;
		dsClass *clsColor;
		dsClass *clsImage;
		dsClass *clsVideo;
		dsClass *clsPoint;
		dsClass *clsPoint3;
		dsClass *clsVector2;
		dsClass *clsFont;
		dsClass *clsCurveBezier;
		dsClass *clsSkinPropertyNodeCombineMode;
		dsClass *clsSkinPropertyMappedInputType;
		dsClass *clsSkinMappedInputType;
		dsClass *clsSkinMappedRenderableComponent;
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
	DEF_NATFUNC(nfBuildSkin);
	DEF_NATFUNC(nfAddMapped);
	DEF_NATFUNC(nfAddTexture);
	DEF_NATFUNC(nfAddPropertyValue);
	DEF_NATFUNC(nfAddPropertyColor);
	DEF_NATFUNC(nfAddPropertyImage);
	DEF_NATFUNC(nfAddPropertyVideo);
	DEF_NATFUNC(nfAddPropertyConstructed);
	DEF_NATFUNC(nfAddPropertyMapped);
	DEF_NATFUNC(nfAddPropertyMapped2);
	DEF_NATFUNC(nfSetPropertyBone);
	DEF_NATFUNC(nfConstructedOpenContent);
	DEF_NATFUNC(nfAddNodeImage);
	DEF_NATFUNC(nfAddNodeText);
	DEF_NATFUNC(nfAddNodeShape);
	DEF_NATFUNC(nfAddNodeGroup);
	DEF_NATFUNC(nfNextNodeAsMask);
	DEF_NATFUNC(nfCloseNode);
#undef DEF_NATFUNC
};

#endif
