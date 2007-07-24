/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2006                                          \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
History

$Log$
Revision 1.125  2007/07/24 07:18:05  cignoni
moved matrix Tr inside mesh class

Revision 1.124  2007/07/13 15:15:52  cignoni
Corrected bug on bbox of multiple meshes

Revision 1.123  2007/07/10 07:19:11  cignoni
** Serious Changes **
again on the MeshDocument, the management of multiple meshes, layers, and per mesh transformation

Revision 1.122  2007/05/16 15:02:05  cignoni
Better management of toggling between edit actions and camera movement

Revision 1.121  2007/04/16 09:24:37  cignoni
** big change **
Added Layers managemnt.
Interfaces are changing...

Revision 1.120  2007/03/27 12:20:17  cignoni
Revamped logging iterface, changed function names in automatic parameters, better selection handling

Revision 1.119  2007/03/26 08:24:10  zifnab1974
When a user minimizes the window using a shortcut that uses modifiers (alt, ctrl, shift), the state of the button remained "pressed" after the window was reraised. Added a hideevent which resets the button state.

Revision 1.118  2007/03/20 16:22:34  cignoni
Big small change in accessing mesh interface. First step toward layers

Revision 1.117  2007/03/05 13:09:21  cignoni
Removed useless clearFocus

Revision 1.116  2007/03/05 11:12:56  cignoni
correct management of release of keyboard modifiers

Revision 1.115  2007/03/03 02:03:25  cignoni
Reformatted lower bar, added number of selected faces. Updated about dialog

Revision 1.114  2007/03/03 00:13:48  cignoni
quick patch of font size

Revision 1.113  2007/02/26 11:57:19  cignoni
Re enabled on screen help, moved back far plane

Revision 1.112  2007/02/26 01:20:59  cignoni
cursor added

Revision 1.111  2007/02/19 06:00:26  cignoni
Added cast for mac compiling

Revision 1.110  2006/12/21 00:37:27  cignoni
Correctly balanced a pushmatrix/popmatrix in the snapshot case

Revision 1.109  2006/12/12 00:03:19  cignoni
Accidentally deleted a quotation mark just before the commit...

Revision 1.108  2006/12/11 23:53:19  cignoni
Corrected gl error (a glEnd in excess) and corrected help entries

Revision 1.107  2006/10/26 12:06:02  corsini
add GLlightSettings structure

Revision 1.106  2006/10/10 19:55:02  cignoni
Corrected trackball bug, changed default background color.

Revision 1.105  2006/06/16 01:27:33  cignoni
Comment

Revision 1.104  2006/06/14 04:18:59  cignoni
removed no per vertex color bug

Revision 1.103  2006/06/13 13:50:01  cignoni
Cleaned FPS management

Revision 1.102  2006/06/12 15:18:36  cignoni
toggle between last editing mode

Revision 1.101  2006/06/07 08:49:25  cignoni
Disable rendering during processing and loading

Revision 1.100  2006/05/25 04:57:45  cignoni
Major 0.7 release. A lot of things changed. Colorize interface gone away, Editing and selection start to work.
Optional data really working. Clustering decimation totally rewrote. History start to work. Filters organized in classes.

Revision 1.99  2006/03/01 10:08:01  ponchio
WHEEL_DELTA -> WHEEL_STEP
as WHEEL_DELTA is already defined somewhere.

Revision 1.98  2006/02/28 13:29:36  ponchio
abs -> fabs

Revision 1.97  2006/02/22 23:45:28  glvertex
Bug solved in myGluPerspective. Now QuotedBox works propely.

Revision 1.96  2006/02/19 22:19:00  glvertex
Some help entries corrected

Revision 1.95  2006/02/19 02:54:27  ggangemi
Added glPushAttrib() and glPopAttrib for shader support

Revision 1.94  2006/02/16 15:05:30  glvertex
Added some entries in quickhelp

Revision 1.93  2006/02/15 01:37:55  glvertex
Restyled help

