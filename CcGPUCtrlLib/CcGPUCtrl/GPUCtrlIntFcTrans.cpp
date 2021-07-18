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



//CcLoadIdentity(MATRIX_ID_MODEL);
// glLoadIdentity
void CcGPUCtrlIntrFc::CcLoadIdentity(short MatrixID)
{
	/*
	 initialize it to the identity matrix by initializing the matrix's diagonals to 1.0; 
	 if we do not initialize it to the identity matrix the matrix would be a null matrix (all elements 0) 
	 and all subsequent matrix operations would end up a null matrix as well.
	*/
	switch (MatrixID)
	{
	case MATRIX_ID_PROJ:
		projMatrix = glm::mat4(1.0f);
		break;

	case MATRIX_ID_VIEW:
		viewMatrix = glm::mat4(1.0f);
		break;

	case MATRIX_ID_MODEL:
		modelMatrix = glm::mat4(1.0f);
		break;

	default:
		break;
	}

}



void CcGPUCtrlIntrFc::CcTranslate(float xTrP, float yTrP, float zTrP)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(xTrP, yTrP, zTrP));

}


// Rotation angle and the Axis 1: x. 2: y, 3:z 
// rotAngP, passed in Degree
void CcGPUCtrlIntrFc::CcRotate(float rotAngP, short axisRotP)
{
	switch (axisRotP)
	{
	case MATRIX_ROTATE_X:
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotAngP), glm::vec3(1.0, 0.0, 0.0));
		break;

	case MATRIX_ROTATE_Y:
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotAngP), glm::vec3(0.0, 1.0, 0.0));
		break;

	case MATRIX_ROTATE_Z:
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotAngP), glm::vec3(0.0, 0.0, 1.0));
		break;

	default:
		break;
	}

}



void CcGPUCtrlIntrFc::CcScale(float xSclP, float ySclP, float zSclP)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(xSclP, ySclP, zSclP));
}


bool CcGPUCtrlIntrFc::CcPushModelMtrx()
{
	bool retFL = false;

	if (modelMtrxAryIndx < PUSH_POP_MTRX_ARY_MAX)
	{
		// Save the Matrix on top (Last)
		modelMtrxAryPP[modelMtrxAryIndx] = modelMatrix;
		modelMtrxAryIndx++;
		retFL = true;
	}

	return retFL;
}


bool CcGPUCtrlIntrFc::CcPopModelMtrx()
{
	bool retFL = false;

	if (modelMtrxAryIndx > 0)
	{
		// Save the Matrix on top (Last)
		modelMtrxAryIndx--;
		modelMatrix = modelMtrxAryPP[modelMtrxAryIndx];
		retFL = true;

		// Wea also need to update GPU - Model uniform Value
		UpdModelGpuUniformVal();
	}

	return retFL;
}

// (3) Model in Trans, UPDPRG
// Update Transform Matrix for the Scene shape Models
bool  CcGPUCtrlIntrFc::UpdModelGpuUniformVal()
{
	unsigned int modelMtrxLocL;
	bool retFL = false;
	GLuint i, shdPrgIDL;

	for (i = 0; i < INDX_SHD_PRG_MAX; i++) {
		shdPrgIDL = shdPrgIdAry[i];

		// gl_Position = projVs * viewVs * {modelVs} * vec4(inPos, 1.0);

		glUseProgram(shdPrgIDL);
		modelMtrxLocL = glGetUniformLocation(shdPrgIDL, "modelVs");
		glUniformMatrix4fv(modelMtrxLocL, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}


		for (i = 0; i < INDX_SHD_PRG_COL_MAX; i++) {
		shdPrgIDL = shdPrgIdAryLgt[i];

		glUseProgram(shdPrgIDL);
		modelMtrxLocL = glGetUniformLocation(shdPrgIDL, "modelVs");
		glUniformMatrix4fv(modelMtrxLocL, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}


	return retFL;
}



void CcGPUCtrlIntrFc::CcTranslateAndUpdGPU(float xTrP, float yTrP, float zTrP)
{
	CcTranslate(xTrP, yTrP, zTrP);
	UpdModelGpuUniformVal();

}
