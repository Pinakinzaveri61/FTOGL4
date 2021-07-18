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


/*
Functions Names could be
BeginTri2D(eleTypeP)
AddTriPts2D(G3DVector vrtxP)

*/


// eleTypeP -> GL_TRIANGLES  /  GL_TRIANGLE_STRIP / GL_TRIANGLE_FAN / 
// note use  GL_TRIANGLE_STRIP  for   GL_QUAD_STRIP also as it would |\|\|\|\


void CcGPUCtrlIntrFc::BeginSetTriVrtxAry(short eleTypeP) {

	eleTypeC = eleTypeP;
}

// 
// Push 'vrtxP' on the vertices(float)  Vector stack.
void CcGPUCtrlIntrFc::AddInTriVrtxAry2D(G3DVector vrtxP) {
	CcVertexP vertexL;
	glm::vec3 vectorL; // we declare a placeholder vector since ConsCAD uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//	int indxAry[4];

	// positions
	vectorL.x = (float)vrtxP.x; vectorL.y = (float)vrtxP.y; vectorL.z = (float)vrtxP.z;
	vertexL.Position = vectorL;

	if (eleTypeC == GL_TRIANGLES)
	{
		// Add the Vertex 
	//	vertices.push_back((float)vrtxP.x); vertices.push_back((float)vrtxP.y); vertices.push_back((float)vrtxP.z);
		cVerticesP.push_back(vertexL);
		indices.push_back(indc);
		indc++;

	}
	else if (eleTypeC == GL_TRIANGLE_STRIP)
	{
		// Add the Vertex 
		// vertices.push_back((float)vrtxP.x); vertices.push_back((float)vrtxP.y); vertices.push_back((float)vrtxP.z);
		cVerticesP.push_back(vertexL);

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
		// Add the Vertex 
	//	vertices.push_back((float)vrtxP.x); vertices.push_back((float)vrtxP.y); vertices.push_back((float)vrtxP.z);
		cVerticesP.push_back(vertexL);

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

void CcGPUCtrlIntrFc::DrawOnTheFlyTriVrtxAry2D() {

	UploadTriVrtxAry2D();
	DrawTriVrtxAry2D();
	ClearTriVrtxAry2D();
	DeleteTriVrtxAry2D();
}


// We split Vertex upload and Draw. This will increae the speed. 

// Upload the Vertex to the GPU memory	UploadRectVrtxAry3D
void CcGPUCtrlIntrFc::UploadTriVrtxAry2D() {
	GLint attribVertex; // attribVertexPosition;
	//	GLint attribNormal;
	GLuint shdPrgIndxL, shdPrgIDL;
	//	GLint attribTxrCor;

		// VBO uses vertex Normal  for lighting . So we use column 'SHD_PRG_INDX_COL_MAT_COLOR'
	//	shdPrgIDL = shdPrgIdAryLgt[indxRowShdPrgLgt] [SHD_PRG_INDX_COL_MAT_COLOR];

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIndxL = SHD_PRG_INDX_COL;
		shdPrgIDL = shdPrgIdAry[shdPrgIndxL];
		glUseProgram(shdPrgIDL);
	}

	//	attribVertex = glGetAttribLocation(shdPrgIDL, "inPos");
	attribVertex = glGetAttribLocation(shdPrgIDL, "inPos");

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
	unsigned int vrtxSizeL = cVerticesP.size() * sizeof(CcVertexP);

//	glBufferData(GL_ARRAY_BUFFER, cVertices.size() * sizeof(CcVertex), &cVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,                   // target
		vrtxSizeL, // data size, # of bytes
		cVerticesP.data(),   // ptr to vertex data
		GL_STATIC_DRAW);                   // usage

	indxSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CcVertex), (void*)0);

	unsigned int size2 = sizeof(CcVertexP);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, size2, NULL);

	glBindVertexArray(0);

}


//ClearRectVrtxAry3D
void CcGPUCtrlIntrFc::ClearTriVrtxAry2D() {
	indc = 0;
	vrtxCount = 0;

	std::vector<CcVertexP>().swap(cVerticesP);
	std::vector<unsigned int>().swap(indices);
}


// This will enable other patch of Tringle / Fan / Strip to be added, in this same Array.
void CcGPUCtrlIntrFc::ClearTriVrtxCount() {
	vrtxCount = 0;
}


// Draw the above Vertex.   DrawRectVrtxAry3D
void CcGPUCtrlIntrFc::DrawTriVrtxAry2D() {

	//	int lgtCountLocL;
	GLuint shdPrgIndxL, shdPrgIDL;

	if (SelectF)
	{
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_SELECT];
		glUseProgram(shdPrgIDL);
	}
	else
	{
		shdPrgIndxL = SHD_PRG_INDX_COL;
		shdPrgIDL = shdPrgIdAry[shdPrgIndxL];
		glUseProgram(shdPrgIDL);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


// DeleteRectVrtxAry3D
void CcGPUCtrlIntrFc::DeleteTriVrtxAry2D() {

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



void CcGPUCtrlIntrFc::AddInTriVrtxAryTxr2D(G3DVector vrtxP, G3DVector txrCorP) {

	glmPosTxr vrtxTxrL;
	glm::vec3 vectorL; // we declare a placeholder vector since ConsCAD uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
	glm::vec2 TexCordL;

	// positions
	vectorL.x = (float)vrtxP.x; vectorL.y = (float)vrtxP.y; vectorL.z = (float)vrtxP.z;
	vrtxTxrL.Position = vectorL;

	TexCordL.x = (float)txrCorP.x; TexCordL.y = (float)txrCorP.y;
	vrtxTxrL.TexCoords = TexCordL;


	if (eleTypeC == GL_TRIANGLES)
	{
		// ***  Add the Vertex  // Push vertex
		vecGlmPosTxr.push_back(vrtxTxrL);
		indices.push_back(indc);
		indc++;

	}
	else if (eleTypeC == GL_TRIANGLE_STRIP)
	{
		// ***-  Add the Vertex 	// Push vertex
		vecGlmPosTxr.push_back(vrtxTxrL);

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
		vecGlmPosTxr.push_back(vrtxTxrL);

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



void CcGPUCtrlIntrFc::DrawOnTheFlyTriVrtxAryTxr2D() {

	UploadTriVrtxAryTxr2D();
	DrawTriVrtxAryTxr2D();
	ClearTriVrtxAryTxr2D();
	DeleteTriVrtxAryTxr2D();
}


void CcGPUCtrlIntrFc::UploadTriVrtxAryTxr2D() {
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
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_COL_TXR];
		glUseProgram(shdPrgIDL);
	}


		// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vecGlmPosTxr.size() * sizeof(glmPosTxr), &vecGlmPosTxr[0], GL_STATIC_DRAW);

	indxSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glmPosTxr), (void*)0);

	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glmPosTxr), (void*)offsetof(glmPosTxr, TexCoords));

	glBindVertexArray(0);
}

void CcGPUCtrlIntrFc::DrawTriVrtxAryTxr2D() {
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
		shdPrgIDL = shdPrgIdAry[SHD_PRG_INDX_COL_TXR];
		glUseProgram(shdPrgIDL);
	}

	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "txrTypeU");
	glUniform1i(lgtCountLocL, txrType);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


void CcGPUCtrlIntrFc::ClearTriVrtxAryTxr2D() {
	indc = 0;
	vrtxCount = 0;

	std::vector<glmPosTxr>().swap(vecGlmPosTxr);
	std::vector<unsigned int>().swap(indices);
}



void CcGPUCtrlIntrFc::DeleteTriVrtxAryTxr2D() {

	DeleteTriVrtxAry2D();
}