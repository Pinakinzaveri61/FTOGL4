/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
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

#include <iostream>

#include "FTGL/ftgl.h"

#include "FTInternals.h"
#include "FTExtrudeGlyphImpl.h"
#include "FTVectoriser.h"


//
//  FTGLExtrudeGlyph
//


FTExtrudeGlyph::FTExtrudeGlyph(FT_GlyphSlot glyph, float depth,
                               float frontOutset, float backOutset,
                               bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP) :
    FTGlyph(new FTExtrudeGlyphImpl(glyph, depth, frontOutset, backOutset,
                                   useDisplayList, rGPUIntFcP))
{}

FTExtrudeGlyph::FTExtrudeGlyph(FT_GlyphSlot glyph, float depth,
    float frontOutset, float backOutset,
    bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP, bool useTxr, bool useLight) :
    FTGlyph(new FTExtrudeGlyphImpl(glyph, depth, frontOutset, backOutset,
        useDisplayList, rGPUIntFcP, useTxr, useLight))
{}

FTExtrudeGlyph::~FTExtrudeGlyph()
{}


const FTPoint& FTExtrudeGlyph::Render(const FTPoint& pen, int renderMode)
{
    FTExtrudeGlyphImpl *myimpl = dynamic_cast<FTExtrudeGlyphImpl *>(impl);
    return myimpl->RenderImpl(pen, renderMode);
}


//
//  FTGLExtrudeGlyphImpl
//

FTExtrudeGlyphImpl::FTExtrudeGlyphImpl(FT_GlyphSlot glyph, float _depth,
    float _frontOutset, float _backOutset,
    bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP)
    : FTGlyphImpl(glyph),
    vectoriser(0),
    glList(0)
{}


FTExtrudeGlyphImpl::FTExtrudeGlyphImpl(FT_GlyphSlot glyph, float _depth,
                                       float _frontOutset, float _backOutset,
                                       bool useDisplayList, CcGPUCtrlIntrFc* rGPUIntFcP, bool useTxrP, bool useLgtP)
:   FTGlyphImpl(glyph),
    vectoriser(0),
    glList(0)
{
	rGPUIntrFcGlyImplC = rGPUIntFcP;
    txrF = useTxrP;
    lightF = useLgtP; // True: 3D - Light enable

    bBox.SetDepth(-_depth);

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
    depth = _depth;
    frontOutset = _frontOutset;
    backOutset = _backOutset;

    if(useDisplayList)
    {
    //    glList = glGenLists(3);

        /* Front face */
 //       glNewList(glList + 0, GL_COMPILE);
        RenderFront();
 //       glEndList();
        TriVrtxAry();

        indexVAO = rGPUIntrFcGlyImplC->VAO; 
        indexVBO = rGPUIntrFcGlyImplC->VBO;
        indexEBO = rGPUIntrFcGlyImplC->EBO;
        indexSize = rGPUIntrFcGlyImplC->indxSize;



        /* Back face */
 //       glNewList(glList + 1, GL_COMPILE);
        RenderBack();
 //       glEndList();
        TriVrtxAry();
        
        indexBkVAO = rGPUIntrFcGlyImplC->VAO;
        indexBkVBO = rGPUIntrFcGlyImplC->VBO;
        indexBkEBO = rGPUIntrFcGlyImplC->EBO;
        indexSizeBk = rGPUIntrFcGlyImplC->indxSize;

        /* Side face */
 //       glNewList(glList + 2, GL_COMPILE);
        RenderSide();
 //       glEndList();
        TriVrtxAry();
 
        indexSdVAO = rGPUIntrFcGlyImplC->VAO;
        indexSdVBO = rGPUIntrFcGlyImplC->VBO;
        indexSdEBO = rGPUIntrFcGlyImplC->EBO;
        indexSizeSd = rGPUIntrFcGlyImplC->indxSize;

        delete vectoriser;
        vectoriser = NULL;
    }
}