Revision 1.92  2006/02/13 15:37:18  cignoni
Restructured some functions (pasteTile, wheelevent,lightmode)
Added DoubleClick for zoom and center. Restructured all the keyboard modifier (removed currentButton)
Removed various gl state leaking

****************************************************************************/

#include <GL/glew.h>
#include <QtGui>

// widget capace di fare rendering di un Modello
#include <vcg/space/box3.h>
#include <wrap/gl/space.h>
#include "meshmodel.h"
#include "interfaces.h"
#include "glarea.h"
#include "layerDialog.h"

#include <wrap/gl/picking.h>

using namespace vcg; 

GLArea::GLArea(QWidget *parent)
: QGLWidget(parent)
{
  animMode=AnimNone;
	iRenderer=0; //Shader support
	iDecoratorsList=0;
	iEdit=0;
	currentEditor=0;
	suspendedEditor=false;
	cfps=0;
  lastTime=0;
  hasToPick=false;
	helpVisible=false;
	takeSnapTile=false;
	activeDefaultTrackball=true;
	infoAreaVisible = false;
	trackBallVisible = true;
	currentShader = NULL;
	lastFilterRef = NULL;
	//lastEditRef = NULL;
	currLogLevel = -1;
	setAttribute(Qt::WA_DeleteOnClose,true);
	// Projection Matrix starting settings
	//objDist = 3.f;
	fov = 60;
	clipRatioFar = 1;
	clipRatioNear = 1;
	nearPlane = .2f;
	farPlane = 5.f;
	layerDialog = new LayerDialog(this);
	}
		

/*
	This member returns the information of the Mesh in terms of VC,VQ,FC,FQ,WT
	where:
	VC = VertColor,VQ = VertQuality,FC = FaceColor,FQ = FaceQuality,WT = WedgTexCoord
*/
QString GLArea::GetMeshInfoString(int mask)
{
	QString info;
	if( mask & MeshModel::IOM_VERTQUALITY){info.append("VQ ");}
	if( mask & MeshModel::IOM_VERTCOLOR){info.append("VC ");}
	if( mask & MeshModel::IOM_FACEQUALITY){info.append("FQ ");}
	if( mask & MeshModel::IOM_FACECOLOR){info.append("FC ");}
	if( mask & MeshModel::IOM_WEDGTEXCOORD){info.append("WT ");}
	return info;
}

QSize GLArea::minimumSizeHint() const {return QSize(400,300);}
QSize GLArea::sizeHint() const				{return QSize(400,300);}


void GLArea::initializeGL()
{
	glShadeModel(GL_SMOOTH);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	static float diffuseColor[]={1.0,1.0,1.0,1.0};
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseColor);
	trackball.center=Point3f(0, 0, 0);
	trackball.radius= 1;

	trackball_light.center=Point3f(0, 0, 0);
	trackball_light.radius= 1;
  GLenum err = glewInit();
	if (err != GLEW_OK ) {
    assert(0);
  }
}



void GLArea::pasteTile()
{
	if (snapBuffer.isNull())
		snapBuffer = QImage(tileBuffer.width() * ss.resolution, tileBuffer.height() * ss.resolution, tileBuffer.format());

	uchar *snapPtr = snapBuffer.bits() + (tileBuffer.bytesPerLine() * tileCol) + ((totalCols * tileRow) * tileBuffer.numBytes());
	uchar *tilePtr = tileBuffer.bits();
    
	for (int y=0; y < tileBuffer.height(); y++)
	{
		memcpy((void*) snapPtr, (void*) tilePtr, tileBuffer.bytesPerLine());		
		snapPtr+=tileBuffer.bytesPerLine() * totalCols;
		tilePtr+=tileBuffer.bytesPerLine();
	}

	tileBuffer=QImage();

	tileCol++;

	if (tileCol >= totalCols)
	{
		tileCol=0;
		tileRow++;

		if (tileRow >= totalRows)
		{				
			QString outfile=QString("%1/%2%3.png")
        .arg(ss.outdir)
        .arg(ss.basename)
        .arg(ss.counter,2,10,QChar('0'));
			bool ret = snapBuffer.save(outfile,"PNG");		
			if (ret)
			{
				ss.counter++;
				if (ss.counter>999)
					ss.counter=0;
                log.Logf(GLLogStream::Info,"Snapshot saved to %s",outfile.toLocal8Bit().constData());
			}
			else
			{
			    log.Logf(GLLogStream::Error,"Error saving %s",outfile.toLocal8Bit().constData());
			}

			takeSnapTile=false;
			snapBuffer=QImage();
		}
	}
}


