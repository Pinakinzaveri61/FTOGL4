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


// Material 
	// Ambient // Diffuse // Specular // Shininess 

void CcGPUCtrlIntrFc::SetMatBlendProp(float tranValP, int blendModeP) {

	alpha = tranValP;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CcGPUCtrlIntrFc::CcUploadMatPropToGPU(G3DColorVector matAmbieP, G3DColorVector matDiffuP, G3DColorVector matSpecuP, float matShininessP)
{
	int i;
	GLuint shdPrgIDL;

	for (i = 0; i < INDX_SHD_PRG_COL_MAX; i++) {
		shdPrgIDL = shdPrgIdAryLgt[i];
		CcUploadMatPropToShader(shdPrgIDL, matAmbieP, matDiffuP, matSpecuP, matShininessP);
	}

}


void CcGPUCtrlIntrFc::CcUploadMatPropToShader(GLuint shdPrgIDP, G3DColorVector matAmbieP, G3DColorVector matDiffuP, G3DColorVector matSpecuP, float matShininessP)
{
	int matPropLocL;
	float matR, matG, matB, matA;

	glUseProgram(shdPrgIDP);
	matPropLocL = glGetUniformLocation(shdPrgIDP, "materialU.ambient");
	// glUniform4f(matPropLocL, r, g, b, a);
	matR = matAmbieP.r; matG = matAmbieP.g; matB = matAmbieP.b; matA = matAmbieP.a;
	glUniform3f(matPropLocL, matR, matG, matB);

//	glUseProgram(shdPrgIDP);
	matPropLocL = glGetUniformLocation(shdPrgIDP, "materialU.diffuse");
	matR = matDiffuP.r; matG = matDiffuP.g; matB = matDiffuP.b; matA = matDiffuP.a;
	glUniform3f(matPropLocL, matR, matG, matB);

//	glUseProgram(shdPrgIDP);
	matPropLocL = glGetUniformLocation(shdPrgIDP, "materialU.specular");
	matR = matSpecuP.r; matG = matSpecuP.g; matB = matSpecuP.b; matA = matSpecuP.a;
	glUniform3f(matPropLocL, matR, matG, matB);


//	glUseProgram(shdPrgIDP);
	matPropLocL = glGetUniformLocation(shdPrgIDP, "materialU.shininess");
	glUniform1f(matPropLocL, matShininessP);

	// Set 
	matPropLocL = glGetUniformLocation(shdPrgIDP, "alphaU"); // Material Alpha
	glUniform1f(matPropLocL, alpha);

}


// [(4) color update : if shPrg has the ourColor as uniform]   UPDPRG
// This gives color to a 2D shape
void CcGPUCtrlIntrFc::CcSetUniformColor(float r, float g, float b, float a)
{
	int vertexColorLocation;
	GLuint shdPrgIDL;

	shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_COL];
	glUseProgram(shdPrgIDL);
	vertexColorLocation = glGetUniformLocation(shdPrgIDL, "ourColor");
	glUniform4f(vertexColorLocation, r, g, b, a);


	shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_COL_TXR];
	glUseProgram(shdPrgIDL);
	vertexColorLocation = glGetUniformLocation(shdPrgIDL, "ourColor");
	glUniform4f(vertexColorLocation, r, g, b, a);


}

// [(4) color update : if shPrg has the ourColor as uniform]   UPDPRG
// This gives color to a 2D shape
void CcGPUCtrlIntrFc::CcSetUniformColor(G3DColorVector ourColorP)
{
	CcSetUniformColor(ourColorP.r, ourColorP.g, ourColorP.b, ourColorP.a);
}