FTExtrudeGlyphImpl::~FTExtrudeGlyphImpl()
{
    if(indexVAO)
    {
       // glDeleteLists(glList, 3);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->VBO = indexVBO;
        rGPUIntrFcGlyImplC->EBO = indexEBO;
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry3D(); // Delete VAO & VBO
        }
        else
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry2D(); // Delete VAO & VBO
        }

        rGPUIntrFcGlyImplC->VAO = indexBkVAO;
        rGPUIntrFcGlyImplC->VBO = indexBkVBO;
        rGPUIntrFcGlyImplC->EBO = indexBkEBO;
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry3D(); // Delete VAO & VBO
        }
        else
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry2D(); // Delete VAO & VBO
        }
        rGPUIntrFcGlyImplC->VAO = indexSdVAO;
        rGPUIntrFcGlyImplC->VBO = indexSdVBO;
        rGPUIntrFcGlyImplC->EBO = indexSdEBO;
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry3D(); // Delete VAO & VBO
        }
        else
        {
            rGPUIntrFcGlyImplC->DeleteTriVrtxAry2D(); // Delete VAO & VBO
        }
    }
    else if(vectoriser)
    {
        delete vectoriser;
    }
}

void FTExtrudeGlyphImpl::TriVrtxAry()
{
    if (txrF)
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAryTxr3D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAryTxr3D();
        }
        else
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAryTxr2D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAryTxr2D();
        }
    }
    else
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAry3D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAry3D();
        }
        else
        {
            rGPUIntrFcGlyImplC->UploadTriVrtxAry2D();
            rGPUIntrFcGlyImplC->ClearTriVrtxAry2D();
        }
    }
}


const FTPoint& FTExtrudeGlyphImpl::RenderImpl(const FTPoint& pen,
                                              int renderMode)
{
    // glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());
	float xL = pen.Xf(); float yL = pen.Yf(); float zL = pen.Zf();
	rGPUIntrFcGlyImplC->CcTranslate(xL, yL, zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();

    if(indexVAO)
    {
 
        rGPUIntrFcGlyImplC->CcSetUniformColor(0.1, 0.1, 0.9, 1.0);
        rGPUIntrFcGlyImplC->VAO = indexVAO;
        rGPUIntrFcGlyImplC->indxSize = indexSize;
        DrawTriVrtxAry();
        
        rGPUIntrFcGlyImplC->CcSetUniformColor(0.1, 0.9, 0.1, 1.0);
        rGPUIntrFcGlyImplC->VAO = indexBkVAO;
        rGPUIntrFcGlyImplC->indxSize = indexSizeBk;
        DrawTriVrtxAry();

        rGPUIntrFcGlyImplC->CcSetUniformColor(0.9, 0.1, 0.1, 1.0);
        rGPUIntrFcGlyImplC->VAO = indexSdVAO;
        rGPUIntrFcGlyImplC->indxSize = indexSizeSd;
        DrawTriVrtxAry();
     }
    else if(vectoriser)
    {
        
        if (renderMode & FTGL::RENDER_FRONT) {
            rGPUIntrFcGlyImplC->CcSetUniformColor(0.1, 0.1, 0.9, 1.0);
            RenderFront();
            DrawOnTheFlyTriVrtxAry();
        }

        
        if (renderMode & FTGL::RENDER_BACK) {
            rGPUIntrFcGlyImplC->CcSetUniformColor(0.9, 0.9, 0.1, 1.0);
            RenderBack();
            DrawOnTheFlyTriVrtxAry();
        }
        
        if (renderMode & FTGL::RENDER_SIDE) {
            rGPUIntrFcGlyImplC->CcSetUniformColor(0.9, 0.9, 0.9, 1.0);
            RenderSide();
            DrawOnTheFlyTriVrtxAry();
        }
    }

    // glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());
	rGPUIntrFcGlyImplC->CcTranslate(-xL, -yL, -zL);
	rGPUIntrFcGlyImplC->UpdModelGpuUniformVal();

    return advance;
}

void FTExtrudeGlyphImpl::DrawOnTheFlyTriVrtxAry()
{
    if (txrF)
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAryTxr3D();
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAryTxr2D();
        }
    }
    else
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAry3D();
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawOnTheFlyTriVrtxAry2D();
        }
    }


}


