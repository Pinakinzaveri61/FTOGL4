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



typedef double ccVTvec;	// Variable type for vector

#ifndef G3DVector_h // Required to avoid conflict due to multiple file include.
#include <math.h>  // to find sqrt


class G3DVector
{
public:
	union
	{
		ccVTvec v[3];
		struct{ccVTvec x, y, z;};
		struct{ccVTvec p1, p2, p3;};
	};

	G3DVector();
	// G3DVector(G3DVector &v);
	G3DVector(ccVTvec xP, ccVTvec yP, ccVTvec zP);

	G3DVector(const G3DVector &vec);    // Special copy constructor
	
	G3DVector& operator=(const G3DVector &vec);  // Special =

	bool operator==(G3DVector & vec);

	void Normalize(ccVTvec n=1.0f);
	void SetV(ccVTvec xP, ccVTvec yP, ccVTvec zP);


	void GetModuleVerF(float* moduleVerF);

};


class G3DColorVector
{
public:


	union
	{
		float color[4];
		float c[4];
		struct{float r, g, b, a;};
	};


	G3DColorVector();
	G3DColorVector(const G3DColorVector &cV);	    // Special copy constructor

	G3DColorVector(float rP, float gP, float bP, float aP);

	G3DColorVector& operator=(const G3DColorVector &cV);	// Special 

	bool operator==(G3DColorVector &cV);

	void SetColor(float rP, float gP, float bP, float aP);
	void SetRGBColor(int iR, int iG, int iB);

};



#define G3DVector_h 
#endif

