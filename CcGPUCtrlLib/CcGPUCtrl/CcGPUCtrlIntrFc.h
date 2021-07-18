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



#pragma once

#include <glad/glad.h>

#include <iostream>
#include <cstring>
#include <cmath>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "G3DVector.h"

//#include "AssImp_Model.h"


#include <vector>

#define MATRIX_ROTATE_X 1
#define MATRIX_ROTATE_Y 2
#define MATRIX_ROTATE_Z 3

#define MATRIX_ID_PROJ 1
#define MATRIX_ID_VIEW 2
#define MATRIX_ID_MODEL 3

#define ELE_BEGINE					1
#define ELE_TYPE_TRINGLE			2
#define ELE_TYPE_QUAD				3
#define ELE_TYPE_POLYGO				4
#define ELE_ADD_VERTEX				5
#define ELE_END						6
#define ELE_END_SHAPE				7	

#define PUSH_POP_MTRX_ARY_MAX   20


// Following all Shaders are created for 2D shapes. (No light effect)
#define SHD_PRG_INDX_COL					0		// Simple Color
#define SHD_PRG_INDX_COL_TXR				1		// Simple Color + Texture
#define SHD_PRG_INDX_SELECT					2		// Select
#define INDX_SHD_PRG_MAX					3 // Last Indx for Loop


// ROW (One of the following number of lights shader created. To save GPU memory 
// e.g. if only 10 lights in the kitchen then why create 100 lights shader)
// So only one of the following light combination used to create shader for the Column
#define SHD_PRG_INDX_ROW_LGT_P6L10		0
#define SHD_PRG_INDX_ROW_LGT_P10L20		1
#define INDX_SHD_PRG_ROW_MAX			2 // Last Indx for Loop


// Column (Create all the following shaders, for above one of the light combination as per scene.)
#define SHD_PRG_INDX_COL_MAT_COLOR				0		// Material Color
#define SHD_PRG_INDX_COL_MAT_COLOR_TXR			1		// Material Color + Texture
#define INDX_SHD_PRG_COL_MAX				2		// Now, Coolor and Txr.


// Light Type (Move to 3D).
#define LT_TYPE_DIRECTIONAL		1	// Infinite
#define LT_TYPE_POSITIONAL		2
#define LT_TYPE_SPOT			3
#define LT_TYPE_ENVIRONMENT		4

// Texture Type
#define TEXTURE_NO				0	// No Texture
#define TEXTURE_ONLY			1	// Decal, Only Texture applied. Often used with reflection mapping. And multi-pass redering
#define TEXTURE_PLUS_MAT		2	// Blend, Texture Plus Material color (Can be blended with amount of each)
#define TEXTURE_MUL_MAT			3	// Modulate, Texture Multiply with Material color

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif


struct CcVertexP {
	// position
	glm::vec3 Position;
};


struct glmPosTxr {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};


struct CcVertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
};


struct CcVertexTxr {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};


struct CcVertexTan {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};


class CcGPUCtrlIntrFc
{
public:

	CcGPUCtrlIntrFc();
	~CcGPUCtrlIntrFc();
	
	// Camera
	G3DVector camTransC;
	G3DVector camScaleC;
	G3DVector camRotationC;

	bool SelectF; // True: For Selection logic.
	


	int actLgtShdr;	// Current active light shaders (Depending on Number of lights.)


	// For better control (May be), we maintain two shader index array. One for 2D shapes and other for 3D shapes needs ligting.

	// Array for General Shaders
	GLuint shdPrgIdAry[INDX_SHD_PRG_MAX]; //Shader Program ID arraty

	// Row for multiple shader for lights. 
	// Column for Colour, Texture, Map 
//	GLuint shdPrgIdAryLgt[INDX_SHD_PRG_ROW_MAX][INDX_SHD_PRG_COL_MAX]; //Shader Program ID arraty
	// We don't need above array as we plan to delete current scene light shader and then create new scene light shader

