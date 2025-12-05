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

#ifndef _DEALWIDGET_H_
#define _DEALWIDGET_H_

#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include "../../deObject.h"
#include "../../common/math/decMath.h"

class dealDisplay;
class dealImage;
class dealShader;
class dealWidgetLayout;
class dealGuiTheme;


/**
 * \brief Widget.
 */
class dealWidget : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealWidget> Ref;


public:
	/** \brief Render context. */
	struct sRenderContext{
		decTexMatrix transform;
		decColor color;
		decPoint screenPosition;
		float clipBaseScreenY;
		decPoint viewFrom;
		decPoint viewTo;
	};
	
private:
	dealDisplay &pDisplay;
	
	dealWidgetLayout *pParent;
	
	decPoint pPosition;
	decPoint pSize;
	
	decPoint pExplicitMinSize;
	bool pHasExplicitMinSize;
	
	decColor pBackgroundColor;
	int pPaddingLeft;
	int pPaddingTop;
	int pPaddingRight;
	int pPaddingBottom;
	
	bool pVisible;
	bool pEnabled;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidget( dealDisplay &display );
	
	/** \brief Create widget. */
	dealWidget( dealDisplay &display, const decPoint &position, const decPoint &size );
	
	/** \brief Clean up widget. */
	virtual ~dealWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Display. */
	inline dealDisplay &GetDisplay() const{ return pDisplay; }
	
	
	
	/** \brief Set widget from gui theme. */
	virtual void SetFromGuiTheme( const dealGuiTheme &guitheme );
	
	
	
	/** \brief Parent widget or \em NULL if not set. */
	inline dealWidgetLayout *GetParent() const{ return pParent; }
	
	/** \brief Set parent widget or \em NULL if not set. */
	void SetParent( dealWidgetLayout *parent );
	
	/** \brief Dirty parent layout. */
	void DirtyParentLayout();
	
	
	
	/** \brief Position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decPoint &position );
	
	/** \brief Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** \brief Set size. */
	void SetSize( const decPoint &size );
	
	/** \brief Background color. */
	inline const decColor &GetBackgroundColor() const{ return pBackgroundColor; }
	
	/** \brief Set background color. */
	void SetBackgroundColor( const decColor &color );
	
	
	
	/** \brief Explicit minimum size if set. */
	inline const decPoint &GetExplicitMinimumSize() const{ return pExplicitMinSize; }
	
	/** \brief Explicit minimum size is set. */
	inline bool GetHasExplicitMinimumSize() const{ return pHasExplicitMinSize; }
	
	/** \brief Set explicit minimum size. */
	void SetExplicitMinimumSize( const decPoint &size );
	
	/** \brief Clear explicit minimum size. */
	void ClearExplicitMinimumSize();
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	
	
	/** \brief Left padding. */
	inline int GetPaddingLeft() const{ return pPaddingLeft; }
	
	/** \brief Set left padding. */
	void SetPaddingLeft( int padding );
	
	/** \brief Top padding. */
	inline int GetPaddingTop() const{ return pPaddingTop; }
	
	/** \brief Set top padding. */
	void SetPaddingTop( int padding );
	
	/** \brief Right padding. */
	inline int GetPaddingRight() const{ return pPaddingRight; }
	
	/** \brief Set right padding. */
	void SetPaddingRight( int padding );
	
	/** \brief Bottom padding. */
	inline int GetPaddingBottom() const{ return pPaddingBottom; }
	
	/** \brief Set bottom padding. */
	void SetPaddingBottom( int padding );
	
	/** \brief Set padding. */
	void SetPadding( int horizontal, int vertical );
	
	/** \brief Set padding. */
	void SetPadding( int padding );
	
	/** \brief Set padding. */
	void SetPaddingLeft( int left, int top, int right, int bottom );
	
	/** \brief Size of content area which is widget size minus padding. */
	decPoint GetContentArea() const;
	
	
	
	/** \brief Widget is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if widget is visible. */
	void SetVisible( bool visible );
	
	/** \brief Widget is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if widget is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Widget and all parent widgets are enabled. */
	bool GetFullEnabled() const;
	
	
	
	/** \brief Widget position in dialog coordinates. */
	virtual decPoint GetDialogPosition() const;
	
	/** \brief Widget position in screen coordinates. */
	decPoint GetScreenPosition() const;
	
	/** \brief Position is inside widget. */
	bool IsPointInside( const decPoint &point ) const;
	
	/** \brief Widget containing position or \em null if not found. */
	virtual dealWidget *WidgetAtPosition( const decPoint &point ) const;
	
	
	
	/** \brief Capture input to this widget. */
	void CaptureInput();
	
	/** \brief Release capture input on this widget if set. */
	void ReleaseCapture();
	
	
	
	/** \brief Render. */
	virtual void Render( const sRenderContext &context );
	
	/** \brief Render background. */
	virtual void RenderBackground( const sRenderContext &context );
	
	/** \brief Render content. */
	virtual void RenderContent( const sRenderContext &context );
	
	
	
	/** \brief Position changed. */
	virtual void OnPositionChanged();
	
	/** \brief Size changed. */
	virtual void OnSizeChanged();
	
	/** \brief Parent changed. */
	virtual void OnParentChanged();
	
	/** \brief Padding changed. */
	virtual void OnPaddingChanged();
	
	/** \brief Visiblity changed. */
	virtual void OnVisibilityChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	
	
	/**
	 * \brief Key has been pressed down.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyPress( int keycode );
	
	/**
	 * \brief Key has been released.
	 * \param[in] keycode A value of AKEYCODE_*.
	 */
	virtual void OnKeyRelease( int keycode );
	
	/**
	 * \brief Mouse button press / finger press.
	* \param[in] buttons Buttons pressed while pressing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMousePress( int buttons, const decPoint &position );
	
	/**
	 * \brief Mouse button release / finger release.
	 * \param[in] buttons Buttons pressed while releasing screen. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMouseRelease( int buttons, const decPoint &position );
	
	/**
	 * \brief Mouse/finger moved while pressing display.
	 * \param[in] buttons Buttons pressed while moving. OR of values of AMOTION_EVENT_BUTTON_.
	 */
	virtual void OnMouseMove( int buttons, const decPoint &position );
	
	
	
	/** \brief Set transformation shader parameter. */
	void ShaderSetTransform( const sRenderContext &context,
		dealShader &shader, int x1, int y1, int x2, int y2 ) const;
	
	/** \brief Set transformation shader parameter. */
	void ShaderSetTransform( const sRenderContext &context,
		dealShader &shader, const decPoint &position, const decPoint &size ) const;
	
	/** \brief Set texture coordinates transform to default. */
	void ShaderSetTCTransform( dealShader &shader );
	
	/** \brief Set texture coordinates transform. */
	void ShaderSetTCTransform( dealShader &shader, const decTexMatrix &matrix );
	
	/** \brief Set texture coordinates transform. */
	void ShaderSetTCTransform( dealShader &shader, const decTexMatrix2 &matrix );
	
	/** \brief Set texture coordinates transform. */
	void ShaderSetTCTransform( dealShader &shader, float u1, float v1, float u2, float v2 );
	
	/** \brief Set texture coordinates transform. */
	void ShaderSetTCTransform( dealShader &shader, const decVector2 &position, const decVector2 &size );
	
	/** \brief Set color matrix shader parameter. */
	void ShaderSetColorMatrix( dealShader &shader, const decColor &color ) const;
	
	/** \brief Set color matrix shader parameter. */
	void ShaderSetColorMatrixDisabled( dealShader &shader, const decColor &color ) const;
	
	/** \brief Set color matrix shader parameter. */
	void ShaderSetColorMatrix( dealShader &shader, const decColorMatrix &matrix ) const;
	
	/** \brief Set gamma shader parameter. */
	void ShaderSetGamma( dealShader &shader, float gamma ) const;
	
	/** \brief Set clip rect shader parameter. */
	void ShaderSetClipRect( const sRenderContext &context, dealShader &shader ) const;
	
	/** \brief Set clip rect shader parameter. */
	void ShaderSetClipRect( const sRenderContext &context, dealShader &shader,
		int x1, int y1, int x2, int y2 ) const;
	
	/** \brief Set clip rect shader parameter. */
	void ShaderSetClipRect( const sRenderContext &context, dealShader &shader,
		const decPoint &position, const decPoint &size ) const;
	
	/** \brief Set texture coordinate clamp shader parameter to default. */
	void ShaderSetTCClamp( dealShader &shader ) const;
	
	/** \brief Set texture coordinate clamp shader parameter. */
	void ShaderSetTCClamp( dealShader &shader, float u1, float v1, float u2, float v2 ) const;
	
	/** \brief Set texture coordinate clamp shader parameter. */
	void ShaderSetTCClamp( dealShader &shader, const decVector2 &position, const decVector2 &size ) const;
	
	/** \brief Bind texture. */
	void BindTexture( int stage, const dealImage &image, bool linearFiltering = true, bool repeatWrap = false );
	
	/** \brief Unbind texture. */
	void UnbindTexture( int stage );
	
	/** \brief Draw point. */
	void DrawPoint();
	
	/** \brief Draw line. */
	void DrawLine();
	
	/** \brief Draw rectangle. */
	void DrawRectangle();
	/*@}*/
};

#endif
