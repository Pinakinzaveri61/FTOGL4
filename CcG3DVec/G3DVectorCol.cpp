/*
 * G3DVector - ConsCAD utility library (Reduced, Limited functions Library)
 *
 * Copyright (c) 2019 Pinakin Zaveri <Pinakin.Zaveri@rediff.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "pch.h"

#include "G3DVector.h"

// ***************************************************************

G3DColorVector::G3DColorVector()
{
	r = 1.0;
	g = 1.0;
	b = 1.0;
	a = 1.0;
}

G3DColorVector::G3DColorVector(const G3DColorVector &cV)
{
	c[0] = cV.c[0];
	c[1] = cV.c[1];
	c[2] = cV.c[2];
	c[3] = cV.c[3];
}

G3DColorVector::G3DColorVector(float rP, float gP, float bP, float aP)
{
	c[0] = rP;
	c[1] = gP;
	c[2] = bP;
	c[3] = aP;

}




G3DColorVector& G3DColorVector::operator=(const G3DColorVector &cV)
{
	color[0] = cV.color[0];
	color[1] = cV.color[1];
	color[2] = cV.color[2];
	color[3] = cV.color[3];

	return *this;
}




bool G3DColorVector::operator==(G3DColorVector &cV)
{
	return (c[0] == cV.c[0] && c[1] == cV.c[1] && c[2] == cV.c[2] && c[3] == cV.c[3]);
}


// ***********

void G3DColorVector::SetColor(float rP, float gP, float bP, float aP)
{
	color[0] = rP;
	color[1] = gP;
	color[2] = bP;
	color[3] = aP;
}


void G3DColorVector::SetRGBColor(int iR, int iG, int iB)
{
	color[0] = (float) iR / 255.0f;
	color[1] = (float) iG / 255.0f;
	color[2] = (float) iB / 255.0f;
	color[3] = 1.0;
}
