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


/**
 * \brief 4 Component Color.
 * 
 * RGBA style color in decimal range. The value of 0 indicates no luminance in that
 * color and 1 indicates full luminance. The color also contains an alpha component
 * where 0 indicates completly transparent and 1 indicates completly opaque.
 */
class DE_DLL_EXPORT decColor{
public:
	/** \brief Red Component of color. */
	float r;
	
	/** \brief Green Component of color. */
	float g;
	
	/** \brief Blue Component of color. */
	float b;
	
	/** \brief Alpha Component of color. */
	float a;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create color initialized to (0,0,0,1). */
	decColor();
	
	/** \brief Create color with initial values. */
	decColor(float red, float green, float blue);
	
	/** \brief Create color with the given values. */
	decColor(float red, float green, float blue, float alpha);
	
	/** \brief Creates copy of a color. */
	decColor(const decColor &copy);
	
	/** \brief Creates copy of a color with different alpha value. */
	decColor(const decColor &copy, float alpha);
	
	/**
	 * \brief Create color from HSV.
	 * \version 1.8
	 * \param[in] hue Hue in the range from 0 (0 degrees) to 1 (360 degrees).
	 *                Values outside this range are normalized hence -0.2
	 *                is used as 0.8 for the calculation.
	 * \param[in] saturation Saturation in the range from 0 to 1. Value is clamped.
	 * \param[in] value Value in the range from 0 to 1. Value is clamped.
	 */
	static decColor CreateHSV(float hue, float saturation, float value);
	
	/**
	 * \brief Create color from HSL.
	 * \version 1.8
	 * \param[in] hue Hue in the range from 0 (0 degrees) to 1 (360 degrees).
	 *                Values outside this range are normalized hence -0.2
	 *                is used as 0.8 for the calculation.
	 * \param[in] saturation Saturation in the range from 0 to 1. Value is clamped.
	 * \param[in] lightness Lightness in the range from 0 to 1. Value is clamped.
	 */
	static decColor CreateHSL(float hue, float saturation, float lightness);
	
	/**
	 * \brief Create color from CMYK.
	 * \version 1.8
	 * \param[in] cyan Cyan in the range from 0 to 1. Value is clamped.
	 * \param[in] magenta Magenta in the range from 0 to 1. Value is clamped.
	 * \param[in] yellow Yellow in the range from 0 to 1. Value is clamped.
	 * \param[in] black Black in the range from 0 to 1. Value is clamped.
	 */
	static decColor CreateCMYK(float cyan, float magenta, float yellow, float black);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Normalized color with each component clamped to the range of 0 to 1. */
	decColor Normalized() const;
	
	/** \brief Normalize color. Each component is clamped to the range of 0 to 1. */
	void Normalize();
	
	/** \brief Inverted color. Each component except alpha is 1-value. */
	decColor Inverted() const;
	
	/** \brief Invert color. Each component except alpha is set to 1-value. */
	void Invert();
	
	/** \brief Set red, green and blue to 0 and alpha to 1. */
	void SetZero();
	
	/** \brief Set values. */
	void Set(float red, float green, float blue);
	
	/** \brief Sets values. */
	void Set(float red, float green, float blue, float alpha);
	
	/** \brief Color is equal component wise to another color with respect to a threshold. */
	bool IsEqualTo(const decColor &color, float threshold = COLOR_THRESHOLD) const;
	
	/**
	 * \brief Convert to HSV.
	 * \version 1.8
	 * \param[out] hue Hue in the range from 0 (0 degrees) to 1 (360 degrees).
	 * \param[out] saturation Saturation in the range from 0 to 1.
	 * \param[out] value Value in the range from 0 to 1.
	 */
	void ToHSV(float &hue, float &saturation, float &value) const;
	
	/**
	 * \brief Convert to HSL.
	 * \version 1.8
	 * \param[out] hue Hue in the range from 0 (0 degrees) to 1 (360 degrees).
	 * \param[out] saturation Saturation in the range from 0 to 1.
	 * \param[out] lightness Lightness in the range from 0 to 1.
	 */
	void ToHSL(float &hue, float &saturation, float &lightness) const;
	
	/**
	 * \brief Convert to CMYK.
	 * \version 1.8
	 * \param[out] cyan Cyan in the range from 0 to 1.
	 * \param[out] magenta Magenta in the range from 0 to 1.
	 * \param[out] yellow Yellow in the range from 0 to 1.
	 * \param[out] black Black in the range from 0 to 1.
	 */
	void ToCMYK(float &cyan, float &magenta, float &yellow, float &black) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy color to this color. */
	decColor &operator=(const decColor &color);
	
	/** \brief Add components of another color to this one. */
	decColor &operator+=(const decColor &color);
	
	/** \brief Subtract components of another color from this color. */
	decColor &operator-=(const decColor &color);
	
	/** \brief Multiply components of this color with a value k. */
	decColor &operator*=(float scalar);
	
	/** \brief Multiply components of this color component wise with another color. */
	decColor &operator*=(const decColor &color);
	
	/**
	 * \brief Divide components of this color by a scalar value.
	 * \throws deeDivisionByZero Scalar is 0.
	 */
	decColor &operator/=(float scalar);
	
	/** \brief Color with the sum of this color with another one. */
	decColor operator+(const decColor &color) const;
	
	/** \brief Color with the difference of this color to another one. */
	decColor operator-(const decColor &color) const;
	
	/** \brief Color with this color scaled by a scalar. */
	decColor operator*(float scalar) const;
	
	/** \brief Color with this color multiplied component wise with another one. */
	decColor operator*(const decColor &color) const;
	
	/**
	 * \brief Color with this color divided by a scalar.
	 * \throws deeDivisionByZero Scalar is 0.
	 */
	decColor operator/(float scalar) const;
	/*@}*/
};
