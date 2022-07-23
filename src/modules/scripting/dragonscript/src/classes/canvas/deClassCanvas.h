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

#ifndef _DECLASSCANVAS_H_
#define _DECLASSCANVAS_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCanvas;
class deScriptingDragonScript;



/**
 * \brief Canvas script class.
 */
class deClassCanvas : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsCanvasBlendMode;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCanvas( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	inline dsClass *GetClassCanvasBlendMode() const{ return pClsCanvasBlendMode; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve canvas from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCanvas *GetCanvas( dsRealObject *myself ) const;
	
	/**
	 * \brief Push canvas onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCanvas( dsRunTime *rt, deCanvas *canvas );
	
	/** \brief Assigns canvas or \em NULL. */
	void AssignCanvas( dsRealObject *myself, deCanvas *canvas );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCanvas, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsPoint, *clsTexMat2, *clsClrMat;
		dsClass *clsCanvasBlendMode;
		dsClass *clsCanvasView;
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
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	DEF_NATFUNC( nfGetTransform );
	DEF_NATFUNC( nfSetTransform );
	DEF_NATFUNC( nfGetColorTransform );
	DEF_NATFUNC( nfSetColorTransform );
	DEF_NATFUNC( nfGetVisible );
	DEF_NATFUNC( nfSetVisible );
	DEF_NATFUNC( nfGetOrder );
	DEF_NATFUNC( nfSetOrder );
	DEF_NATFUNC( nfGetTransparency );
	DEF_NATFUNC( nfSetTransparency );
	DEF_NATFUNC( nfGetBlendMode );
	DEF_NATFUNC( nfSetBlendMode );
	DEF_NATFUNC( nfGetMask );
	DEF_NATFUNC( nfSetMask );
	DEF_NATFUNC( nfGetParentView );
	DEF_NATFUNC( nfGetParentMask );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
