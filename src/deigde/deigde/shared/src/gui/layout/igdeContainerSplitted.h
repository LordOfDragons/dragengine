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

#ifndef _IGDECONTAINERSPLITTED_H_
#define _IGDECONTAINERSPLITTED_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget splitting space between two widgets.
 * 
 * The layout contains at most 2 widgets. Both widgets are set to fill the available space.
 * The first widget has desired size along the placement direction. The second widget is
 * stretched along the placement direction. Between the two widgets a grabber is displayed
 * allowing the user to change the size of the side widget.
 */
class DE_DLL_EXPORT igdeContainerSplitted : public igdeContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeContainerSplitted> Ref;
	
	
	/** \brief Side widget placement. */
	enum eSidePlacement{
		/** \brief Side left, center right. */
		espLeft,
		
		/** \brief Side right, center left. */
		espRight,
		
		/** \brief Side top, center below. */
		espTop,
		
		/** \brief Side bottom, center above. */
		espBottom
	};
	
	/** \brief Area. */
	enum eArea{
		/** \brief Side. */
		eaSide,
		
		/** \brief Center. */
		eaCenter
	};
	
	
	
private:
	eSidePlacement pSidePlacement;
	int pSplitPosition;
	float pSplitPositionRelative;
	igdeWidget *pWidgetSide;
	igdeWidget *pWidgetCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerSplitted(igdeEnvironment &environment, eSidePlacement sidePlacement);
	
	/** \brief Create widget. */
	igdeContainerSplitted(igdeEnvironment &environment,
		eSidePlacement sidePlacement, int splitPosition);
	
	/** \brief Create widget. */
	igdeContainerSplitted(igdeEnvironment &environment,
		eSidePlacement sidePlacement, float splitPositionRelative);
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerSplitted();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Side placement. */
	inline eSidePlacement GetSidePlacement() const{return pSidePlacement;}
	
	/** \brief Split position. */
	inline int GetSplitPosition() const{return pSplitPosition;}
	
	/** \brief Set split position. */
	void SetSplitPosition(int splitPosition);
	
	/** \brief Relative split position. */
	inline float GetSplitPositionRelative() const{return pSplitPositionRelative;}
	
	/** \brief Set relative split position. */
	void SetSplitPositionRelative(float splitPosition);
	
	
	
	/** \brief Add child to area. */
	virtual void AddChild(igdeWidget *child, eArea area);
	
	/** \brief Add child not possible without specifying an area. */
	virtual void AddChild(igdeWidget *child);
	
	/** \brief Remove child. */
	virtual void RemoveChild(igdeWidget *child);
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	
	/** \brief Widget in area or NULL. */
	igdeWidget *GetWidgetIn(eArea area) const;
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	
	/**
	 * \brief Create child widget native widgets.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateChildWidgetNativeWidgets();
	/*@}*/
};

#endif
