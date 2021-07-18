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



void CcGPUCtrlIntrFc::BeginSetTriVrtxAry3D(short eleTypeP) {

	eleTypeC = eleTypeP;

}


void CcGPUCtrlIntrFc::AddInTriVrtxAry3D(G3DVector vrtxP, G3DVector norP) {

	CcVertex vertexL;
	glm::vec3 vectorL; // we declare a placeholder vector since ConsCAD uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

	// positions
	vectorL.x = (float)vrtxP.x; vectorL.y = (float)vrtxP.y; vectorL.z = (float)vrtxP.z;
	vertexL.Position = vectorL;

	// Normal
	vectorL.x = (float)norP.x; vectorL.y = (float)norP.y; vectorL.z = (float)norP.z;
	vertexL.Normal = vectorL;

	

	if (eleTypeC == GL_TRIANGLES)
	{
		// ***  Add the Vertex  // Push vertex
		cVertices.push_back(vertexL);
		indices.push_back(indc);
		indc++;

	}
	else if (eleTypeC == GL_TRIANGLE_STRIP)
	{
		// ***-  Add the Vertex 	// Push vertex
		cVertices.push_back(vertexL);

		// Trick to form the Tingle using indices indicating above Vertex.
		if (vrtxCount == 0)
		{
			lastVrtxIndx2 = indc;	// For Fan ,this is pointing at Vrtx0.
			//indices.push_back(indc);
			indc++;

			vrtxCount++;

		}
		else if (vrtxCount < 2)
		{
			lastVrtxIndx1 = indc;
			//indices.push_back(indc);
			indc++;
			vrtxCount++;
			patnFstTimeF = true;
		}
		else
		{

			// Form a Tringle of the past 3 vertices.
			if (patnFstTimeF)
			{
				indices.push_back(lastVrtxIndx2); indices.push_back(lastVrtxIndx1); indices.push_back(indc);
				patnFstTimeF = false;
			}
			else
			{
				indices.push_back(lastVrtxIndx1); indices.push_back(lastVrtxIndx2); indices.push_back(indc);
				patnFstTimeF = true;
			}


			lastVrtxIndx2 = lastVrtxIndx1;
			lastVrtxIndx1 = indc;
			indc++;

		}

	}
	else if (eleTypeC == GL_TRIANGLE_FAN)
	{
		// *** Add the Vertex 	// Push vertex
		cVertices.push_back(vertexL);

		// Trick to form the Tingle using indices indicating above Vertex.
		if (vrtxCount == 0)
		{
			lastVrtxIndx2 = indc;	// For Fan ,this is pointing at Vrtx0.
			//indices.push_back(indc);
			indc++;

			vrtxCount++;

		}
		else if (vrtxCount < 2)
		{
			lastVrtxIndx1 = indc;
			//indices.push_back(indc);
			indc++;
			vrtxCount++;
		}
		else
		{
			// Form a tringle of '0' and past two vertices
			indices.push_back(lastVrtxIndx2); indices.push_back(lastVrtxIndx1); indices.push_back(indc);
			lastVrtxIndx1 = indc;
			indc++;

		}

	}
	else
	{

	}


}

void CcGPUCtrlIntrFc::DrawOnTheFlyTriVrtxAry3D() {

	UploadTriVrtxAry3D();
	DrawTriVrtxAry3D();
	ClearTriVrtxAry3D();
	DeleteTriVrtxAry3D();
}


// We split Vertex upload and Draw. This will increae the speed. 

// Upload the Vertex to the GPU memory	UploadRectVrtxAry3D
void CcGPUCtrlIntrFc::UploadTriVrtxAry3D() {
	//	GLint attribVertex; // attribVertexPosition;
	//	GLint attribNormal;
	GLuint shdPrgIDL;
	//	GLint attribTxrCor;

	if (cVertices.size() == 0)
	{
		shdPrgIDL = 0;

		return;	// Return to calling program to check who is causing this error.
	}

		// VBO uses vertex Normal  for lighting . So we use column 'SHD_PRG_INDX_COL_MAT_COLOR'
	//	shdPrgIDL = shdPrgIdAryLgt[indxRowShdPrgLgt] [SHD_PRG_INDX_COL_MAT_COLOR];

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
		// UploadShpNumToRGB();  Taken care by application
	}
	else {
		shdPrgIDL = shdPrgIdAryLgt[SHD_PRG_INDX_COL_MAT_COLOR];
		glUseProgram(shdPrgIDL);
	}



	//	attribVertex = glGetAttribLocation(shdPrgIDL, "inPos");
	//	attribNormal = glGetAttribLocation(shdPrgIDL, "inNormal");
	//	attribTxrCor = glGetAttribLocation(shdPrgIDL, "inTexCoord");


		// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
