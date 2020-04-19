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

#ifndef _DECLASSCANVASPAINT_H_
#define _DECLASSCANVASPAINT_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deCanvasPaint;
class deScriptingDragonScript;



/**
 * \brief Canvas paint script class.
 */
class deClassCanvasPaint : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsCanvasPaintShape;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new script class. */
	deClassCanvasPaint( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassCanvasPaint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	inline dsClass *GetClassCanvasPaintShape() const{ return pClsCanvasPaintShape; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve canvas paint from a script object.
	 * \details Returns \em NULL if \em myself is \em NULL.
	 */
	deCanvasPaint *GetCanvas( dsRealObject *myself ) const;
	
	/**
	 * \brief Push canvas paint onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is \em NULL.
	 */
	void PushCanvas( dsRunTime *rt, deCanvasPaint *canvas );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCPaint, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsPoint, *clsColor;
		dsClass *clsCanvasPaintShape;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetShapeType );
	DEF_NATFUNC( nfSetShapeType );
	DEF_NATFUNC( nfGetLineColor );
	DEF_NATFUNC( nfSetLineColor );
	DEF_NATFUNC( nfGetFillColor );
	DEF_NATFUNC( nfSetFillColor );
	DEF_NATFUNC( nfGetThickness );
	DEF_NATFUNC( nfSetThickness );
	DEF_NATFUNC( nfGetRoundCornerX );
	DEF_NATFUNC( nfSetRoundCornerX );
	DEF_NATFUNC( nfGetRoundCornerY );
	DEF_NATFUNC( nfSetRoundCornerY );
	DEF_NATFUNC( nfGetStartAngle );
	DEF_NATFUNC( nfSetStartAngle );
	DEF_NATFUNC( nfGetEndAngle );
	DEF_NATFUNC( nfSetEndAngle );
	DEF_NATFUNC( nfGetPointCount );
	DEF_NATFUNC( nfGetPointAt );
	DEF_NATFUNC( nfAddPoint );
	DEF_NATFUNC( nfRemoveAllPoints );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
