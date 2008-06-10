#include <QtGui>
#include "QuadTreeNode.h"

QuadTreeNode::QuadTreeNode(double x, double y,double w, double h){	
	qx=x;
	qy=y;
	qw=w;
	qh=h;
	endOfTree = false;
}

	
QuadTreeNode::~QuadTreeNode(){		
}
	
void QuadTreeNode::buildQuadTree(QList<QuadTreeLeaf*> *list, double min_width, double min_height){
	qDebug()<< "min_width: "<<min_width << "min_height:" <<min_height;
	buildQuadTree(list,min_width, min_height,MAX_LEAFS,MAX_DEPTH);
}
void QuadTreeNode::buildQuadTree(QList<QuadTreeLeaf*> *list, double min_width, double min_height, int max_leafs, int max_depth){
	qleafs = list;
	if(list->size()> max_leafs && max_depth>=0 && qw/2.0 >= min_width && qh/2.0>=min_height){
		//qDebug()<<"split leafs: "<< list->size() << qx << qy <<qw <<qh;
		endOfTree = false;
		qchildren[0] = new QuadTreeNode(qx,			qy,			qw/2.0,	qh/2.0);
		qchildren[1] = new QuadTreeNode(qx+qw/2.0,	qy,			qw/2.0,	qh/2.0);
		qchildren[2] = new QuadTreeNode(qx+qw/2.0,	qy+qh/2.0,	qw/2.0,	qh/2.0);
		qchildren[3] = new QuadTreeNode(qx,			qy+qh/2.0,	qw/2.0,	qh/2.0);
		//| Q0 | Q1 |
		//-----------
		//| Q3 | Q2 |
		QList<QuadTreeLeaf*> *q0list = new QList<QuadTreeLeaf*>();
		QList<QuadTreeLeaf*> *q1list = new QList<QuadTreeLeaf*>();
		QList<QuadTreeLeaf*> *q2list = new QList<QuadTreeLeaf*>();
		QList<QuadTreeLeaf*> *q3list = new QList<QuadTreeLeaf*>();
		int i;
		QuadTreeLeaf* tmp;
		for (i=0;i<list->size();i++){
			tmp=list->at(i);
			
			if(tmp->isInside(qchildren[0]->qx,qchildren[0]->qy,qchildren[0]->qw,qchildren[0]->qh)){
				q0list->push_back(tmp);
			}
			if(tmp->isInside(qchildren[1]->qx,qchildren[1]->qy,qchildren[1]->qw,qchildren[1]->qh)){
				q1list->push_back(tmp);
			}
			if(tmp->isInside(qchildren[2]->qx,qchildren[2]->qy,qchildren[2]->qw,qchildren[2]->qh)){
				q2list->push_back(tmp);
			}
			if(tmp->isInside(qchildren[3]->qx,qchildren[3]->qy,qchildren[3]->qw,qchildren[3]->qh)){
				q3list->push_back(tmp);
			}
		}
		qchildren[0]->buildQuadTree(q0list, min_width, min_height, max_leafs,max_depth-1);
		qchildren[1]->buildQuadTree(q1list, min_width, min_height, max_leafs,max_depth-1);
		qchildren[2]->buildQuadTree(q2list, min_width, min_height, max_leafs,max_depth-1);
		qchildren[3]->buildQuadTree(q3list, min_width, min_height, max_leafs,max_depth-1);
	}else{
		endOfTree=true;
		qleafs = list;
		//qDebug() << "leafs: "<< qleafs->size()<< "depth: " << max_depth;
	}
}


void QuadTreeNode::getLeafs(double x, double y,QList <QuadTreeLeaf*> &list){

	if (endOfTree){
		
		int i;
		QuadTreeLeaf* tmp;
		for(i=0;i<qleafs->size();i++){
			
			tmp = qleafs->at(i);
			if(tmp->isInside(x,y)){
				list.push_back(tmp);
			}
		}
		//qDebug()<< "qleafs" << qleafs->size() << "list" << list->size();
	}else{	
		int i;
		bool found = false;
		for (i=0;i<4;i++){
			if ((x>=qchildren[i]->qx && x<= (qchildren[i]->qx + qchildren[i]->qw)) && (y>=qchildren[i]->qy && y<= (qchildren[i]->qy + qchildren[i]->qh))){
				qchildren[i]->getLeafs(x,y,list);
				found =true;
			}
		}
	}
}

