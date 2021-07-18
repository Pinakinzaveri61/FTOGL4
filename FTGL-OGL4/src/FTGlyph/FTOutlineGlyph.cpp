/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@hocevar.net>
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

#include "config.h"

#include "FTGL/ftgl.h"

#include "FTInternals.h"
#include "FTOutlineGlyphImpl.h"
#include "FTVectoriser.h"


//
//  FTGLOutlineGlyph
//


FTOutlineGlyph::FTOutlineGlyph(FT_GlyphSlot glyph, float outset,
                               bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP) :
    FTGlyph(new FTOutlineGlyphImpl(glyph, outset, useDisplayList, rGPUIntFcP))
{}


FTOutlineGlyph::~FTOutlineGlyph()
{}


const FTPoint& FTOutlineGlyph::Render(const FTPoint& pen, int renderMode)
{
    FTOutlineGlyphImpl *myimpl = dynamic_cast<FTOutlineGlyphImpl *>(impl);
    return myimpl->RenderImpl(pen, renderMode);
}


//
//  FTGLOutlineGlyphImpl
//


FTOutlineGlyphImpl::FTOutlineGlyphImpl(FT_GlyphSlot glyph, float _outset,
                                       bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP)
:   FTGlyphImpl(glyph),
    vectoriser(0),
    glList(0)
{
	rGPUIntrFcGlyImplC = rGPUIntFcP;

    if(ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }

    vectoriser = new FTVectoriser(glyph);

    if((vectoriser->ContourCount() < 1) || (vectoriser->PointCount() < 3))
    {
        delete vectoriser;
        vectoriser = NULL;
        return;
    }

    outset = _outset;

    indexVAO = 0;

    if(useDisplayList)
    {
     //   glList = glGenLists(1);			// TBD for VAO
     //   glNewList(glList, GL_COMPILE);

 
        DoRender(); // Renders ans stores the VAO
        rGPUIntrFcGlyImplC->UploadLineStripAry();
        //       rGPUIntrFcGlyImplC->DrawTriVrtxAry2D();
        rGPUIntrFcGlyImplC->ClearLineStripAry();
        indexVAO = rGPUIntrFcGlyImplC->VAO;
        indexVBO = rGPUIntrFcGlyImplC->VBO;
        indexEBO = rGPUIntrFcGlyImplC->EBO;

        indexSize = rGPUIntrFcGlyImplC->indxSize;

     //   glEndList();

        delete vectoriser;
        vectoriser = NULL;
    }
}


FTOutlineGlyphImpl::~FTOutlineGlyphImpl()
{
    if (indexVAO)
    {
        //  glDeleteLists(glList, 1);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->VBO = indexVBO;
        rGPUIntrFcGlyImplC->EBO = indexEBO;
        
        rGPUIntrFcGlyImplC->DeleteLineStripAry(); // Delete VAO & VBO

        // glDeleteLists(glList, 1);
        // PZ: Delete VBO
        // ? rGPUIntrFcGlyImplC->
    }
    else if(vectoriser)
    {
        delete vectoriser;
    }
}


const FTPoint& FTOutlineGlyphImpl::RenderImpl(const FTPoint& pen,
                                              int renderMode)
{
    (void)renderMode;

//    glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());
	float xL = pen.Xf(); float yL = pen.Yf(); float zL = pen.Zf();
	rGPUIntrFcGlyImplC->CcTranslate(xL, yL, zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();

    if(indexVAO)
    {
      //  glCallList(glList);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->indxSize = indexSize;
        rGPUIntrFcGlyImplC->DrawLineStripAry(); // Display fro VAO
    }
    else if(vectoriser)
    {
        DoRender();
        rGPUIntrFcGlyImplC->DrawOnTheFlyLineStripAry();
    }

    //glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());
	rGPUIntrFcGlyImplC->CcTranslate(-xL, -yL, -zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();

    return advance;
}


void FTOutlineGlyphImpl::DoRender()
{
	G3DVector vPtL, fstPtL;
	bool fstPtFL = false;

    for(unsigned int c = 0; c < vectoriser->ContourCount(); ++c)
    {
        const FTContour* contour = vectoriser->Contour(c);

		/*
        glBegin(GL_LINE_LOOP);
            for(unsigned int i = 0; i < contour->PointCount(); ++i)
            {
                FTPoint point = FTPoint(contour->Point(i).X() + contour->Outset(i).X() * outset,
                                        contour->Point(i).Y() + contour->Outset(i).Y() * outset,
                                        0);
                glVertex2f(point.Xf() / 64.0f, point.Yf() / 64.0f);
            }
        glEnd();
		*/
		fstPtFL = true;
		for (unsigned int i = 0; i < contour->PointCount(); ++i)
		{

			FTPoint point = FTPoint(contour->Point(i).X() + contour->Outset(i).X() * outset,
				contour->Point(i).Y() + contour->Outset(i).Y() * outset,
				0);

			// glVertex2f(point.Xf() / 64.0f, point.Yf() / 64.0f);
			vPtL.x = point.Xf() / 64.0f; vPtL.y = point.Yf() / 64.0f; vPtL.z = 0.0;

			if (fstPtFL) {fstPtL = vPtL; fstPtFL = false;}

			rGPUIntrFcGlyImplC->LinkListLineStripBuild(vPtL);
		}
		
 		rGPUIntrFcGlyImplC->LinkListLineStripBuild(fstPtL);

    }
}