void FTExtrudeGlyphImpl::DrawTriVrtxAry()
{
    if (txrF)
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAryTxr3D(); // Display front VAO
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAryTxr2D(); // Display Side VAO
        }

    }
    else
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAry3D(); // Display front VAO
        }
        else
        {
            rGPUIntrFcGlyImplC->DrawTriVrtxAry2D(); // Display Side VAO
        }

    }


}

void FTExtrudeGlyphImpl::RenderFront()
{
	G3DVector vPtL, vNormalL;
    vNormalL.SetV(0.0, 0.0, 1.0);
    vectoriser->MakeMesh(1.0, 1, frontOutset);
    glNormal3d(0.0, 0.0, 1.0);

    const FTMesh *mesh = vectoriser->GetMesh();
    for(unsigned int j = 0; j < mesh->TesselationCount(); ++j)
    {
        const FTTesselation* subMesh = mesh->Tesselation(j);
        unsigned int polygonType = subMesh->PolygonType();

        rGPUIntrFcGlyImplC->BeginSetTriVrtxAry(polygonType);

		/*
        glBegin(polygonType);
            for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
            {
                FTPoint pt = subMesh->Point(i);

                glTexCoord2f(pt.Xf() / hscale,
                             pt.Yf() / vscale);

                glVertex3f(pt.Xf() / 64.0f,
                           pt.Yf() / 64.0f,
                           0.0f);
            }
        glEnd();
		*/
		for (unsigned int i = 0; i < subMesh->PointCount(); ++i)
		{
			FTPoint point = subMesh->Point(i);
			//	glTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
			//	glVertex3f(point.Xf() / 64.0f, point.Yf() / 64.0f, 0.0f);
//			vPtL.x = point.Xf() / 64.0f; vPtL.y = point.Yf() / 64.0f; vPtL.z = 0.0;
            vPtL.x = point.Xf(); vPtL.y = point.Yf(); vPtL.z = 0.0;

            AddInTriVrtxAry(vPtL, vNormalL);
            /*
            if (lightF)
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry3D(vPtL, vNormalL);
            }
            else
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vPtL);
            }
            */
		}
		rGPUIntrFcGlyImplC->ClearTriVrtxCount();
    }
}


void FTExtrudeGlyphImpl::RenderBack()
{
	G3DVector vPtL, vNormalL;
    vNormalL.SetV(0.0, 0.0, -1.0);

    vectoriser->MakeMesh(-1.0, 2, backOutset);
    glNormal3d(0.0, 0.0, -1.0);

    const FTMesh *mesh = vectoriser->GetMesh();
    for(unsigned int j = 0; j < mesh->TesselationCount(); ++j)
    {
        const FTTesselation* subMesh = mesh->Tesselation(j);
        unsigned int polygonType = subMesh->PolygonType();
         
        rGPUIntrFcGlyImplC->BeginSetTriVrtxAry(polygonType);
        
		/*
        glBegin(polygonType);
            for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
            {
                FTPoint pt = subMesh->Point(i);

                glTexCoord2f(subMesh->Point(i).Xf() / hscale,
                             subMesh->Point(i).Yf() / vscale);

                glVertex3f(subMesh->Point(i).Xf() / 64.0f,
                           subMesh->Point(i).Yf() / 64.0f,
                           -depth);
            }
        glEnd();
		*/

        for (unsigned int i = 0; i < subMesh->PointCount(); ++i)
        {
            FTPoint point = subMesh->Point(i);
            //	glTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
            //	glVertex3f(point.Xf() / 64.0f, point.Yf() / 64.0f, 0.0f);
 //           vPtL.x = point.Xf() / 64.0f; vPtL.y = point.Yf() / 64.0f; vPtL.z = -depth;
            vPtL.x = point.Xf(); vPtL.y = point.Yf(); vPtL.z = -depth;

            AddInTriVrtxAry(vPtL, vNormalL);
            /*
            if (lightF)
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry3D(vPtL, vNormalL);
            }
            else
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vPtL);
            }
            */
        }
		rGPUIntrFcGlyImplC->ClearTriVrtxCount();
    }
}


