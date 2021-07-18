/*
 * CcGPUCtrl - ConsCAD utility library (Reduced, Limited functions Library)
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
#include "CcGPUCtrlIntrFc.h"



CcGPUCtrlIntrFc::CcGPUCtrlIntrFc()
{

	indc = 0;
	patnFstTimeF = true;

	alpha = 1.0;

	modelMtrxAryIndx = 0;	// Init to start Array
}


CcGPUCtrlIntrFc::~CcGPUCtrlIntrFc() {}


/*
	Note: When a new Shader program added, Pl. make sure its prgID is used and 
	(1) P updated in projection, (2) V updated in Camera, (3) Model in Trans, [(4) color update : if shPrg has the color as uniform]
	As all ShPrg uses the same PVM, for a single scene. We update all ShPrg with same PVM.
	(Search for UPDPRG, for ease of update)
*/

//   
void CcGPUCtrlIntrFc::CreGeneralShaderFmFile()
{
	GLuint indx1L;

	indx1L = SHD_PRG_INDX_COL;
//	shdPrgIdAry[indx1L] = InitShaderFile("E:\\FTOGL4\\CcGPUCtrlLib\\shader\\Vrtx.vs", "E:\\FTOGL4\\CcGPUCtrlLib\\shader\\Vrtx.fs");
	shdPrgIdAry[indx1L] = InitShaderFile(".\\..\\CcGPUCtrlLib\\shader\\Vrtx.vs", ".\\..\\CcGPUCtrlLib\\shader\\Vrtx.fs");

	indx1L = SHD_PRG_INDX_COL_TXR;
	shdPrgIdAry[indx1L] = InitShaderFile(".\\..\\CcGPUCtrlLib\\shader\\VrtxTxr.vs", ".\\..\\CcGPUCtrlLib\\shader\\VrtxTxr.fs");

	indx1L = SHD_PRG_INDX_SELECT;
	shdPrgIdAry[indx1L] = InitShaderFile(".\\..\\CcGPUCtrlLib\\shader\\VrtxSelect.vs", ".\\..\\CcGPUCtrlLib\\shader\\VrtxSelect.fs");

}

// Creates shaders for Material color , texture and map for given number of lights.
void CcGPUCtrlIntrFc::CreLightShaderFmFile(int lgtShdrType)
{
	// GLuint indx1L;
	GLuint indx2L;

	// Lights for Material-Color

		indx2L = SHD_PRG_INDX_COL_MAT_COLOR;
		shdPrgIdAryLgt[indx2L] = InitShaderFile(".\\..\\CcGPUCtrlLib\\shader\\MatColLigtP6L10.vs", ".\\..\\CcGPUCtrlLib\\shader\\MatColLigtP6L10.fs");
	
		// Lights for Material-Texture
		indx2L = SHD_PRG_INDX_COL_MAT_COLOR_TXR;
		shdPrgIdAryLgt[indx2L] = InitShaderFile(".\\..\\CcGPUCtrlLib\\shader\\MatColLigtTxrP6L10.vs", ".\\..\\CcGPUCtrlLib\\shader\\MatColLigtTxrP6L10.fs");

}



void CcGPUCtrlIntrFc::OpenGLDrawInit(G3DColorVector backGndClrP) {

	
	glClearColor(backGndClrP.r, backGndClrP.g, backGndClrP.b, backGndClrP.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CcLoadIdentity(MATRIX_ID_MODEL);	// ** ModelMatrix = Unit on each fresh draw.


	bool bCull, bDepth, bOutline, fClkWise, fShdSmooth;
	bCull = true;		// True hides the back side
	bDepth = true;		// Depth test should be True most of the time.

	bOutline = true;
	fClkWise = false;
	fShdSmooth = true;	// True- Smooth shading else Flat.

	

	if(bCull)
		glEnable(GL_CULL_FACE); // Do not try to display the back side.
	else
		glDisable(GL_CULL_FACE); // Display the back side.

	
	if(bDepth)
		glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	else
		glDisable(GL_DEPTH_TEST);
	
	
	if(bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	if(fClkWise)
		glFrontFace(GL_CW);		// clockwise polygons face out.
	else
		glFrontFace(GL_CCW);	// Counterclockwise polygons face out.

	if(fShdSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	glCullFace(GL_BACK);

}



// Calculates the UnitNormal vector to the tringle.
// 3 points input vertex1, 2, 3 each with x,y & z values.
// IT returns the unit vector in rVN.
void CcGPUCtrlIntrFc::getFaceUnitNormal(float* rVN, float vertx1[3], float vertx2[3], float vertx3[3])
{

	float vect1[3];
	float vect2[3];

	// Find two vector from 3 vertices.
	// i=0 is x, i=1 is y, i=2 is z.
	vect1[0] = vertx1[0] - vertx2[0];
	vect1[1] = vertx1[1] - vertx2[1];
	vect1[2] = vertx1[2] - vertx2[2];

	vect2[0] = vertx2[0] - vertx3[0];
	vect2[1] = vertx2[1] - vertx3[1];
	vect2[2] = vertx2[2] - vertx3[2];



	//calculates vector at 90° to 2 vectors
	//	find crossProduct(norm,vect[0],vect[1]);

	rVN[0] = vect1[1] * vect2[2] - vect2[1] * vect1[2];
	rVN[1] = vect1[2] * vect2[0] - vect2[2] * vect1[0];
	rVN[2] = vect1[0] * vect2[1] - vect2[0] * vect1[1];

	//	normalize(norm);									//makes the vector length 1

	float length;

	length = rVN[0] * rVN[0] + rVN[1] * rVN[1] + rVN[2] * rVN[2];
	length = sqrt(length);		// sqrt(x^2 + y^2 + z^2)

	if (length == 0.0) length = 1.0;	// Avoid devide by Zero

	//divides vector by its length to normalise		
	rVN[0] /= length;
	rVN[1] /= length;
	rVN[2] /= length;

}
