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



// ******   Line Strip Builder    ***********


void CcGPUCtrlIntrFc::LinkListLineStripBuild(G3DVector vrtxP)
{
	vertices.push_back((float)vrtxP.x); vertices.push_back((float)vrtxP.y); vertices.push_back((float)vrtxP.z);
	indices.push_back(indc);
	indc++;

}


void CcGPUCtrlIntrFc::DrawOnTheFlyLineStripAry() {

	UploadLineStripAry();
	DrawLineStripAry();
	ClearLineStripAry();
	DeleteLineStripAry();
}

// Upload the Vertex to the GPU memory	UploadRectVrtxAry3D
void CcGPUCtrlIntrFc::UploadLineStripAry() {

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
	unsigned int vrtxSizeL = vertices.size() * sizeof(float);

	//	glBufferData(GL_ARRAY_BUFFER, cVertices.size() * sizeof(CcVertex), &cVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,                   // target
		vrtxSizeL, // data size, # of bytes
		vertices.data(),   // ptr to vertex data
		GL_STATIC_DRAW);                   // usage

	indxSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CcVertex), (void*)0);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBindVertexArray(0);

}


// Draw the above Vertex.   DrawRectVrtxAry3D
void CcGPUCtrlIntrFc::DrawLineStripAry() {

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
	glDrawElements(GL_LINE_STRIP, indxSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}


/*
// Draw the above Vertex.   DrawRectVrtxAry3D
void CcGPUCtrlIntrFc::DrawLineStripAry() {

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

	// draw a Line Strip
	glDrawElements(GL_LINE_STRIP,                    // primitive type
		indxSize,          // # of indices
		GL_UNSIGNED_INT,                 // data type
		(void*)0);                       // offset to indices

	glBindVertexArray(0);
}
*/


//ClearRectVrtxAry3D
void CcGPUCtrlIntrFc::ClearLineStripAry() {
	indc = 0;
	vrtxCount = 0;

	std::vector<float>().swap(vertices);
	std::vector<unsigned int>().swap(indices);
}


// DeleteRectVrtxAry3D
void CcGPUCtrlIntrFc::DeleteLineStripAry() {

	// unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Delete GPU Buffers 
	const GLuint* delObjL;

	delObjL = &VBO;
	glDeleteBuffers(1, delObjL);

	delObjL = &EBO;
	glDeleteBuffers(1, delObjL);

	delObjL = &VAO;
	glDeleteVertexArrays(1, delObjL);

}




// Following to Delete ???

// Draw Line Strip from above LL
void CcGPUCtrlIntrFc::DrawLineStripLL()
{
	GLint attribVertex; // attribVertexPosition;
	GLuint shdPrgIndxL, shdPrgIDL;
	SelectF = false;

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

	// attribVertexPosition = glGetAttribLocation(progId, "vertexPosition");
	attribVertex = glGetAttribLocation(shdPrgIDL, "inPos");


	unsigned int vrtxSizeL = vertices.size() * sizeof(float);
	unsigned int numOfIndicL = indices.size();
	unsigned int indicSizeL = numOfIndicL * sizeof(unsigned int);

	// copy interleaved vertex data (V/N/T) to VBO
//	GLuint vboId;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);           // for vertex data
	glBufferData(GL_ARRAY_BUFFER,                   // target
		vrtxSizeL, // data size, # of bytes
		vertices.data(),   // ptr to vertex data
		GL_STATIC_DRAW);                   // usage

// copy index data to VBO
//	GLuint iboId;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);   // for index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
		indicSizeL,             // data size, # of bytes
		indices.data(),               // ptr to index data
		GL_STATIC_DRAW);                   // usage



		// bind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	// activate attrib arrays
	glEnableVertexAttribArray(attribVertex);

	// set attrib arrays with stride and offset
//	int stride = 32; // sphere.getInterleavedStride();     // should be 32 bytes

	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

//	glVertexAttribPointer(attribVertex, 0, GL_FLOAT, GL_FALSE, 0, NULL);

	
//	glDrawArrays(GL_LINES, 0, 2);

	glDrawElements(GL_TRIANGLES, numOfIndicL, GL_UNSIGNED_INT, (void*)0);

	// draw a Line Strip
	glDrawElements(GL_LINE_STRIP,                    // primitive type
		numOfIndicL,          // # of indices
		GL_UNSIGNED_INT,                 // data type
		(void*)0);                       // offset to indices

		// deactivate attrib arrays
//	glDisableVertexAttribArray(attribVertex);

	const GLuint* delObjL;

	// unbind VBOs
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Following Clears the VBO & EBO. Otherwise it increases the VBO & EBO on each 'glGenBuffers'
	delObjL = &VBO;
	glDeleteBuffers(1, delObjL);

	delObjL = &EBO;
	glDeleteBuffers(1, delObjL);

}
