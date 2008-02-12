#ifndef QUALITYMAPPERDIALOG_H
#define QUALITYMAPPERDIALOG_H

#include <QDockWidget>
#include <QGraphicsItem>
/*
#include <vcg/simplex/face/pos.h>
#include <vcg/simplex/face/topology.h>
//#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/closest.h>
#include <vcg/space/index/grid_static_ptr.h>
#include <vcg/complex/trimesh/allocate.h>
*/

#include <vcg/complex/trimesh/base.h>
#include <meshlab/meshmodel.h>
#include "ui_qualitymapperdialog.h"

#include <vcg/math/histogram.h>
#include "../../meshlabplugins/meshcolorize/curvature.h"		//<--contains Frange
#include <vcg/complex/trimesh/stat.h>
#include <meshlab/glarea.h>
#include "transferfunction.h"
#include "eqhandle.h"
#include "tfhandle.h"

using namespace vcg;


struct KNOWN_EXTERNAL_TFS
{
	QString path;
	QString name;
	KNOWN_EXTERNAL_TFS( QString p="", QString n="" )
	{
		path = p;
		name = n;
	}
};
#define KNOWN_EXTERNAL_TFSsize	sizeof(KNOWN_EXTERNAL_TFS)


#define GRAPHICS_ITEMS_LIST		QList<QGraphicsItem *>
#define TF_HANDLES_LIST			QList<TFHandle*>

class QualityMapperDialog : public QDockWidget
{
	Q_OBJECT

public:
	QualityMapperDialog(QWidget *parent=0, MeshModel *m=0, GLArea *gla=0);
	~QualityMapperDialog();
	
//	inline void setMesh(MeshModel *m){ mesh=m; }
	void ComputePerVertexQualityHistogram( CMeshO& m, Frange range, Histogramf *h, int bins=10000);

	void clearScene(QGraphicsScene *scene);
	void drawChartBasics(QGraphicsScene& scene, CHART_INFO *current_chart_info );	//controllare il puntatore alla vista (!!) MAL
	void initEqualizerHistogram();
	void drawTransferFunction();


private:
	Ui::QualityMapperDialogClass ui;
//	QualityMapperSettings _settings;

	Histogramf		*_equalizer_histogram;
	CHART_INFO		*_histogram_info;
	QGraphicsScene	_equalizerHistogramScene;	//questo equivale a graphics di .NET. O ne conserviamo una sola e la utilizziamo per disegnare tutto, o ne creiamo una ogni volta che dobbiamo disegnare qualcosa. forse sbaglio in pieno(??) indagare MAL
	EqHandle*		_equalizerHandles[NUMBER_OF_EQHANDLES];
	qreal			_equalizerMidHandlePercentilePosition;
	GRAPHICS_ITEMS_LIST _equalizerHistogramBars;

	TransferFunction *_transferFunction;
	CHART_INFO		*_transferFunction_info;
	QGraphicsScene	_transferFunctionScene;
	TF_HANDLES_LIST	_transferFunctionHandles[NUMBER_OF_CHANNELS];
	TFHandle		*_currentTfHandle;
	GRAPHICS_ITEMS_LIST _transferFunctionLines;
	GRAPHICS_ITEMS_LIST _transferFunctionBg;
	bool			_isTransferFunctionInitialized;
	QList<KNOWN_EXTERNAL_TFS>		_knownExternalTFs;


	GRAPHICS_ITEMS_LIST _removed_items;

	MeshModel		*mesh;
	GLArea			*gla;

	void initTF();
	void updateColorBand();
	void drawTransferFunctionBG ();
	void drawHistogramBars (QGraphicsScene&, CHART_INFO*, int minIndex, int maxIndex, QColor color = QColor(Qt::black));
	//void drawHistogramBarsSTRETCHED (QGraphicsScene&, CHART_INFO*, int minIndex, int maxIndex, QColor color = QColor(Qt::black));


	GRAPHICS_ITEMS_LIST	*clearScene(QGraphicsScene *scene, int toClean=0);
	GRAPHICS_ITEMS_LIST	*clearItems(int itemsToClear);
	void				deleteRemoveItems();
	void				moveAheadChannel( TF_CHANNELS channelCode );
	void				updateTfHandlesOrder(int channelCode);
	TFHandle			*addTfHandle(int channelCode, QPointF handlePos, TF_KEY *key, int zOrder );
	TFHandle			*addTfHandle(TFHandle *handle);
	TFHandle			*removeTfHandle(TFHandle *handle);

signals:
	void suspendEditToggle();

private slots:
	void on_removePointButton_clicked();
	void on_ySpinBox_valueChanged(double);
	void on_xSpinBox_valueChanged(double);
	void on_previewButton_clicked();
	void on_applyButton_clicked();

	void on_blueButton_toggled(bool checked);
	void on_greenButton_toggled(bool checked);
	void on_redButton_toggled(bool checked);

	void on_presetComboBox_textChanged(const QString &);

	void on_loadPresetButton_clicked();
	void on_savePresetButton_clicked();

	void on_addPointButton_clicked();
	void on_EQHandle_moved();

	void on_handle_released();

	void on_TfHandle_moved(TFHandle *sender);
	void on_TfHandle_clicked(TFHandle *sender);
	void on_TfHandle_doubleClicked(TFHandle *sender);
	
	void drawEqualizerHistogram();
	void drawGammaCorrection();
};

bool TfHandleCompare(TFHandle*h1, TFHandle*h2);

#endif // QUALITYMAPPERDIALOG_H