void GLArea::myGluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble fLeft, fRight, fBottom, fTop, left, right, bottom, top, xDim, yDim, xOff, yOff, tDimX, tDimY;
	
	fTop = zNear * tan(fovy * M_PI / 360.0);
	fLeft = -fTop * aspect;
	fBottom = -fTop;
	fRight = -fLeft;
	
	// Dimensione totale
	xDim = fabs(fLeft * 2);
	yDim = fabs(fTop * 2);
	
	// Dimensione di un tile
	tDimX = xDim / totalCols;
	tDimY = yDim / totalRows; 
	
	// Offset del tile
	yOff = tDimY * tileRow;
	xOff = tDimX * tileCol;

	// Nuovo frustum
	left = fLeft + xOff;
	right = fLeft + xOff + tDimX;
	bottom = fTop - yOff - tDimY;
	top = fTop - yOff;

	glFrustum(left, right, bottom, top, zNear, zFar);
}

void GLArea::paintGL()
{
  QTime time;
  time.start();
	
	//int lastTime=time.elapsed();
	initTexture();
	glClearColor(1.0,1.0,1.0,0.0);	//vannini: alpha was 1.0
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set Modelview and Projection matrix
	setView();

	// Enter in 2D screen Mode and
	// draws the background
	if(!takeSnapTile)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1,1,-1,1,-1,1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_TRIANGLE_STRIP);
			glColor(cs.bColorTop);  	glVertex2f(-1, 1);
			glColor(cs.bColorBottom);	glVertex2f(-1,-1);
			glColor(cs.bColorTop);		glVertex2f( 1, 1);
			glColor(cs.bColorBottom);	glVertex2f( 1,-1);
		glEnd();

		glPopAttrib();
		glPopMatrix(); // restore modelview
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}


	// ============== LIGHT TRACKBALL ==============
	// Apply the trackball for the light direction
	glPushMatrix();
	trackball_light.GetView();
	trackball_light.Apply(!(isDefaultTrackBall()));

	static float lightPosF[]={0.0,0.0,1.0,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosF);
	static float lightPosB[]={0.0,0.0,-1.0,0.0};
	glLightfv(GL_LIGHT1,GL_POSITION,lightPosB);

  if (!(isDefaultTrackBall()))
	{
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glColor3f(1,1,0);
    glDisable(GL_LIGHTING);
	  const int lineNum=3;
		glBegin(GL_LINES);
    for(unsigned int i=0;i<=lineNum;++i)
      for(unsigned int j=0;j<=lineNum;++j) {
        glVertex3f(-1.0f+i*2.0/lineNum,-1.0f+j*2.0/lineNum,-2);
        glVertex3f(-1.0f+i*2.0/lineNum,-1.0f+j*2.0/lineNum, 2);
      }
		glEnd();
    glPopAttrib();
	}
	glPopMatrix();
  // =============================================
	/// Compute BBox 
	Box3f FullBBox=meshDoc.bbox();
	foreach(MeshModel * mp, meshDoc.meshList) 
	 FullBBox.Add(mp->cm.Tr,mp->cm.bbox);
		
	// Finally apply the Trackball for the model
	trackball.GetView();
  glPushMatrix(); 
	trackball.Apply(trackBallVisible && !takeSnapTile && !(iEdit && !suspendedEditor));
	float d=2.0f/FullBBox.Diag();
	glScale(d);
	
	glTranslate(-FullBBox.Center());
  setLightModel();

	// Modify frustum... 
	if (takeSnapTile)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		myGluPerspective(fov, (GLdouble) curSiz.width() / (GLdouble) curSiz.height(), nearPlane, farPlane);
		glMatrixMode(GL_MODELVIEW);
	}

	// Set proper colorMode
	if(rm.colorMode != GLW::CMNone)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	}
	else glColor(Color4b::LightGray);
	
	if(rm.backFaceCull) glEnable(GL_CULL_FACE);
	              else glDisable(GL_CULL_FACE);
  if(!mm()->busy)
  {
    if(iRenderer && currentShader) {
		  glPushAttrib(GL_ALL_ATTRIB_BITS);
		  iRenderer->Render(currentShader, *mm(), rm, this); 
	  }

	  foreach(MeshModel * mp, meshDoc.meshList) 
			if(mp->visible)
				mp->Render(rm.drawMode,rm.colorMode,rm.textureMode);

	  if(iEdit) iEdit->Decorate(currentEditor,*mm(),this);
    

	  if(iRenderer) {
		  glPopAttrib();
		  glUseProgramObjectARB(0);
	  }

    // Draw the selection
    if(rm.selectedFaces)  mm()->RenderSelectedFaces();

	  if(iDecoratorsList){
		  pair<QAction *,MeshDecorateInterface *> p;
		  foreach(p,*iDecoratorsList){p.second->Decorate(p.first,*mm(),rm,this,qFont);}
	  }
  } ///end if busy 
	
	// ...and take a snapshot
	if (takeSnapTile)
	{
		glPushAttrib(GL_ENABLE_BIT);
		tileBuffer=grabFrameBuffer(true);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
		pasteTile();
		update();
		glPopAttrib();
	}
 glPopMatrix(); // now we are back in pre-trackball space
  if(hasToPick) 
  { // Double click move picked point to center
    Point3f pp;
    hasToPick=false;
    if(Pick<Point3f>(pointToPick[0],pointToPick[1],pp)) {
          trackball.Translate(-pp);
          trackball.Scale(1.25f);
          QCursor::setPos(mapToGlobal(QPoint(width()/2+2,height()/2+2)));
        }  
  }


	// Enter in 2D screen Mode again
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// Draw the log area background
	// on the bottom of the glArea
	if(infoAreaVisible)
	{
		displayInfo();    
		updateFps(time.elapsed());
	}
	
	// Finally display HELP if requested
	if (isHelpVisible()) displayHelp();

	// Closing 2D
	glPopAttrib();
	glPopMatrix(); // restore modelview
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
  assert(!glGetError());
}