	// Array of  current scene light shader
	GLuint shdPrgIdAryLgt[INDX_SHD_PRG_COL_MAX]; //Shader Program ID arraty

	GLuint viewPortW, viewPortH;

	GLint uColor;     //Shader color input
	GLint uLightPosition;//Shader light position input
	GLint mvIndex;    //Shader positioning input
	GLint projIndex;     //Shader projection input
//	mat4 p, mv;       //Local projection and positioning variables

	GLfloat rotY = 0;    //rotate model around y axis
	GLfloat rotX = 0;    //rotate model around x axis

	glm::mat4 projMatrix; // Store the projection matrix  
	glm::mat4 viewMatrix; // Store the view matrix  
	glm::mat4 modelMatrix; // Store the model matrix  
	// glm::mat4 transformMatrix; // Store the ???

	glm::mat4 modelMtrxAryPP[PUSH_POP_MTRX_ARY_MAX]; // Store the model matrix  for Push / Pop Op.
	short modelMtrxAryIndx;

	// Material
	float alpha;  // for Blending
	G3DColorVector glblIlumin;


	// Light Property
	int ltType;			// Directional, Position, Spot
	G3DColorVector ambient;
	G3DColorVector diffuse;
	G3DColorVector specular;

	// For Point + Spot
	float constant;
	float linear;
	float quadratic;
	// For Spot, Angles in Deg. (0 to 180, more than it will not create spot effect.)
	float cutOff;
	float outerCutOff;

	// Light Shape Pos & Dir
	int  indxToLtProp;
	G3DVector position;
	G3DVector direction;

	unsigned int numOfLights;
	unsigned int txrType;

	short eleTypeC;
	short vrtxCount;
	unsigned int lastVrtxIndx2, lastVrtxIndx1;


	void UploadLgtComn(GLuint shdPrgIndxP);
	void UploadLgtProp(GLuint shdPrgIndxP, unsigned int lgtNum);
	void UploadLgtDef(GLuint shdPrgIndxP, unsigned int lgtNum);


	void CreGeneralShaderFmFile();
	void CreLightShaderFmFile(int lgtShdrType);


	static char* readShaderSource(const char* shaderFileP);
	unsigned int InitShaderFile(const char* vShaderFile, const char* fShaderFile);
	unsigned int InitShaderIn();

	void OpenGLDrawInit(G3DColorVector backGndClrP);

	bool LoadTextureFileGPU(std::string, unsigned int* textureIDL);

//	bool LoadChars();
//	bool CreateVBO();
//	bool AddChar(char chrP);
//	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

	void SetMatBlendProp(float tranValP, int blendModeP);
	void CcUploadMatPropToGPU(G3DColorVector matAmbieP, G3DColorVector matDiffuP, G3DColorVector matSpecuP, float matShininessP);
	void CcUploadMatPropToShader(GLuint shdPrgIDP, G3DColorVector matAmbieP, G3DColorVector matDiffuP, G3DColorVector matSpecuP, float matShininessP);


	unsigned int indxSize;
	unsigned int VBO, EBO, VAO;

	std::vector<CcVertexP> cVerticesP;

	std::vector<CcVertex> cVertices;
//	std::vector<unsigned int> cIndices;
	std::vector<CcVertexTxr> cVerticesTxr;
	std::vector <glmPosTxr> vecGlmPosTxr;

	void BeginSetTriVrtxAry3D(short eleTypeP);
	void AddInTriVrtxAry3D(G3DVector vrtxP, G3DVector norP);
	void UploadTriVrtxAry3D();
	void DrawTriVrtxAry3D();
	void ClearTriVrtxAry3D();
	void DeleteTriVrtxAry3D();
	void DrawOnTheFlyTriVrtxAry3D();

	// Texture
	void AddInTriVrtxAryTxr3D(G3DVector vrtxP, G3DVector norP, G3DVector txrCorP);
	void UploadTriVrtxAryTxr3D();
	void DrawTriVrtxAryTxr3D();
	void DrawTriVrtxAryTxr3D(int txrTypeP);
	void ClearTriVrtxAryTxr3D();
	void DrawOnTheFlyTriVrtxAryTxr3D();

