#ifndef VASEPLUGIN_H
#define VASEPLUGIN_H

#include <common/interfaces.h> // meshlab stuff
#include "meshlab/glarea.h" // required if you access members
#include <vector>

using namespace vcg;
using namespace std;

class ScanLineGeom{
public:
    // Screen offsets of scan points
    vector<Point2f> soff;
    Box2i bbox;
    ScanLineGeom(){}
    ScanLineGeom(int N, float aperture);
    bool isScanning;
    void render();
};

class VirtualScan : public QObject, public MeshEditInterface, public MeshEditInterfaceFactory{
	Q_OBJECT
	Q_INTERFACES(MeshEditInterface)
    Q_INTERFACES(MeshEditInterfaceFactory)

private:
    QAction*     action;
    // Keeps track of editing document
    MeshDocument*    md;
    // Keeps the scanned cloud
    MeshModel*    cloud;
    ScanLineGeom  sline;
    bool     isScanning;
    bool    sampleReady;
    // Function that performs the scan
    void scanpoints();
    // Timer to sample scanner
    QTimer *timer;

public:
    //--- Dummy implementation of MeshEditInterface
    static const QString Info(){ return tr("Virtual Scan "); }
    virtual bool StartEdit(MeshDocument &md, GLArea *parent);
    virtual void EndEdit(MeshModel &, GLArea* gla){
        // md->addNewMesh("Scan cloud", cloud);
        // md->addNewMesh("test");
        // gla->meshDoc.addNewMesh("test");
    }
    virtual void Decorate(MeshModel& m, GLArea* gla);
    virtual void mousePressEvent(QMouseEvent *, MeshModel &, GLArea * );
    virtual void mouseMoveEvent(QMouseEvent *, MeshModel &, GLArea* );
    virtual void mouseReleaseEvent(QMouseEvent *, MeshModel &, GLArea* );

    //--- Dummy implementation of MeshEditInterfaceFactory, passes control to this MeshEditInterface
    VirtualScan(){
        action = new QAction(QIcon(":/images/scan.png"),"Virtual scanner", this);
        action->setCheckable(true);
    }
    virtual ~VirtualScan(){ delete action; }
    virtual QList<QAction *> actions() const{
        QList<QAction *> actionList;
        return actionList << action;
    }
    virtual MeshEditInterface* getMeshEditInterface(QAction* ){ return this; }
    virtual QString getEditToolDescription(QAction *){  return this->Info(); }

private:
    //--- Virtual scan functions
    void scanPoints();
public slots:
    void readytoscan(){ sampleReady = true; }
};

#endif