void GLArea::displayInfo()
{	
	qFont.setFamily("Helvetica");
	qFont.setPixelSize(12);

	glBlendFunc(GL_ONE,GL_SRC_ALPHA);
	cs.lColor.V(3) = 128;	// set half alpha value
	glColor(cs.lColor);
	int lineNum =4;
	float lineSpacing = qFont.pixelSize()*1.5f;
	float barHeight = -1 + 2.0*(lineSpacing*(lineNum+.25))/float(curSiz.height());

	glBegin(GL_QUADS);
		glVertex2f(-1.f,barHeight); glVertex2f( 1.f,barHeight);
		glVertex2f( 1.f,-1.f);      glVertex2f(-1.f,-1.f);
	glEnd();
		
	// First the LOG
	glColor4f(1,1,1,1);

  float middleCol=curSiz.width()*0.40;
  float rightCol=curSiz.width()*0.85;
 	float startPos = curSiz.height()-(5+lineSpacing*(lineNum));

	renderText(20,startPos+ 1*lineSpacing,tr("LOG MESSAGES"),qFont);
	log.glDraw(this,currLogLevel,3,lineSpacing,qFont);

	if(meshDoc.size()==1) 
	{ 
		renderText(middleCol,startPos+ 1*lineSpacing,tr("Vertices: %1").arg(mm()->cm.vn),qFont);
		renderText(middleCol,startPos+ 2*lineSpacing,tr("Faces: %1").arg(mm()->cm.fn),qFont);
	}
	else
	{
		renderText(middleCol,startPos+ 1*lineSpacing,tr("<%1>").arg(QFileInfo(mm()->fileName.c_str()).fileName()),qFont);
		renderText(middleCol,startPos+ 2*lineSpacing,tr("Vertices: %1 (%2)").arg(mm()->cm.vn).arg(meshDoc.vn()),qFont);
		renderText(middleCol,startPos+ 3*lineSpacing,tr("Faces: %1 (%2)").arg(mm()->cm.fn).arg(meshDoc.fn()),qFont);
	}
	if(rm.selectedFaces)  
		 renderText(middleCol,startPos+ 4*lineSpacing,tr("Selected: %1").arg(mm()->cm.sfn),qFont);
	renderText(rightCol,startPos+ 4*lineSpacing,GetMeshInfoString(mm()->ioMask),qFont);

  renderText(rightCol,startPos+1*lineSpacing,QString("FOV: ")+QString::number((int)fov,10),qFont);
	if ((cfps>0) && (cfps<500)) 
			renderText(rightCol,startPos+2*lineSpacing,QString("FPS: %1").arg(cfps,7,'f',1),qFont);
}