	unsigned int fbo;
	unsigned int rbo;
	unsigned int fb, color_rb, depth_rb;


	// Transform

	void CcTranslate(float xTrP, float yTrP, float zTrP);
	void CcRotate(float rotAngP, short axisRotP);
	void CcScale(float xSclP, float ySclP, float zSclP);
	//void CcPushMatrix();
	//void CcPopMatrix();
	void CcLoadIdentity(short MatrixID);
//	void CcSendTransFormToGPU();

	void CcTranslateAndUpdGPU(float xTrP, float yTrP, float zTrP);

	// Update GPU
	bool  UpdModelGpuUniformVal();
	void CcSetUniformColor(float r, float g, float b, float a);
	void CcSetUniformColor(G3DColorVector ourColorP);

	bool ChangeSizeOrtho(int x, int y, int dwW, int dwH, float nRangeL, float nRangeZL);

	void SetViewPort(int pxlStaW, int pxlStaH, int pxlEndW, int pxlEndH);
	bool SetProjMatrixOrtho(float rangeW, float rangeH, float rangeD);
	bool UpdateProjMatrix();
	bool SetProjMatrixPerspective(float FOV, float aspect, float nearP, float farP);
	bool UpdateViewMatrix();

	// Push / Pop
	bool CcPushModelMtrx();
	bool CcPopModelMtrx();

	// Camera
	void UpdCameraOrtho();


	void DrawOnTheFlyLineStripAry();
	void UploadLineStripAry();
	void DrawLineStripAry();
	void DeleteLineStripAry();



	void getFaceUnitNormal(float* rVN, float vertx1[3], float vertx2[3], float vertx3[3]);

	void CcSetTexture(unsigned int texrIDP);

//	uofrGraphics urgl;
		
	/*
			To draw List of Tringles (LOT)
	*/
	// memeber vars
	float radius;
	int sectorCount;                        // longitude, # of slices
	int stackCount;                         // latitude, # of stacks
	bool smooth;
	unsigned int indc;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;

	// interleaved
	std::vector<float> interleavedVertices;
	int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)


	// 2D Function.
	void BeginSetTriVrtxAry(short eleTypeP); // ? Remove 2D
	void ClearTriVrtxCount();

	void AddInTriVrtxAry2D(G3DVector vrtxP);
	void DrawOnTheFlyTriVrtxAry2D();
	void ClearTriVrtxAry2D();
	void UploadTriVrtxAry2D();
	void DrawTriVrtxAry2D();
	void DeleteTriVrtxAry2D();

	// 2D Texture Functions.
	void AddInTriVrtxAryTxr2D(G3DVector vrtxP, G3DVector txrCorP);
	void DrawOnTheFlyTriVrtxAryTxr2D();
	void UploadTriVrtxAryTxr2D();
	void DrawTriVrtxAryTxr2D();
	void ClearTriVrtxAryTxr2D();
	void DeleteTriVrtxAryTxr2D();

//	void DrawVecAry();

	// Tesslate support 
	G3DVector vrtxAry[4];
	unsigned int vrtxAryIndx;


	void LinkListLineStripBuild(G3DVector vrtxP);
	void DrawLineStripLL();
	void ClearLineStripAry();

	void CcDrawRect(G3DVector cv1, G3DVector cv2, G3DVector cv3, G3DVector cv4);
	void CcDrawRectTxr(G3DVector cv1, G3DVector cv2, G3DVector cv3, G3DVector cv4);
	void CcDrawRectClrTxr();
	void CcDrawRectTxrUV(G3DVector cv1, G3DVector cv2, G3DVector cv3, G3DVector cv4,
		float UV0x, float UV0y, float UV1x, float UV1y);


	//  std::vector  used to create array of glm::vec3
	std::vector<glm::vec3> varray;
	std::vector<float> darray; //  darray(varray.size(), 0.0f);


	bool patnFstTimeF;
	unsigned int textureIDC;

};