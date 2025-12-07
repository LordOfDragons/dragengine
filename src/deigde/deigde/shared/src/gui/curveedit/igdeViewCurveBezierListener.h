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

#ifndef _IGDEVIEWCURVEBEZIERLISTENER_H_
#define _IGDEVIEWCURVEBEZIERLISTENER_H_


#include <dragengine/deObject.h>


class igdeViewCurveBezier;
class igdeMenuCascade;


/**
 * \brief IGDE UI ViewCurveBezier Listener.
 */
class DE_DLL_EXPORT igdeViewCurveBezierListener : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeViewCurveBezierListener> Ref;
	
	
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeViewCurveBezierListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeViewCurveBezierListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief ViewCurveBezier curve changed.
	 * 
	 * User finished modifying curve. Called if values are changed manually or user
	 * used mouse interaction to manipulate curve and finished the interaction.
	 */
	virtual void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier);
	
	/**
	 * \brief ViewCurveBezier curve is changing.
	 * 
	 * User is using mouse interaction to manipulate curve but has not yet finished.
	 * This event is send each time the curve changed while the user is actively
	 * modifying using mouse interaction. Once finished OnCurveChanged() is called.
	 */
	virtual void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier);
	
	/**
	 * \brief ViewCurveBezier selected point changed.
	 * 
	 * User selected curve point.
	 */
	virtual void OnSelectedPointChanged(igdeViewCurveBezier *viewCurveBezier);
	
	/**
	 * \brief Create context menu.
	 * 
	 * Context menu entries have to be added to the menu. This allows multiple listeners
	 * to add entries to the context menu without affecting others. If entries are already
	 * present the listener should also add a separator first.
	 */
	virtual void AddContextMenuEntries(igdeViewCurveBezier *viewCurveBezier, igdeMenuCascade &menu);
	/*@}*/
};

#endif