void GLArea::resizeGL(int _width, int _height)
{
	glViewport(0,0, _width, _height);
	curSiz.setWidth(_width);
	curSiz.setHeight(_height);
}


void GLArea::displayHelp()
{
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT );
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBegin(GL_TRIANGLE_STRIP);
		glColor4f(.5f,.8f,1.f,.6f); glVertex2f(-1, 1);
		glColor4f(.0f,.0f,.0f,.0f); glVertex2f(-1,-1);
		glColor4f(.5f,.8f,1.f,.6f); glVertex2f(-.5f,1);
		glColor4f(.0f,.0f,.0f,.0f); glVertex2f(-.5f,-1);
	glEnd();


  float fontSpacingV = (curSiz.height()*.01f)+3;
	float hPosition = curSiz.width()*.1f;
	glColor(Color4b::White);
	qFont.setBold(true);renderText(2+hPosition-(qFont.pointSize()*9),1.5*fontSpacingV,QString("MeshLab Quick Help"),qFont);qFont.setBold(false);
  renderText(2,3*fontSpacingV,QString("Drag:"),qFont);								renderText(hPosition,3*fontSpacingV,QString("Rotate"),qFont);
	renderText(2,4.5*fontSpacingV,QString("Ctrl-Drag:"),qFont);					renderText(hPosition,4.5*fontSpacingV,QString("Pan"),qFont);
	renderText(2,6*fontSpacingV,QString("Shift-Drag:"),qFont);					renderText(hPosition,6*fontSpacingV,QString("Zoom"),qFont);
	renderText(2,7.5*fontSpacingV,QString("Alt-Drag:"),qFont);					renderText(hPosition,7.5*fontSpacingV,QString("Z-Panning"),qFont);
	renderText(2,9*fontSpacingV,QString("Ctrl-Shift-Drag:"),qFont);			renderText(hPosition,9*fontSpacingV,QString("Rotate light"),qFont);
	renderText(2,10.5*fontSpacingV,QString("Wheel:"),qFont);						renderText(hPosition,10.5*fontSpacingV,QString("Zoom"),qFont);
	renderText(2,12*fontSpacingV,QString("Shift-Wheel:"),qFont);				renderText(hPosition,12*fontSpacingV,QString("Change perspective"),qFont);
	renderText(2,13.5*fontSpacingV,QString("Ctrl-Wheel:"),qFont);				renderText(hPosition,13.5*fontSpacingV,QString("Move far clipping plane"),qFont);
	renderText(2,15*fontSpacingV,QString("Ctrl-Shift-Wheel:"),qFont);		renderText(hPosition,15*fontSpacingV,QString("Move near clipping plane"),qFont);
	renderText(2,16.5*fontSpacingV,QString("Double Click:"),qFont);			renderText(hPosition,16.5*fontSpacingV,QString("Center on mouse"),qFont);
	renderText(2,18*fontSpacingV,QString("F1:"),qFont);									renderText(hPosition,18*fontSpacingV,QString("Toggle this help"),qFont);
	renderText(2,19.5*fontSpacingV,QString("Alt+enter:"),qFont);						renderText(hPosition,19.5*fontSpacingV,QString("Enter/Exit fullscreen mode"),qFont);
	glPopAttrib();
}

