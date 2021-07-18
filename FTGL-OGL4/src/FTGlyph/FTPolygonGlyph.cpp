/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@hocevar.net>
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

#include "pch.h"

#include "config.h"

#include "FTGL/ftgl.h"

#include "FTInternals.h"
#include "FTPolygonGlyphImpl.h"
#include "FTVectoriser.h"


//
//  FTGLPolyGlyph
//


FTPolygonGlyph::FTPolygonGlyph(FT_GlyphSlot glyph, float outset,
                               bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP) :
    FTGlyph(new FTPolygonGlyphImpl(glyph, outset, useDisplayList, rGPUIntFcP))
{}




FTPolygonGlyph::FTPolygonGlyph(FT_GlyphSlot glyph, float outset,
    bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP, bool useTxr) :
    FTGlyph(new FTPolygonGlyphImpl(glyph, outset, useDisplayList, rGPUIntFcP, useTxr))
{}


FTPolygonGlyph::~FTPolygonGlyph()
{}


const FTPoint& FTPolygonGlyph::Render(const FTPoint& pen, int renderMode)
{
    FTPolygonGlyphImpl *myimpl = dynamic_cast<FTPolygonGlyphImpl *>(impl);
    return myimpl->RenderImpl(pen, renderMode);
}


//
//  FTGLPolyGlyphImpl
//

FTPolygonGlyphImpl::FTPolygonGlyphImpl(FT_GlyphSlot glyph, float _outset,
    bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP)
    : FTGlyphImpl(glyph),
    vectoriser(0),
    glList(0)
{
    txrF = false;
}


FTPolygonGlyphImpl::FTPolygonGlyphImpl(FT_GlyphSlot glyph, float _outset,
                                       bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP, bool useTxrP)
:   FTGlyphImpl(glyph),
    vectoriser(0),
    glList(0)
{
	rGPUIntrFcGlyImplC = rGPUIntFcP;
    txrF = useTxrP;

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


    hscale = glyph->face->size->metrics.x_ppem * 64;
    vscale = glyph->face->size->metrics.y_ppem * 64;
    outset = _outset;

    indexVAO = 0;

    if(useDisplayList)
    {
    //    glList = glGenLists(1);			// TBD for VAO ?
    //    glNewList(glList, GL_COMPILE);

        DoRender(); // Renders ans stores the VAO
        if (txrF)
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAryTxr2D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAryTxr2D();
        }
        else
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAry2D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAry2D();
        }

        indexVAO = rGPUIntrFcGlyImplC->VAO;
        indexVBO = rGPUIntrFcGlyImplC->VBO;
        indexEBO = rGPUIntrFcGlyImplC->EBO;

        indexSize = rGPUIntrFcGlyImplC->indxSize;

    //    glEndList();

        delete vectoriser;
        vectoriser = NULL;
    }
}


FTPolygonGlyphImpl::~FTPolygonGlyphImpl()
{
    if(indexVAO)
    {
       //  glDeleteLists(glList, 1);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->VBO = indexVBO;
        rGPUIntrFcGlyImplC->EBO = indexEBO;
        if (txrF)
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAryTxr2D(); // Delete VAO & VBO
        }else
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry2D(); // Delete VAO & VBO
        }
    }
    else if(vectoriser)
    {
        delete vectoriser;
    }
}


const FTPoint& FTPolygonGlyphImpl::RenderImpl(const FTPoint& pen,
                                              int renderMode)
{
    (void)renderMode;

    //glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());
	float xL = pen.Xf(); float yL = pen.Yf(); float zL = pen.Zf();
	rGPUIntrFcGlyImplC->CcTranslate(xL, yL, zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();


    if (indexVAO)
    {
        // glCallList(glList);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->indxSize = indexSize;
        if (txrF)
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAryTxr2D(); // Display fro VAO
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAry2D(); // Display fro VAO
        }
    }else if(vectoriser)
    {
        DoRender();
        if (txrF)
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAryTxr2D();
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAry2D();
        }
    }


//    glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());
//	float xL = pen.Xf(); float yL = pen.Yf(); float zL = pen.Zf();
	rGPUIntrFcGlyImplC->CcTranslate(-xL, -yL, -zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();

    return advance;
}


void FTPolygonGlyphImpl::DoRender()
{
	G3DVector vPtL, tCorL;

    vectoriser->MakeMesh(1.0, 1, outset);

    const FTMesh *mesh = vectoriser->GetMesh();

    for(unsigned int t = 0; t < mesh->TesselationCount(); ++t)
    {
        const FTTesselation* subMesh = mesh->Tesselation(t);
        unsigned int polygonType = subMesh->PolygonType();

		rGPUIntrFcGlyImplC->BeginSetTriVrtxAry(polygonType);

		/* OGL1
        glBegin(polygonType);
            for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
            {
                FTPoint point = subMesh->Point(i);
                glTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
                glVertex3f(point.Xf() / 64.0f, point.Yf() / 64.0f, 0.0f);
            }
        glEnd();
		*/

		for (unsigned int i = 0; i < subMesh->PointCount(); ++i)
		{
			FTPoint point = subMesh->Point(i);
		//	glTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
		//	glVertex3f(point.Xf() / 64.0f, point.Yf() / 64.0f, 0.0f);
		//	vPtL.x = point.Xf() / 64.0f; vPtL.y = point.Yf() / 64.0f; vPtL.z = 0.0;
            vPtL.x = point.Xf(); vPtL.y = point.Yf(); vPtL.z = 0.0;
            tCorL = vPtL;
            tCorL.x /= hscale;  tCorL.y /= vscale;
            vPtL.x /= 64.0; vPtL.y /= 64.0;
            if (txrF)
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAryTxr2D(vPtL, tCorL);
            }
            else
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vPtL);
            }
		}
		rGPUIntrFcGlyImplC->ClearTriVrtxCount();
    }

}

