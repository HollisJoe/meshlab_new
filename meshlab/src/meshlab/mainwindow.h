/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//None of this should happen if we are compiling c, not c++
#ifdef __cplusplus
#include <GL/glew.h>
#include <QtScript>

#include <QDir>
#include <QMainWindow>
#include <QMdiArea>
#include <QStringList>
#include <QColorDialog>
#include "../common/pluginmanager.h"
#include "../common/mlparameter.h"
#include "glarea.h"
#include "layerDialog.h"
#include "stdpardialog.h"
#include "xmlstdpardialog.h"

#define MAXRECENTFILES 4

class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
class QSignalMapper;
class QProgressDialog;
class QHttp;


class MainWindow : public QMainWindow, MainWindowInterface
{
	Q_OBJECT

public:
	// callback function to execute a filter
	void executeFilter(QAction *action, RichParameterSet &srcpar, bool isPreview);
	void executeFilter(MeshLabXMLFilterContainer* mfc, FilterEnv& env, bool  isPreview);

  MainWindow();
	static bool QCallBack(const int pos, const char * str);
	const QString appName() const {return tr("MeshLab v")+appVer(); }
  const QString appVer() const {return tr("1.3.0"); }

signals:
	void dispatchCustomSettings(RichParameterSet& rps);

public slots:

  bool open(QString fileName=QString());
  bool openIn();
	bool openProject(QString fileName=QString());
	  bool importRaster(const QString& fileImg = QString());
	void saveProject();
	void delCurrentMesh();
	void delCurrentRaster();
	void endEdit();
	void updateCustomSettings();
  void updateDocumentScriptBindings() {if(currentViewContainer()) PM.updateDocumentScriptBindings(*meshDoc());}
  void evaluateExpression(const Expression& exp,Value** res);

private slots:

	//////////// Slot Menu File //////////////////////
  GLArea* newDocument(const QString& projName = QString());
  void reload();
	void openRecentFile();
	void openRecentProj();
	bool saveAs(QString fileName = QString(),const bool saveAllPossibleAttributes = false);
	bool save(const bool saveAllPossibleAttributes = false);
	bool saveSnapshot();
	///////////Slot Menu Edit ////////////////////////
	void applyEditMode();
	void suspendEditMode();
	///////////Slot Menu Filter ////////////////////////
	void startFilter();
	void applyLastFilter();
	void runFilterScript();
	void showFilterScript();
	void showScriptEditor();
  void showTooltip(QAction*);
  /////////// Slot Menu Render /////////////////////
	void renderBbox();
	void renderPoint();
	void renderWire();
	void renderFlat();
	void renderFlatLine();
	void renderHiddenLines();
	void renderSmooth();
	void renderTexture();
	void setLight();
	void setDoubleLighting();
	void setFancyLighting();
    void setColorMode(QAction *qa);
	void applyRenderMode();
	//void applyColorMode();
	void toggleBackFaceCulling();
  void toggleSelectFaceRendering();
  void toggleSelectVertRendering();
  void applyDecorateMode();
	///////////Slot Menu View ////////////////////////
	void fullScreen();
	void showToolbarFile();
	void showToolbarRender();
	void showInfoPane();
	void showTrackBall();
	void resetTrackBall();
	void showLayerDlg();
	///////////Slot Menu Windows /////////////////////
	void updateWindowMenu();
	void updateMenus();
	void activateSubFiltersMenu(const bool create,const bool act);
	void updateStdDialog();
	void setSplit(QAction *qa);
	void setUnsplit();
	void linkViewers();
	void viewFrom(QAction *qa);
	void readViewFromFile();
  void viewFromCurrentMeshShot();
  void viewFromCurrentRasterShot();
  void copyViewToClipBoard();
	void pasteViewFromClipboard();

	///////////Slot PopUp Menu Handles /////////////////////
	void splitFromHandle(QAction * qa);
	void unsplitFromHandle(QAction * qa);

	///////////Slot Menu Preferences /////////////////
	void setCustomize();
	///////////Slot Menu Help ////////////////////////
	void about();
	void aboutPlugins();
	void helpOnline();
	void helpOnscreen();
	void submitBug();
	void checkForUpdates(bool verboseFlag=true);

	///////////Slot General Purpose ////////////////////////

	void dropEvent ( QDropEvent * event );
	void dragEnterEvent(QDragEnterEvent *);
	void connectionDone(bool status);

	///////////Solt Wrapper for QMdiArea //////////////////
	void wrapSetActiveSubWindow(QWidget* window);
private:
    void createStdPluginWnd(); // this one is
	void createXMLStdPluginWnd();
	void initGlobalParameters();
    void createActions();
	void createMenus();
	void fillFilterMenu();
	void fillDecorateMenu();
	void fillRenderMenu();
	void fillEditMenu();
	void createToolBars();
	void loadMeshLabSettings();
  // void loadPlugins();
	void keyPressEvent(QKeyEvent *);
	void updateRecentFileActions();
	void updateRecentProjActions();
	void setCurrentFile(const QString &fileName);
	void saveRecentProjectList(const QString &projName);
	void addToMenu(QList<QAction *>, QMenu *menu, const char *slot);