void GLArea::saveSnapshot()
{ 
	totalCols=totalRows=ss.resolution;
	tileRow=tileCol=0;

	takeSnapTile=true;
	update();
}

Trackball::Button QT2VCG(Qt::MouseButton qtbt,  Qt::KeyboardModifiers modifiers)
{

	int vcgbt=Trackball::BUTTON_NONE;
	if(qtbt & Qt::LeftButton		) vcgbt |= Trackball::BUTTON_LEFT;
	if(qtbt & Qt::RightButton		) vcgbt |= Trackball::BUTTON_RIGHT;
	if(qtbt & Qt::MidButton			) vcgbt |= Trackball::BUTTON_MIDDLE;
	if(modifiers & Qt::ShiftModifier   ) vcgbt |= Trackball::KEY_SHIFT;
	if(modifiers & Qt::ControlModifier ) vcgbt |= Trackball::KEY_CTRL;
	if(modifiers & Qt::AltModifier     ) vcgbt |= Trackball::KEY_ALT;
	return Trackball::Button(vcgbt);
}


void GLArea::closeEvent(QCloseEvent *event)
{
	bool close = true;
	if(isWindowModified())
	{
		if(QMessageBox::question(
                this,
                tr("MeshLab"),
                tr("File %1 modified.\n\n"
                   "Continue without saving?")
                .arg(getFileName()),
								QMessageBox::Yes|QMessageBox::Default,
								QMessageBox::No|QMessageBox::Escape,
								QMessageBox::NoButton) == QMessageBox::No)
		{
			close = false;	// don't close please!
		}
	}

	event->ignore();
	if(close)
	{
		event->accept();
	}
}

void GLArea::keyReleaseEvent ( QKeyEvent * e )
{
	e->ignore();
      if(e->key()==Qt::Key_Control) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ControlModifier ) );
      if(e->key()==Qt::Key_Shift) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::ShiftModifier ) );
      if(e->key()==Qt::Key_Alt) trackball.ButtonUp(QT2VCG(Qt::NoButton, Qt::AltModifier ) );
}	

void GLArea::keyPressEvent ( QKeyEvent * e )
{
	e->ignore();
      if(e->key()==Qt::Key_Control) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ControlModifier ) );
      if(e->key()==Qt::Key_Shift) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::ShiftModifier ) );
      if(e->key()==Qt::Key_Alt) trackball.ButtonDown(QT2VCG(Qt::NoButton, Qt::AltModifier ) );
}

void GLArea::mousePressEvent(QMouseEvent*e)
{
  e->accept();
	setFocus();
  if(iEdit && !suspendedEditor)  iEdit->mousePressEvent(currentEditor,e,*mm(),this);
  else {          
	    if ((e->modifiers() & Qt::ShiftModifier) && (e->modifiers() & Qt::ControlModifier) && 
          (e->button()==Qt::LeftButton) )
            activeDefaultTrackball=false;
	      else activeDefaultTrackball=true;
      
	    if (isDefaultTrackBall())
          trackball.MouseDown(e->x(),height()-e->y(), QT2VCG(e->button(), e->modifiers() ) );
	    else trackball_light.MouseDown(e->x(),height()-e->y(), QT2VCG(e->button(), Qt::NoModifier ) );
  }
	update();
}

