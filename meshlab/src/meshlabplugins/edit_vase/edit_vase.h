#ifndef VASEPLUGIN_H
#define VASEPLUGIN_H

#include <common/interfaces.h> // meshlab stuff
#include <common/meshmodel.h> // CMesh0
#include <vasewidget.h> // vase widget GUI class
#include "balloon.h" // all balloon logic in here

using namespace vcg;

class Vase : public QObject, public MeshEditInterface, public MeshEditInterfaceFactory{
	Q_OBJECT
	Q_INTERFACES(MeshEditInterface)
    Q_INTERFACES(MeshEditInterfaceFactory)

private:
    // Instance of dialog window
    VaseWidget* gui;
    // Plugin defines this single action
    QAction* action;

public:
    //--- Dummy implementation of MeshEditInterface, passes all control to Widget
    static const QString Info(){ return tr("VASE"); }
    virtual bool StartEdit(MeshModel &m, GLArea* gla){
        gui = new VaseWidget( gla->window(), m, gla );
        // Try to force the widget on the right hand side
        //addDockWidget( Qt::RightDockWidgetArea, gui );
        return true;
    }
    virtual void EndEdit(MeshModel &, GLArea*){ delete gui; }
    virtual void Decorate(MeshModel& m, GLArea* gla){ gui->decorate(m,gla); }
    virtual void mousePressEvent(QMouseEvent *, MeshModel &, GLArea * );
    virtual void mouseMoveEvent(QMouseEvent *, MeshModel &, GLArea* );
    virtual void mouseReleaseEvent(QMouseEvent *, MeshModel &, GLArea* );

    //--- Dummy implementation of MeshEditInterfaceFactory, passes control to this MeshEditInterface
    Vase();
    virtual ~Vase(){ delete action; }
    virtual QList<QAction *> actions() const{
        QList<QAction *> actionList;
        return actionList << action;
    }
    virtual MeshEditInterface* getMeshEditInterface(QAction* ){ return this; }
    virtual QString getEditToolDescription(QAction *){  return this->Info(); }
};

#endif

