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
By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such that it only contains the relevant 
definition source code, effectively turning the header file into a .cpp file, and that's about it.
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool CcGPUCtrlIntrFc::LoadTextureFileGPU(std::string texFileP, unsigned int* textureIDR)
{
	bool retFL = false;
	// load and generate the texture
	int txrWidL, txrHtL, colrComp;

	unsigned int textureIDL = 0;

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//	stbi_set_flip_vertically_on_load(true);
	stbi_set_flip_vertically_on_load(false); 
	unsigned char* texData = stbi_load(texFileP.c_str(), &txrWidL, &txrHtL, &colrComp, 0);

	if (texData)
	{
		glGenTextures(1, &textureIDL);
		glBindTexture(GL_TEXTURE_2D, textureIDL);
 
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txrWidL, txrHtL, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		/*
		if (colrComp == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txrWidL, txrHtL, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		}
		else if (colrComp == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txrWidL, txrHtL, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		}
		*/
		glGenerateMipmap(GL_TEXTURE_2D);

		*textureIDR = textureIDL;
		retFL = true;
	}

	stbi_image_free(texData);

	return retFL;
}




void CcGPUCtrlIntrFc::CcSetTexture(unsigned int texrIDP)
{
	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	glBindTexture(GL_TEXTURE_2D, texrIDP); // Auto assigned to 'smpl2Dtxr'

	// short txrParam 

	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}
