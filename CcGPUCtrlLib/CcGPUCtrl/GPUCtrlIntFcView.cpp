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

// #include "Camera.h"

#include "CcGPUCtrlIntrFc.h"



// (2) V updated in Camera, UPDPRG
// Set Camera for view.
void CcGPUCtrlIntrFc::UpdCameraOrtho()
{


	// For Camera , we need to update only the viewVS in Vertex shader.
	G3DVector camTransL = camTransC;
	G3DVector camScaleL = camScaleC;
	G3DVector camRotL = camRotationC;

	// Define and init with identity matrix
	glm::mat4 viewMatrixL = glm::mat4(1.0f);

	float rotXL, rotYL, rotZL;
	rotXL = (float)camRotL.x;
	rotYL = (float)camRotL.y;
	rotZL = (float)camRotL.z;


	// Move Camera in opposite direction of scene
//	camTransL *= -1.0;
	camTransL.x = -camTransL.x; camTransL.y = -camTransL.y; camTransL.z = -camTransL.z;

	// Rotate the scene
	viewMatrixL = glm::rotate(viewMatrixL, glm::radians(rotXL), glm::vec3(1.0, 0.0, 0.0));
	viewMatrixL = glm::rotate(viewMatrixL, glm::radians(rotYL), glm::vec3(0.0, 1.0, 0.0));
	viewMatrixL = glm::rotate(viewMatrixL, glm::radians(rotZL), glm::vec3(0.0, 0.0, 1.0));

	// Scale
	viewMatrixL = glm::scale(viewMatrixL, glm::vec3(camScaleL.x, camScaleL.y, camScaleL.z));

	// At last move the scene
	viewMatrixL = glm::translate(viewMatrixL, glm::vec3(camTransL.x, camTransL.y, camTransL.z));

	viewMatrix = viewMatrixL;

	UpdateViewMatrix();
}

bool CcGPUCtrlIntrFc::UpdateViewMatrix()
{
	int viewLocL;
	GLuint i, shdPrgIDL;

	for (i = 0; i < INDX_SHD_PRG_MAX; i++) {
		shdPrgIDL = shdPrgIdAry[i];

	//	gl_Position = projVs * {viewVs} * modelVs * vec4(inPos, 1.0);
		glUseProgram(shdPrgIDL);
		viewLocL = glGetUniformLocation(shdPrgIDL, "viewVs");
		glUniformMatrix4fv(viewLocL, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Srt Camera position
		viewLocL = glGetUniformLocation(shdPrgIDL, "viewPos");
		if (viewLocL > -1) {
			glUniform3f(viewLocL, camTransC.x, camTransC.y, camTransC.z);
		}
	}


	for (i = 0; i < INDX_SHD_PRG_COL_MAX; i++) {
		shdPrgIDL = shdPrgIdAryLgt[i];
		glUseProgram(shdPrgIDL);
		viewLocL = glGetUniformLocation(shdPrgIDL, "viewVs");
		glUniformMatrix4fv(viewLocL, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Srt Camera position
		viewLocL = glGetUniformLocation(shdPrgIDL, "viewPosU");
		if (viewLocL > -1) {
			glUniform3f(viewLocL, (float) camTransC.x, (float) camTransC.y, (float) camTransC.z);
		}
	}

	return true;
}

/*
glm::mat4  CcGPUCtrlIntrFc::GetCameraViewMatrix() {

	return 	rCameraC->GetViewMatrix();
}

void  CcGPUCtrlIntrFc::UpdViewMatrixFmCamera() {

	viewMatrix = rCameraC->GetViewMatrix();
}

*/