void GLArea::mouseMoveEvent(QMouseEvent*e)
{ 
	if(e->buttons() | Qt::LeftButton) 
	{
      if(iEdit && !suspendedEditor) iEdit->mouseMoveEvent(currentEditor,e,*mm(),this);
      else {
		    if (isDefaultTrackBall()) 
			{
			  trackball.MouseMove(e->x(),height()-e->y());
			  setCursorTrack(trackball.current_mode);
			}
		    else trackball_light.MouseMove(e->x(),height()-e->y());
        update();
      }
	}
}
// When mouse is released we set the correct mouse curson
void GLArea::mouseReleaseEvent(QMouseEvent*e)
{
  //clearFocus();
	activeDefaultTrackball=true;
	if(iEdit && !suspendedEditor) iEdit->mouseReleaseEvent(currentEditor,e,*mm(),this);
    else {
          if (isDefaultTrackBall()) trackball.MouseUp(e->x(),height()-e->y(), QT2VCG(e->button(), e->modifiers() ) );
	        else trackball_light.MouseUp(e->x(),height()-e->y(), QT2VCG(e->button(),e->modifiers()) );
		      setCursorTrack(trackball.current_mode);
        }
	
	update();
}

void GLArea::wheelEvent(QWheelEvent*e)
{
	const int WHEEL_STEP = 120;
	float notch = e->delta()/ float(WHEEL_STEP);
  switch(e->modifiers())
  {
    case Qt::ShiftModifier + Qt::ControlModifier : clipRatioFar  *= powf(1.2f, notch); break;
    case Qt::ControlModifier                     : clipRatioNear *= powf(1.2f, notch); break;
    case Qt::ShiftModifier                       : fov = math::Clamp(fov*powf(1.2f,notch),5.0f,90.0f); break;
    default:
      trackball.MouseWheel( e->delta()/ float(WHEEL_STEP)); 
      break;
	}
	updateGL();
}		


void GLArea::mouseDoubleClickEvent ( QMouseEvent * e ) 
{
  hasToPick=true;
  pointToPick=Point2i(e->x(),height()-e->y());
	updateGL();
}

void GLArea::setCursorTrack(vcg::TrackMode *tm)
{
 static QMap<QString,QCursor> curMap;
 if(curMap.isEmpty())
 {
  curMap[QString("")]=QCursor(Qt::ArrowCursor);	
  curMap["SphereMode"]=QCursor(QPixmap(":/images/cursors/plain_trackball.png"),1,1);	
  curMap["PlaneMode"]=QCursor(QPixmap(":/images/cursors/plain_pan.png"),1,1);	
  curMap["ScaleMode"]=QCursor(QPixmap(":/images/cursors/plain_zoom.png"),1,1);	
 }
 if(tm) setCursor(curMap[tm->Name()]);
 else setCursor(curMap[""]);

}

void GLArea::setDrawMode(vcg::GLW::DrawMode mode)
{
	rm.drawMode = mode;
	updateGL();
}


void GLArea::setColorMode(vcg::GLW::ColorMode mode)
{
	rm.colorMode = mode;
	updateGL();
}

// Texture loading done during the first paint.
void GLArea::initTexture()
{
  if(!mm()->cm.textures.empty() && mm()->glw.TMId.empty()){
		glEnable(GL_TEXTURE_2D);
		for(unsigned int i =0; i< mm()->cm.textures.size();++i){
			QImage img, imgScaled, imgGL;
			img.load(mm()->cm.textures[i].c_str());
      // image has to be scaled to a 2^n size. We choose the first 2^N <= picture size.
      int bestW=pow(2.0,floor(::log(double(img.width() ))/::log(2.0)));
      int bestH=pow(2.0,floor(::log(double(img.height()))/::log(2.0)));
      imgScaled=img.scaled(bestW,bestH,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
			imgGL=convertToGLFormat(imgScaled);
			qDebug("loaded texture %s. with id %i w %i  h %i",mm()->cm.textures[i].c_str(),i, imgGL.width(), imgGL.height());
			mm()->glw.TMId.push_back(0);

			glGenTextures( 1, (GLuint*)&(mm()->glw.TMId.back()) );
			glBindTexture( GL_TEXTURE_2D, mm()->glw.TMId.back() );
			glTexImage2D( GL_TEXTURE_2D, 0, 3, imgGL.width(), imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits() );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgGL.width(), imgGL.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits() );
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			qDebug("loaded texture %s. in %i",mm()->cm.textures[i].c_str(),mm()->glw.TMId[i]);
		}
	}
	glDisable(GL_TEXTURE_2D);
}