	QHttp *httpReq;
	QBuffer myLocalBuf;
	int idHost;
	int idGet;
	bool VerboseCheckingFlag;

	MeshlabStdDialog *stddialog;
	MeshLabXMLStdDialog* xmldialog;
	static QProgressBar *qb;
	QMdiArea *mdiarea;
	LayerDialog *layerDialog;
	QSignalMapper *windowMapper;


    PluginManager PM;

		/* 
		Note this part should be detached from MainWindow just like the loading plugin part.
		
		For each running instance of meshlab, for the global params we have default (hardwired) values and current(saved,modified) values. 
		At the start up the initGlobalParameterSet function (of decorations and of glarea and of ... ) is called with the empty RichParameterSet defaultGlobalParams (to collect the default values) 
		At the start up the currentGlobalParams is filled with the values saved in the registry.
	*/
	
	RichParameterSet currentGlobalParams;
	RichParameterSet defaultGlobalParams;
	
	QByteArray toolbarState;								//stato delle toolbar e dockwidgets

	QDir lastUsedDirectory;  //This will hold the last directory that was used to load/save a file/project in

public:

  MeshDocument *meshDoc() {
    assert(currentViewContainer());
    return &currentViewContainer()->meshDoc;
  }

  const RichParameterSet& currentGlobalPars() const { return currentGlobalParams; }
  RichParameterSet& currentGlobalPars() { return currentGlobalParams; }
  const RichParameterSet& defaultGlobalPars() const { return defaultGlobalParams; }

	GLArea *GLA() const {
	  if(mdiarea->currentSubWindow()==0) return 0;
    MultiViewer_Container *mvc = currentViewContainer();
    if(!mvc) return 0;
    GLArea *glw =  qobject_cast<GLArea*>(mvc->currentView());
	  return glw;
	}

  MultiViewer_Container* currentViewContainer() const {
    if(mdiarea->currentSubWindow()==0) return 0;
    MultiViewer_Container *mvc = qobject_cast<MultiViewer_Container *>(mdiarea->currentSubWindow());
		if(!mvc){ 
			mvc = qobject_cast<MultiViewer_Container *>(mdiarea->currentSubWindow()->widget());
			return mvc;
		}
		else return 0;
	}


	void setHandleMenu(QPoint p, Qt::Orientation o, QSplitter *origin);

	const PluginManager& pluginManager() const { return PM; }

  static QStatusBar *&globalStatusBar()
  {
    static QStatusBar *_qsb=0;
    return _qsb;
  }
	QMenu* layerMenu() { return filterMenuLayer; }
	bool importMesh(const QString& fileName,MeshIOInterface *pCurrentIOPlugin,MeshModel* mm,int& mask,RichParameterSet* prePar);
	//void importMeshWithStandardParams(QString& fullPath,MeshModel* mm);
  //bool importRaster(const QString& fileImg);
	bool exportMesh(QString fileName,MeshModel* mod,const bool saveAllPossibleAttributes);
	bool importMeshWithStandardParams(QString& fullPath,MeshModel* mm);
	

private:
	//////// ToolBars ///////////////
	QToolBar *mainToolBar;
	QToolBar *renderToolBar;
	QToolBar *editToolBar;
	QToolBar *filterToolBar;

	///////// Menus ///////////////
	QMenu *fileMenu;
  QMenu *filterMenu;
  QMenu* recentProjMenu;
  QMenu* recentFileMenu;

  QMenu *filterMenuSelect;
  QMenu *filterMenuClean;
  QMenu *filterMenuCreate;
  QMenu *filterMenuRemeshing;
  QMenu *filterMenuPolygonal;
  QMenu *filterMenuColorize;
  QMenu *filterMenuSmoothing;
  QMenu *filterMenuQuality; 
  QMenu *filterMenuLayer;
  QMenu *filterMenuNormal;
  QMenu *filterMenuRangeMap;
  QMenu *filterMenuPointSet;
  QMenu *filterMenuSampling;
  QMenu *filterMenuTexture; 
  QMenu *filterMenuCamera;

	QMenu *editMenu;

  //Render Menu and SubMenu ////
	QMenu *shadersMenu;
	QMenu *renderMenu;
	QMenu *renderModeMenu;
	QMenu *lightingModeMenu;
	QMenu *colorModeMenu;

	//View Menu and SubMenu //////
	QMenu *viewMenu;
	QMenu *trackBallMenu;
	QMenu *logMenu;
	QMenu *toolBarMenu;
	//////////////////////////////
	QMenu *windowsMenu;
	QMenu *preferencesMenu;
	QMenu *helpMenu;
	QMenu *splitModeMenu;
	QMenu *viewFromMenu;
	//////////// Split/Unsplit Menu from handle///////////
	QMenu *handleMenu;
	QMenu *splitMenu;
	QMenu *unSplitMenu;