//	glBufferData(GL_ARRAY_BUFFER, cVertices.size() * sizeof(CcVertex), &cVertices[0], GL_STATIC_DRAW);
	unsigned int vrtxSizeL = cVertices.size() * sizeof(CcVertex);
	glBufferData(GL_ARRAY_BUFFER, vrtxSizeL, cVertices.data(), GL_STATIC_DRAW);

	indxSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	unsigned int size2 = sizeof(CcVertex);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size2, (void*)0);


	// vertex normals
	unsigned int offSetNorL = offsetof(CcVertex, Normal);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size2, (void*)offSetNorL);

	glBindVertexArray(0);

}

//ClearRectVrtxAry3D
void CcGPUCtrlIntrFc::ClearTriVrtxAry3D() {
	indc = 0;
	vrtxCount = 0;

	std::vector<CcVertex >().swap(cVertices);
	std::vector<unsigned int>().swap(indices);
}


// Draw the above Vertex.   DrawRectVrtxAry3D
void CcGPUCtrlIntrFc::DrawTriVrtxAry3D() {

	//	int lgtCountLocL;
	GLuint shdPrgIDL;

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIDL = shdPrgIdAryLgt[SHD_PRG_INDX_COL_MAT_COLOR];
		glUseProgram(shdPrgIDL);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


// DeleteRectVrtxAry3D
void CcGPUCtrlIntrFc::DeleteTriVrtxAry3D() {

	// unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	const GLuint* delObjL;
	// We can delete VBO & EBO after 'glBindVertexArray(0);' But will not help in saving memory.
	// Because VBO memory is still alive untill we use 'glDeleteVertexArrays'
	// Only VBO & EBO index number is lost for h=that number to reuse. So we do it in last, when object is deleted.

	delObjL = &VBO;
	glDeleteBuffers(1, delObjL);

	delObjL = &EBO;
	glDeleteBuffers(1, delObjL);
	// glDeleteVertexArrays(GLsizei n, const GLuint * arrays);
	/*
	n - Specifies the number of vertex array objects to be deleted.
	arrays - Specifies the address of an array containing the n names of the objects to be deleted.
	*/
	delObjL = &VAO;
	glDeleteVertexArrays(1, delObjL);

}



//        ***   Texture		*****


void CcGPUCtrlIntrFc::AddInTriVrtxAryTxr3D(G3DVector vrtxP, G3DVector norP, G3DVector txrCorP) {

	CcVertexTxr vrtxTxrL;
	glm::vec3 vectorL; // we declare a placeholder vector since ConsCAD uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
	glm::vec2 TexCordL;

	// positions
	vectorL.x = (float)vrtxP.x; vectorL.y = (float)vrtxP.y; vectorL.z = (float)vrtxP.z;
	vrtxTxrL.Position = vectorL;

	// Normal
	vectorL.x = (float)norP.x; vectorL.y = (float)norP.y; vectorL.z = (float)norP.z;
	vrtxTxrL.Normal = vectorL;

	TexCordL.x = (float)txrCorP.x; TexCordL.y = (float)txrCorP.y;
	vrtxTxrL.TexCoords = TexCordL;


	if (eleTypeC == GL_TRIANGLES)
	{
		// ***  Add the Vertex  // Push vertex
		cVerticesTxr.push_back(vrtxTxrL);
		indices.push_back(indc);
		indc++;

	}
	else if (eleTypeC == GL_TRIANGLE_STRIP)
	{
		// ***-  Add the Vertex 	// Push vertex
		cVerticesTxr.push_back(vrtxTxrL);

		// Trick to form the Tingle using indices indicating above Vertex.
		if (vrtxCount == 0)
		{
			lastVrtxIndx2 = indc;	// For Fan ,this is pointing at Vrtx0.
			//indices.push_back(indc);
			indc++;

			vrtxCount++;

		}
		else if (vrtxCount < 2)
		{
			lastVrtxIndx1 = indc;
			//indices.push_back(indc);
			indc++;
			vrtxCount++;
			patnFstTimeF = true;
		}
		else
		{

			// Form a Tringle of the past 3 vertices.
			if (patnFstTimeF)
			{
				indices.push_back(lastVrtxIndx2); indices.push_back(lastVrtxIndx1); indices.push_back(indc);
				patnFstTimeF = false;
			}
			else
			{
				indices.push_back(lastVrtxIndx1); indices.push_back(lastVrtxIndx2); indices.push_back(indc);
				patnFstTimeF = true;
			}


			lastVrtxIndx2 = lastVrtxIndx1;
			lastVrtxIndx1 = indc;
			indc++;

		}

	}
	else if (eleTypeC == GL_TRIANGLE_FAN)
	{
		// *** Add the Vertex 	// Push vertex
		cVerticesTxr.push_back(vrtxTxrL);

		// Trick to form the Tingle using indices indicating above Vertex.
		if (vrtxCount == 0)
		{
			lastVrtxIndx2 = indc;	// For Fan ,this is pointing at Vrtx0.
			//indices.push_back(indc);
			indc++;

			vrtxCount++;

		}
		else if (vrtxCount < 2)
		{
			lastVrtxIndx1 = indc;
			//indices.push_back(indc);
			indc++;
			vrtxCount++;
		}
		else
		{
			// Form a tringle of '0' and past two vertices
			indices.push_back(lastVrtxIndx2); indices.push_back(lastVrtxIndx1); indices.push_back(indc);
			lastVrtxIndx1 = indc;
			indc++;
		}
	}
	else
	{

	}
}



void CcGPUCtrlIntrFc::DrawOnTheFlyTriVrtxAryTxr3D() {

	UploadTriVrtxAryTxr3D();
	DrawTriVrtxAryTxr3D();
	ClearTriVrtxAryTxr3D();
	DeleteTriVrtxAry3D();
}


void CcGPUCtrlIntrFc::UploadTriVrtxAryTxr3D() {
	//	GLint attribVertex; // attribVertexPosition;
		// GLint attribNormal;
	GLuint shdPrgIDL;
	// GLuint shdPrgIndxL;


	//	shdPrgIDL = shdPrgIdAryLgt[indxRowShdPrgLgt][SHD_PRG_INDX_COL_MAT_COLOR];

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIDL = shdPrgIdAryLgt[SHD_PRG_INDX_COL_MAT_COLOR_TXR];
		glUseProgram(shdPrgIDL);
	}

//	attribVertex = glGetAttribLocation(shdPrgIDL, "inPos");
//	attribNormal = glGetAttribLocation(shdPrgIDL, "inNormal");

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, cVerticesTxr.size() * sizeof(CcVertexTxr), &cVerticesTxr[0], GL_STATIC_DRAW);

	indxSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CcVertexTxr), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CcVertexTxr), (void*)offsetof(CcVertexTxr, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CcVertexTxr), (void*)offsetof(CcVertexTxr, TexCoords));

	/*
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	*/
	glBindVertexArray(0);
}

