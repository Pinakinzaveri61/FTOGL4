/*
 * FTOGL4-MS19 - Main program to test FTGL-OGL4 lib
 *
 * Copyright (c) 2021 Pinakin Zaveri <Pinakin.Zaveri@rediff.com>
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


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "FTGL/ftgl.h"

#include <iostream>

#include "CcGPUCtrlIntrFc.h"

CcGPUCtrlIntrFc* rGPUIntrFcG;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void CreOneDefaultLight();
void SetDefaultMaterial();
void TestShp();
void TestShpTxr();

// settings
const unsigned int SCR_WIDTH = 500;
const unsigned int SCR_HEIGHT = 200;


int main()
{
	char textAryStrL[256];
	std::string ccTextL;
	std::string fontFilePathL;
	const char* font_ChFilePathL;	// Char. font file with path

	unsigned int faceSizeNumL = 10;
	float rotY = 0.0;
	unsigned int txrIDL = 0;

	int VaoL, VboL;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FTOGL4-Sample", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	G3DColorVector backGndClrL;

	rGPUIntrFcG = new CcGPUCtrlIntrFc();

	rGPUIntrFcG->OpenGLDrawInit(backGndClrL);
	rGPUIntrFcG->CreGeneralShaderFmFile();
	rGPUIntrFcG->glblIlumin = backGndClrL;
	CreOneDefaultLight();
	SetDefaultMaterial();

	glfwSetWindowSize(window, 800, 600);

	rGPUIntrFcG->CcLoadIdentity(MATRIX_ID_VIEW);
	rGPUIntrFcG->camTransC.SetV(0.0, 0.0, 0.0);
	rGPUIntrFcG->UpdateViewMatrix();

	rGPUIntrFcG->CcLoadIdentity(MATRIX_ID_MODEL);
	rGPUIntrFcG->UpdModelGpuUniformVal();

	// Font for MS Window.
	fontFilePathL = "C:\\WINDOWS\\Fonts\\arial.ttf";
	font_ChFilePathL = fontFilePathL.c_str();

	bool useVBOFL = false;

	// Test ***
	bool useTexrFL = true;
	bool DrawFontFL = true;

	FTFont* rFTFontPolyL = NULL;
	FTFont* rFTFontOtLnL = NULL;
	FTFont* rFTFontExtuL = NULL;


	std::string txrFileL = ".\\..\\Images\\Flower.jpg";
	if (useTexrFL) rGPUIntrFcG->LoadTextureFileGPU(txrFileL, &txrIDL);

	if (DrawFontFL)
	{
		rFTFontPolyL = new FTGLPolygonFont(font_ChFilePathL);	// (Polygon fonts, good for rotate and scale) pz 2020
		rFTFontPolyL->rGPUIntrFcFontC = rGPUIntrFcG;
		rFTFontPolyL->UseDisplayList(useVBOFL);	// Use of Display list should make the draw faster. 
		rFTFontPolyL->UseTexture(useTexrFL);
		rFTFontPolyL->Depth(2.0);
		rFTFontPolyL->FaceSize(faceSizeNumL);

		rFTFontOtLnL = new FTOutlineFont(font_ChFilePathL);	// (Polygon fonts, good for rotate and scale) pz 2020
		rFTFontOtLnL->rGPUIntrFcFontC = rGPUIntrFcG;
		rFTFontOtLnL->UseDisplayList(useVBOFL);	// Use of Display list should make the draw faster. 
		rFTFontOtLnL->Depth(2.0);
		rFTFontOtLnL->FaceSize(faceSizeNumL);	// We need it for printing ! pz 2020

		rFTFontExtuL = new FTExtrudeFont(font_ChFilePathL);	// (Polygon fonts, good for rotate and scale) pz 2020
		rFTFontExtuL->rGPUIntrFcFontC = rGPUIntrFcG;
		rFTFontExtuL->UseDisplayList(useVBOFL);	// Use of Display list should make the draw faster. 
		rFTFontExtuL->UseTexture(useTexrFL);
		rFTFontExtuL->UseLight(false);
		rFTFontExtuL->Depth(2.0);
		rFTFontExtuL->FaceSize(faceSizeNumL);	// We need it for printing ! pz 2020


		// For texture use (Note: Not working )
	//	FTFont* rFTFontL = new FTTextureFont(font_ChFilePathL);	// (Polygon fonts, good for rotate and scale) pz 2020
	}

	glLineWidth(3.0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rGPUIntrFcG->CcLoadIdentity(MATRIX_ID_MODEL);	// ** ModelMatrix = Unit on each fresh draw.

		rGPUIntrFcG->CcSetUniformColor(0.9, 0.9, 0.1, 1.0);
		glLineWidth(3.0);

		rGPUIntrFcG->UpdModelGpuUniformVal();	// Upload the Transform Matrix


		//  ***
		if (DrawFontFL)
		{
			ccTextL = "FTOGL4-Poygon";
			sprintf(textAryStrL, "%s", ccTextL.c_str());
			rGPUIntrFcG->CcSetTexture(txrIDL);

			rGPUIntrFcG->CcScale(0.5, 0.5, 1.0);	// Makes Ht. 1 unit, on (0 - 10) size
	//		rGPUIntrFcG->CcRotate(30.0, MATRIX_ROTATE_Z);
	//		rGPUIntrFcG->CcRotate(rotY, MATRIX_ROTATE_Y);

			rGPUIntrFcG->CcTranslate(-38.0, 10.0, 0.0);
			rGPUIntrFcG->UpdModelGpuUniformVal();	// Upload the Transform Matrix
			rFTFontPolyL->Render(textAryStrL);

			rGPUIntrFcG->CcTranslate(0.0, -15.0, 0.0);
			rGPUIntrFcG->UpdModelGpuUniformVal();	// Upload the Transform Matrix
			ccTextL = "FTOGL4-Outline";
			sprintf(textAryStrL, "%s", ccTextL.c_str());
			rGPUIntrFcG->CcSetTexture(txrIDL);
			rFTFontOtLnL->Render(textAryStrL);

			rotY = 15.0;
			rGPUIntrFcG->CcTranslate(5.0, -15.0, 0.0);
			rGPUIntrFcG->CcRotate(30.0, MATRIX_ROTATE_X);
			rGPUIntrFcG->CcRotate(20.0, MATRIX_ROTATE_Z);
			rGPUIntrFcG->CcRotate(rotY, MATRIX_ROTATE_Y);
			rGPUIntrFcG->UpdModelGpuUniformVal();	// Upload the Transform Matrix
			ccTextL = "FTOGL4-Extrude";
			sprintf(textAryStrL, "%s", ccTextL.c_str());
			rGPUIntrFcG->CcSetTexture(txrIDL);
			rFTFontExtuL->Render(textAryStrL);
		}
		else
		{
			if (useTexrFL)
			{
				TestShpTxr();
			}
			else
			{
				TestShp();
			}

		}

		//		rotY = rotY + 0.2;

				// glfw: swap buffers
		glfwSwapBuffers(window);

		// check for keys pressed/released, mouse moved etc.
		glfwPollEvents();
	}

	if (DrawFontFL)
	{
		delete rFTFontPolyL;
		delete rFTFontOtLnL;
		delete rFTFontExtuL;
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int dwW, int dwH)
{
	float nRangeL = 10.0;
	float nRangeZL = 100.0;

	rGPUIntrFcG->SetViewPort(0, 0, dwW, dwH);


	if (dwW <= dwH)
	{
		rGPUIntrFcG->SetProjMatrixOrtho(nRangeL, nRangeL * dwH / dwW, nRangeZL);
	}
	else
	{
		rGPUIntrFcG->SetProjMatrixOrtho(nRangeL * dwW / dwH, nRangeL, nRangeZL);

	}
}


void TestShp()
{
	G3DVector pt1, tx1;

	rGPUIntrFcG->BeginSetTriVrtxAry(GL_TRIANGLES);

	pt1.SetV(-5.0, -5.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAry2D(pt1);

	pt1.SetV(5.0, -5.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAry2D(pt1);

	pt1.SetV(0.0, 5.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAry2D(pt1);

	rGPUIntrFcG->DrawOnTheFlyTriVrtxAry2D();

}

void TestShpTxr()
{
	G3DVector pt1, tx1;

	rGPUIntrFcG->BeginSetTriVrtxAry(GL_TRIANGLES);

	pt1.SetV(-5.0, -5.0, 0.0);
	tx1.SetV(0.0, 0.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAryTxr2D(pt1, tx1);

	pt1.SetV(5.0, -5.0, 0.0);
	tx1.SetV(1.0, 0.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAryTxr2D(pt1, tx1);

	pt1.SetV(0.0, 5.0, 0.0);
	tx1.SetV(0.5, 1.0, 0.0);
	rGPUIntrFcG->AddInTriVrtxAryTxr2D(pt1, tx1);

	rGPUIntrFcG->DrawOnTheFlyTriVrtxAryTxr2D();

}

//One default light (Property & Pos.)  to lit any 3D shape.
void CreOneDefaultLight()
{
	G3DVector shpPositionL;
	G3DVector directionL;

	//	shpPositionL.SetV(5.0, 2.0, -2.0);
	shpPositionL.SetV(-3.0, -2.0, 50.0);
	directionL.SetV(3.0, 5.0, -45.0);

	//	directionL.SetV(-3.0, -2.0, 50.0);


//	GsLightProp* rGsLightPropL;

	// Create One default light Property in LL
//	AddLightProp(rScMainsInfo);

	// And one minimum light LightShaderP6L10
	rGPUIntrFcG->CreLightShaderFmFile(SHD_PRG_INDX_ROW_LGT_P6L10);

	// Send one default light Property to above shader
//	rGsLightPropL = new GsLightProp();

	rGPUIntrFcG->ltType = LT_TYPE_DIRECTIONAL;
	rGPUIntrFcG->ambient.SetColor(0.1, 0.2, 0.1, 1.0);
	rGPUIntrFcG->diffuse.SetColor(0.6, 0.8, 0.4, 1.0);
	rGPUIntrFcG->specular.SetColor(0.6, 0.8, 0.4, 1.0);


	rGPUIntrFcG->constant = 0.001;
	rGPUIntrFcG->linear = 0.001;
	rGPUIntrFcG->quadratic = 0.0001;

	rGPUIntrFcG->cutOff = 1.5;
	rGPUIntrFcG->outerCutOff = 2.5;

	rGPUIntrFcG->UploadLgtProp(SHD_PRG_INDX_COL_MAT_COLOR, 0); // Light Prop. At index '0'
	rGPUIntrFcG->UploadLgtProp(SHD_PRG_INDX_COL_MAT_COLOR_TXR, 0); // Light Prop. At index '0'
//	rGPUIntrFcG->UploadLgtProp(SHD_PRG_INDX_COL_MAT_MAP, 0); // Light Prop. At index '0'

	// Send one default light Def. to above shader
	rGPUIntrFcG->indxToLtProp = 0;	// shpDef = index of lgtPropAry 'Above'
	rGPUIntrFcG->position = shpPositionL;
	rGPUIntrFcG->direction = directionL;

	// Aminimum shader.
	rGPUIntrFcG->UploadLgtDef(SHD_PRG_INDX_COL_MAT_COLOR, 0); // Lgt Pos. also at '0'
	rGPUIntrFcG->UploadLgtDef(SHD_PRG_INDX_COL_MAT_COLOR_TXR, 0); // Lgt Pos. also at '0'


	rGPUIntrFcG->numOfLights = 1;
	rGPUIntrFcG->txrType = TEXTURE_NO;	// By default no texture.
	rGPUIntrFcG->alpha = 0.1;
	rGPUIntrFcG->glblIlumin.SetColor(0.3, 0.1, 0.1, 1.0);
	rGPUIntrFcG->UploadLgtComn(SHD_PRG_INDX_COL_MAT_COLOR); // Lgt Pos. also at '0'

	rGPUIntrFcG->txrType = TEXTURE_ONLY;	// By default no texture.
	rGPUIntrFcG->UploadLgtComn(SHD_PRG_INDX_COL_MAT_COLOR_TXR); // Lgt Pos. also at '0'

}


//One default light (Property & Pos.)  to lit any 3D shape.
void SetDefaultMaterial()
{
	G3DColorVector matAmbieL; matAmbieL.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	G3DColorVector matDiffuL; matDiffuL.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	G3DColorVector matSpecuL; matSpecuL.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	float matShininessL = 100.0;

	glDisable(GL_BLEND);
	rGPUIntrFcG->alpha = 1.0;

	rGPUIntrFcG->CcUploadMatPropToGPU(matAmbieL, matDiffuL, matSpecuL, matShininessL);

}