	//////////// Actions Menu File ///////////////////////
  QAction *newProjectAct;
  QAction *openProjectAct,*saveProjectAct,*saveProjectAsAct;
  QAction  *importMeshAct,   *exportMeshAct,  *exportMeshAsAct;
  QAction  *importRasterAct;
  QAction *closeProjectAct;
  QAction *reloadMeshAct;
  QAction *saveSnapshotAct;
  QAction *recentFileActs[MAXRECENTFILES];
  QAction *recentProjActs[MAXRECENTFILES];
	QAction *separatorAct;
	QAction *exitAct;
  //////
  QAction *lastFilterAct;
  QAction *runFilterScriptAct;
  QAction *showFilterScriptAct;
  QAction* showScriptEditAct;
  /////////// Actions Menu Edit  /////////////////////
  QAction *suspendEditModeAct;
	/////////// Actions Menu Render /////////////////////
	QActionGroup *renderModeGroupAct;
	QAction *renderBboxAct;
	QAction *renderModePointsAct;
	QAction *renderModeWireAct;
	QAction *renderModeHiddenLinesAct;
	QAction *renderModeFlatLinesAct;
	QAction *renderModeFlatAct;
	QAction *renderModeSmoothAct;
	QAction *renderModeTextureAct;
	QAction *setDoubleLightingAct;
	QAction *setFancyLightingAct;
  QAction *setLightAct;
	QAction *backFaceCullAct;
  QAction *setSelectFaceRenderingAct;
  QAction *setSelectVertRenderingAct;

	QActionGroup *colorModeGroupAct;
	QAction *colorModeNoneAct;
        QAction *colorModePerMeshAct;
	QAction *colorModePerVertexAct;
	QAction *colorModePerFaceAct;
	///////////Actions Menu View ////////////////////////
	QAction *fullScreenAct;
	QAction *showToolbarStandardAct;
	QAction *showToolbarRenderAct;
	QAction *showInfoPaneAct;
	QAction *showTrackBallAct;
	QAction *resetTrackBallAct;
	QAction *showLayerDlgAct;
	///////////Actions Menu Windows /////////////////////
	QAction *windowsTileAct;
	QAction *windowsCascadeAct;
	QAction *windowsNextAct;
	QAction *closeAllAct;
	QAction *setSplitHAct;
	QAction *setSplitVAct;
    QActionGroup *setSplitGroupAct;
	QAction *setUnsplitAct;
	///////////Actions Menu Windows -> Split/UnSplit from Handle ////////////////////////
	QActionGroup *splitGroupAct;
	QActionGroup *unsplitGroupAct;	

	QAction *splitUpAct;
	QAction *splitDownAct;

	QAction *unsplitUpAct;
	QAction *unsplitDownAct;

	QAction *splitRightAct;
	QAction *splitLeftAct;

	QAction *unsplitRightAct;
	QAction *unsplitLeftAct;

	///////////Actions Menu Windows -> View From ////////////////////////
	QActionGroup *viewFromGroupAct;
	QAction *viewTopAct;
	QAction *viewBottomAct;
	QAction *viewLeftAct;
	QAction *viewRightAct;
	QAction *viewFrontAct;
	QAction *viewBackAct;
  QAction *viewFromMeshAct;
  QAction *viewFromRasterAct;
	QAction *viewFromFileAct;

	///////////Actions Menu Windows -> Link/Copy/Paste View ////////////////////////
public:
	QAction *linkViewersAct;
private:
	QAction *copyShotToClipboardAct;
	QAction *pasteShotFromClipboardAct;

	///////////Actions Menu Preferences /////////////////
	QAction *setCustomizeAct;
	///////////Actions Menu Help ////////////////////////
	QAction *aboutAct;
	QAction *aboutPluginsAct;
	QAction *submitBugAct;
	QAction *onlineHelpAct;
	QAction *onscreenHelpAct;
	QAction *checkUpdatesAct;
	////////////////////////////////////////////////////
};

/// Event filter that is installed to intercept the open events sent directly by the Operative System
class FileOpenEater : public QObject
{
  Q_OBJECT

public:
  FileOpenEater(MainWindow *_mainWindow)
  {
    mainWindow= _mainWindow;
    noEvent=true;
  }

  MainWindow *mainWindow;
  bool noEvent;

protected:

  bool eventFilter(QObject *obj, QEvent *event)
  {
    if (event->type() == QEvent::FileOpen) {
      noEvent=false;
      QFileOpenEvent *fileEvent = static_cast<QFileOpenEvent*>(event);
      mainWindow->open(fileEvent->file());
      return true;
    } else {
      // standard event processing
      return QObject::eventFilter(obj, event);
    }
  }
};

#endif
#endif