void CcGPUCtrlIntrFc::DrawTriVrtxAryTxr3D() {
	int lgtCountLocL;
	GLuint shdPrgIDL;
	// GLuint shdPrgIndxL;

	//	shdPrgIDL = shdPrgIdAryLgt[indxRowShdPrgLgt][SHD_PRG_INDX_COL_MAT_COLOR];
	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIDL = shdPrgIdAryLgt[SHD_PRG_INDX_COL_MAT_COLOR_TXR];
		glUseProgram(shdPrgIDL);
	}

	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "txrTypeU");
	glUniform1i(lgtCountLocL, txrType);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


// Draw the above Vertex.   DrawRectVrtxAry3D
void CcGPUCtrlIntrFc::DrawTriVrtxAryTxr3D(int txrTypeP) {

	int lgtCountLocL;
	GLuint shdPrgIDL;

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIDL = shdPrgIdAryLgt[SHD_PRG_INDX_COL_MAT_COLOR_TXR];
		glUseProgram(shdPrgIDL);
	}

	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "txrTypeU");
	glUniform1i(lgtCountLocL, txrTypeP);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


void CcGPUCtrlIntrFc::ClearTriVrtxAryTxr3D() {
	indc = 0;
	vrtxCount = 0;

	std::vector<CcVertexTxr >().swap(cVerticesTxr);
	std::vector<unsigned int>().swap(indices);
}


// void CcGPUCtrlIntrFc::DeleteRectVrtxAryTxr3D()
// {We can use     'void CcGPUCtrlIntrFc::DeleteRectVrtxAry3D()'   }


/*  Use from " LoadTextureFileGPU "

bool CcGPUCtrlIntrFc::LoadTxrInRectVrtxAry3D(short paternNum, unsigned int* textureIDR)
{
	bool retFL = false;
	// load and generate the texture
	int width=0, height=0, nrChannels=0;

	unsigned int textureIDL = 0;

	unsigned char* data = stbi_load(texFileP.c_str(), &width, &height, &nrChannels, 0);




	if (data)
	{
		glGenTextures(1, &textureIDL);
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, textureIDL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		*textureIDR = textureIDL;
		retFL = true;
	}

	//	stbi_image_free(data);

	return retFL;
}
*/

/*

 // * Cleanup all the things we bound and allocated
glUseProgram(0);
glDisableVertexAttribArray(0);
glDisableVertexAttribArray(1);
glDetachShader(shaderprogram, vertexshader);
glDetachShader(shaderprogram, fragmentshader);
glDeleteProgram(shaderprogram);
glDeleteShader(vertexshader);
glDeleteShader(fragmentshader);
glDeleteBuffers(2, vbo);
glDeleteVertexArrays(1, &vao);
free(vertexsource);
free(fragmentsource);

*/