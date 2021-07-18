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

#include <string>


// Uploads common Light params
// Pl. note 'glGetUniformLocation' returns value, only for the variable used in main(), just by defining will not use it.
void CcGPUCtrlIntrFc::UploadLgtComn(GLuint shdPrgIndxP) {

	int lgtCountLocL;

//	GLuint shdPrgIndxL, lgtNumL;

	GLuint shdPrgIDL;

	shdPrgIDL = shdPrgIdAryLgt[shdPrgIndxP];
	glUseProgram(shdPrgIDL);
	

	// Update Global Illumination Light.
	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "glblIluminU"); // No. of Point Light
	glUniform3f(lgtCountLocL, glblIlumin.r, glblIlumin.g, glblIlumin.b);
	
	// Set 
	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "alphaU"); // Material Alpha
	glUniform1f(lgtCountLocL, alpha); 

	// Upload number of Spot light to GPU
	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "numOfLightsU");
	glUniform1i(lgtCountLocL, numOfLights);
	
	
	lgtCountLocL = glGetUniformLocation(shdPrgIDL, "txrTypeU");
	if (lgtCountLocL >= 0) glUniform1i(lgtCountLocL, txrType);

}



void CcGPUCtrlIntrFc::UploadLgtProp(GLuint shdPrgIndxP, unsigned int lgtNum) {
	int lgtPropLocL;
	float val1, val2, val3;
	GLuint shdPrgIDL;

	shdPrgIDL = shdPrgIdAryLgt[shdPrgIndxP];
	glUseProgram(shdPrgIDL);
	
	std::string lgtAry = "lightPropAry[";
	lgtAry += std::to_string(lgtNum);
	lgtAry += "].";
	std::string lgtAry2;

	lgtAry2 = lgtAry; lgtAry2 += "lightType";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	glUniform1i(lgtPropLocL, ltType);

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].ambient");
	lgtAry2 = lgtAry; lgtAry2 += "ambient";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	val1 = ambient.r; val2 = ambient.g; val3 = ambient.b;
	glUniform3f(lgtPropLocL, val1, val2, val3);

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].diffuse");
	lgtAry2 = lgtAry; lgtAry2 += "diffuse";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	val1 = diffuse.r; val2 = diffuse.g; val3 = diffuse.b;
	glUniform3f(lgtPropLocL, val1, val2, val3);

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].specular");
	lgtAry2 = lgtAry; lgtAry2 += "specular";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	val1 = specular.r; val2 = specular.g; val3 = specular.b;
	glUniform3f(lgtPropLocL, val1, val2, val3);


//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].constant");
	lgtAry2 = lgtAry; lgtAry2 += "constant";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	glUniform1f(lgtPropLocL, constant);

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].linear");
	lgtAry2 = lgtAry; lgtAry2 += "linear";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	glUniform1f(lgtPropLocL, linear);

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].quadratic");
	lgtAry2 = lgtAry; lgtAry2 += "quadratic";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	glUniform1f(lgtPropLocL, quadratic);


//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].cutOff");
	lgtAry2 = lgtAry; lgtAry2 += "cutOff";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
//	cutOff = 2.0; // HC to test ?
	glUniform1f(lgtPropLocL, glm::cos(glm::radians(cutOff)) );

//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].outerCutOff");
	lgtAry2 = lgtAry; lgtAry2 += "outerCutOff";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
//	outerCutOff = 3.0; // HC to test ?
	glUniform1f(lgtPropLocL, glm::cos(glm::radians(outerCutOff)));
	

}

void CcGPUCtrlIntrFc::UploadLgtDef(GLuint shdPrgIndxP, unsigned int lgtNum) {

	int lgtPropLocL;
	float val1, val2, val3;
	GLuint shdPrgIDL;

	shdPrgIDL = shdPrgIdAryLgt[shdPrgIndxP];
	glUseProgram(shdPrgIDL);

	std::string lgtAry = "lightDefAry[";
	lgtAry += std::to_string(lgtNum);
	lgtAry += "].";

	std::string lgtAry2;
		

	lgtAry2 = lgtAry; lgtAry2 += "indxToProp";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	glUniform1i(lgtPropLocL, indxToLtProp);
	
//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "lightDefAry[0].indxToProp");
//	glUniform1i(lgtPropLocL, 0);

		//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].position");
	lgtAry2 = lgtAry; lgtAry2 += "position";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	val1 = (float) position.x; val2 = (float) position.y; val3 = (float) position.z;
	glUniform3f(lgtPropLocL, val1, val2, val3);


	//	lgtPropLocL = glGetUniformLocation(shdPrgIDL, "spotLightAry[0].direction");
	lgtAry2 = lgtAry; lgtAry2 += "direction";
	lgtPropLocL = glGetUniformLocation(shdPrgIDL, lgtAry2.c_str());
	val1 = (float) direction.x; val2 = (float) direction.y; val3 = (float) direction.z;
	glUniform3f(lgtPropLocL, val1, val2, val3);


}