void FTExtrudeGlyphImpl::RenderSide()
{
	G3DVector vBackPtL, vFrontPtL, vNormalL;
	G3DVector vFstBackPtL, vFstFrontPtL;

	bool fstPtFL = false;

    int contourFlag = vectoriser->ContourFlag();

    for(size_t c = 0; c < vectoriser->ContourCount(); ++c)
    {
        const FTContour* contour = vectoriser->Contour(c);
        size_t n = contour->PointCount();

        if(n < 2)
        {
            continue;
        }

		fstPtFL = true;
        rGPUIntrFcGlyImplC->BeginSetTriVrtxAry(GL_TRIANGLE_STRIP);
 
    //    glBegin(GL_QUAD_STRIP);
		for (size_t j = 0; j <= n; ++j)
		{
			size_t cur = (j == n) ? 0 : j;
			size_t next = (cur == n - 1) ? 0 : cur + 1;

			FTPoint frontPt = contour->FrontPoint(cur);
			FTPoint nextPt = contour->FrontPoint(next);
			FTPoint backPt = contour->BackPoint(cur);

			FTPoint normal = FTPoint(0.f, 0.f, 1.f) ^ (frontPt - nextPt);
			if (normal != FTPoint(0.0f, 0.0f, 0.0f))
			{
				//   glNormal3dv(static_cast<const FTGL_DOUBLE*>(normal.Normalise()));
			}

			// tex TBD ?   glTexCoord2f(frontPt.Xf() / hscale, frontPt.Yf() / vscale);

            vBackPtL.x = backPt.Xf();  vBackPtL.y = backPt.Yf(); 
            vFrontPtL.x = frontPt.Xf(); vFrontPtL.y = frontPt.Yf();

			if (contourFlag & ft_outline_reverse_fill)
			{
				//    glVertex3f(backPt.Xf() / 64.0f, backPt.Yf() / 64.0f, 0.0f);
				//    glVertex3f(frontPt.Xf() / 64.0f, frontPt.Yf() / 64.0f, -depth);
                vBackPtL.z = 0.0;  vFrontPtL.z = -depth;
			}
			else
			{
				//    glVertex3f(backPt.Xf() / 64.0f, backPt.Yf() / 64.0f, -depth);
				//    glVertex3f(frontPt.Xf() / 64.0f, frontPt.Yf() / 64.0f, 0.0f);
//				vBackPtL.x = backPt.Xf() / 64.0f; vBackPtL.y = backPt.Yf() / 64.0f; vBackPtL.z = -depth;
//				vFrontPtL.x = frontPt.Xf() / 64.0f; vFrontPtL.y = frontPt.Yf() / 64.0f; vFrontPtL.z = 0.0;
                vBackPtL.z = -depth;  vFrontPtL.z = 0.0;
            }
			if (fstPtFL) { vFstFrontPtL = vFrontPtL; vFstBackPtL = vBackPtL;  fstPtFL = false; }

            vNormalL.x = normal.Xf(); vNormalL.y = normal.Yf(); vNormalL.z = normal.Zf();
            vNormalL.Normalize();

            AddInTriVrtxAry(vBackPtL, vNormalL);
            AddInTriVrtxAry(vFrontPtL, vNormalL);

            /*
            if (lightF)
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry3D(vBackPtL, vNormalL);
                rGPUIntrFcGlyImplC->AddInTriVrtxAry3D(vFrontPtL, vNormalL);
            }
            else
            {
                rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vBackPtL);
                rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vFrontPtL);
            }
            */
		}
        rGPUIntrFcGlyImplC->ClearTriVrtxCount();
    }
}


void FTExtrudeGlyphImpl::AddInTriVrtxAry(G3DVector vPtP, G3DVector vNormalP)
{
    G3DVector tCorL;
    tCorL = vPtP;
    tCorL.x /= hscale;  tCorL.y /= vscale;
    vPtP.x /= 64.0; vPtP.y /= 64.0;

    if (txrF)
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->AddInTriVrtxAryTxr3D(vPtP, vNormalP, tCorL);
        }
        else
        {
            rGPUIntrFcGlyImplC->AddInTriVrtxAryTxr2D(vPtP, tCorL);
        }
    }
    else
    {
        if (lightF)
        {
            rGPUIntrFcGlyImplC->AddInTriVrtxAry3D(vPtP, vNormalP);
        }
        else
        {
            rGPUIntrFcGlyImplC->AddInTriVrtxAry2D(vPtP);
        }
    }
 
}