void GLArea::setTextureMode(vcg::GLW::TextureMode mode)
{
	rm.textureMode = mode;
	updateGL();
}

void GLArea::setLight(bool state)
{
	rm.lighting = state;
	updateGL();
}

void GLArea::setLightMode(bool state,LightingModel lmode)
{
	switch(lmode)
	{
	  case LDOUBLE:		rm.doubleSideLighting = state;	break;
	  case LFANCY:		rm.fancyLighting = state; break;
	}
	updateGL();
}

void GLArea::setBackFaceCulling(bool enabled)
{
	rm.backFaceCull = enabled;
	updateGL();
}

void GLArea::setSelectionRendering(bool enabled)
{
	rm.selectedFaces = enabled;
	updateGL();
}

void GLArea::setLightModel()
{	
	glDisable(GL_LIGHTING);
	if (rm.lighting) 
	{
		glEnable(GL_LIGHTING);

		if (rm.doubleSideLighting) 
			glEnable(GL_LIGHT1);
		else 
			glDisable(GL_LIGHT1);

		if(rm.fancyLighting)
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, ls.ambientFancyFront);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, ls.diffuseFancyFront);
			glLightfv(GL_LIGHT0, GL_SPECULAR, ls.specularFancyFront);

			glLightfv(GL_LIGHT1, GL_AMBIENT, ls.ambientFancyBack);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, ls.diffuseFancyBack);
			glLightfv(GL_LIGHT1, GL_SPECULAR, ls.specularFancyBack);
		}
		else
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, ls.ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, ls.diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, ls.specular);

			glLightfv(GL_LIGHT1, GL_AMBIENT, ls.ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, ls.diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, ls.specular);
		}
	}
}


void GLArea::setCustomSetting(const ColorSetting & s)
{
	cs.bColorBottom = s.bColorBottom;
	cs.bColorTop = s.bColorTop;
	cs.lColor = s.lColor;
}

void GLArea::setSnapshotSetting(const SnapshotSetting & s)
{
	ss=s;
}

void GLArea::setView()
{
	GLfloat fAspect = (GLfloat)curSiz.width()/ curSiz.height();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Si deve mettere la camera ad una distanza che inquadri la sfera unitaria bene.
	
	float ratio = 1.75f;
	float objDist = ratio / tanf(vcg::math::ToRad(fov*.5f));

	nearPlane = objDist - 2.f*clipRatioNear;
	farPlane =  objDist + 10.f*clipRatioFar;
	if(nearPlane<=objDist*.1f) nearPlane=objDist*.1f;

	if(fov==5)		glOrtho(-ratio*fAspect,ratio*fAspect,-ratio,ratio,objDist - 2.f*clipRatioNear, objDist+2.f*clipRatioFar);
	   else    		gluPerspective(fov, fAspect, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, objDist,0, 0, 0, 0, 1, 0);
}

void GLArea::updateFps(float deltaTime)
{
 	static float fpsVector[10];
	static int j=0;
	float averageFps=0;
  if (deltaTime>0) {
    fpsVector[j]=deltaTime;
	  j=(j+1) % 10;
  }
	for (int i=0;i<10;i++) averageFps+=fpsVector[i];
	cfps=1000.0f/(averageFps/10);
  lastTime=deltaTime;
}

void GLArea::resetTrackBall(){trackball.Reset();updateGL();}

void GLArea::hideEvent(QHideEvent * /*event*/)
{
	trackball.current_button=0;
}

