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


// note:since the projection matrix rarely changes,
// i.e. remains the same for the Single scene. So we set it once 


bool CcGPUCtrlIntrFc::SetProjMatrixPerspective(float FOV, float aspect, float nearP, float farP)
{


	/*
	Its first parameter defines the fov value, that stands for field of view and sets how large the viewspace is. 
	For a realistic view it is usually set to 45 degrees, but for more doom-style results you could set it to a 
	higher value. The second parameter sets the aspect ratio which is calculated by dividing the viewport's width 
	by its height. The third and fourth parameter set the near and far plane of the frustum. We usually 
	set the near distance to 0.1 and the far distance to 100.0. All the vertices between the near and far plane 
	and inside the frustum will be rendered.

	Whenever the near value of your perspective matrix is set too high (like 10.0), OpenGL will clip all coordinates 
	close to the camera (between 0.0 and 10.0), which can give a visual result you maybe have seen before in videogames 
	where you could see through certain objects when moving uncomfortably close to them

	For Perspective x, y, z is divided by w . So for higher 'w', object will lokk smaller i.e. far.
	Each component of the vertex coordinate is divided by its w component giving smaller vertex coordinates 
	the further away a vertex is from the viewer. This is another reason why the w component is important, since 
	it helps us with perspective projection.
	*/

	// glm::mat4 proj = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
	projMatrix = glm::perspective(FOV, aspect, nearP, farP);  // Create our perspective projection matrix  

	// Update above projMatrix in GPU
	UpdateProjMatrix();
	   	
	return true;
}

// Sets the Pixel of the View port window. For Screen or the Document.
void CcGPUCtrlIntrFc::SetViewPort(int pxlStaW, int pxlStaH, int pxlEndW, int pxlEndH)
{
	viewPortW = pxlEndW - pxlStaW;
	viewPortH = pxlEndH - pxlStaH;

	glViewport(pxlStaW, pxlStaH, pxlEndW, pxlEndH);
}



// range W & H are in Eng. Units as Orth will map Eng. Units to Pixels (Set in the ViewPort)
bool CcGPUCtrlIntrFc::SetProjMatrixOrtho(float rangeW, float rangeH, float rangeD)
{
	float rangeLR = rangeW;// *0.5;	// Range -Left, Right
	float rangeTB = rangeH;// *0.5;	// Range Top, Bottom
	float rangeNF = rangeD;// *0.5;	// Range Near, Far

	// create transformations
	projMatrix = glm::mat4(1.0f);

	//							L		R		B			T		Near		Far
	projMatrix = glm::ortho(-rangeLR, rangeLR, -rangeTB,  rangeTB, -rangeNF, rangeNF);

	// Update above projMatrix in GPU
	UpdateProjMatrix();

	return true;
}


// Update projMatrix in GPU (Called fromOrth & Perspective)
bool CcGPUCtrlIntrFc::UpdateProjMatrix()
{
	unsigned int projLocL;
	GLuint i, shdPrgIDL;

	for (i = 0; i < INDX_SHD_PRG_MAX; i++) {
		shdPrgIDL = shdPrgIdAry[i];

	//	gl_Position = {projVs} * viewVs * modelVs * vec4(inPos, 1.0);
		glUseProgram(shdPrgIDL);
		projLocL = glGetUniformLocation(shdPrgIDL, "projVs");
		glUniformMatrix4fv(projLocL, 1, GL_FALSE, glm::value_ptr(projMatrix));
	}


	for (i = 0; i < INDX_SHD_PRG_COL_MAX; i++) {
		shdPrgIDL = shdPrgIdAryLgt[i];

		//	gl_Position = {projVs} * viewVs * modelVs * vec4(inPos, 1.0);
		glUseProgram(shdPrgIDL);
		projLocL = glGetUniformLocation(shdPrgIDL, "projVs");
		glUniformMatrix4fv(projLocL, 1, GL_FALSE, glm::value_ptr(projMatrix));
	}


	return true;
}


// ***  Old Not used !!! *** ,  just for study

// (1) P updated in projection, UPDPRG
// note:since the projection matrix rarely changes,
// i.e. remains the same for the Single scene. So we set it once 
bool CcGPUCtrlIntrFc::ChangeSizeOrtho(int x, int y, int dwW, int dwH, float nRangeL, float nRangeZL)
{
	// x, y Start position of the VP. And  dwW, dwH are widthh and height of VP.
	glViewport(x, y, dwW, dwH);
	viewPortW = dwW;
	viewPortH = dwH;

	// create transformations
	projMatrix = glm::mat4(1.0f);


	// set Projectin Matrix
	// projMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	if (dwW <= dwH)
	{
		projMatrix = glm::ortho(-nRangeL, nRangeL, -nRangeL * dwH / dwW, nRangeL * dwH / dwW, -nRangeZL, nRangeZL);
	}
	else
	{
		projMatrix = glm::ortho(-nRangeL * dwW / dwH, nRangeL * dwW / dwH, -nRangeL, nRangeL, -nRangeZL, nRangeZL);
	}

	UpdateProjMatrix();

	return true;
}


