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
/****************************************************************************
History
Revision 1.0 2008/02/20 Alessandro Maione, Federico Bellucci
FIRST RELEASE

****************************************************************************/

#include <QtGui>
#include <math.h>
#include <stdlib.h>
#include "filterqualitymapper.h"

// Constructor usually performs only two simple tasks of filling the two lists 
//  - typeList: with all the possible id of the filtering actions
//  - actionList with the corresponding actions. If you want to add icons to your filtering actions you can do here by construction the QActions accordingly

QualityMapperFilter::QualityMapperFilter() 
{ 
	typeList << FP_QUALITY_MAPPER;
  
  foreach(FilterIDType tt , types())
	  actionList << new QAction(filterName(tt), this);
}

// ST() must return the very short string describing each filtering action 
// (this string is used also to define the menu entry)
const QString QualityMapperFilter::filterName(FilterIDType filterId) const
{
  switch(filterId) {
		case FP_QUALITY_MAPPER :  return QString("Quality Mapper applier"); 
		default : assert(0); 
	}
  return QString("");
}

// Info() must return the longer string describing each filtering action 
// (this string is used in the About plugin dialog)
const QString QualityMapperFilter::filterInfo(FilterIDType filterId) const
{
  switch(filterId) {
		case FP_QUALITY_MAPPER :  return QString("The filter maps quality levels into colors using a colorband built from a transfer function (may be loaded from an external file) and colorizes the mesh vertexes. The minimum, medium and maximum quality values can be set by user to obtain a custom quality range for mapping");
		default : assert(0); 
	}
  return QString("");
}

const MeshFilterInterface::FilterClass QualityMapperFilter::getClass(QAction *a)
{
  switch(ID(a))
  {
    case FP_QUALITY_MAPPER :           return MeshFilterInterface::Quality;
		default :  assert(0);			return MeshFilterInterface::Generic;
  }
}


// This function define the needed parameters for each filter. Return true if the filter has some parameters
// it is called every time, so you can set the default value of parameters according to the mesh
// For each parameter you need to define, 
// - the name of the parameter, 
// - the string shown in the dialog 
// - the default value
// - a possibly long string describing the meaning of that parameter (shown as a popup help in the dialog)
void QualityMapperFilter::initParameterSet(QAction *action,MeshModel &m, FilterParameterSet & parlst) 
//void QualityMapperFilter::initParList(QAction *action, MeshModel &m, FilterParameterSet &parlst)
{
	 switch(ID(action))	 {
		case FP_QUALITY_MAPPER :
			{
				_meshMinMaxQuality = tri::Stat<CMeshO>::ComputePerVertexQualityMinMax(m.cm);

				parlst.addFloat("minQualityVal", _meshMinMaxQuality.minV, "Minimum mesh quality","The specified quality value is mapped in the <b>lower</b> end of the choosen color scale. Default value: the minumum quality value found on the mesh."  );
				parlst.addFloat("maxQualityVal", _meshMinMaxQuality.maxV, "Maximum mesh quality","The specified quality value is mapped in the <b>upper</b> end of the choosen color scale. Default value: the maximum quality value found on the mesh." );
				parlst.addFloat("midHandlePos", 50, "Gamma biasing (0..100)", "Defines a gamma compression of the quality values, by setting the position of the middle of the color scale. Value is defined as a percentage (0..100). Default value is 50, that corresponds to a linear mapping." );
				parlst.addFloat("brightness", 1.0f, "Mesh brightness", "must be between 0 and 2. 0 represents a completely dark mesh, 1 represents a mesh colorized with original colors, 2 represents a completely bright mesh");
				//setting default transfer functions names
				TransferFunction::defaultTFs[GREY_SCALE_TF] = "Grey Scale";
				TransferFunction::defaultTFs[MESHLAB_RGB_TF] = "Meshlab RGB";
				TransferFunction::defaultTFs[RGB_TF] = "RGB";
				TransferFunction::defaultTFs[RED_SCALE_TF] = "Red Scale";
				TransferFunction::defaultTFs[GREEN_SCALE_TF] = "Green Scale";
				TransferFunction::defaultTFs[BLUE_SCALE_TF] = "Blue Scale";
				TransferFunction::defaultTFs[FLAT_TF] = "Flat";

				QStringList tfList;
				tfList << "Custom Transfer Function File";
				for (int i=0; i<NUMBER_OF_DEFAULT_TF; i++ )
					//fetching and adding default TFs to TFList
					tfList << TransferFunction::defaultTFs[(STARTUP_TF_TYPE + i)%NUMBER_OF_DEFAULT_TF];
				
				parlst.addEnum( "TFsList", 1, tfList, "Transfer Function type to apply to filter", "Choose the Transfer Function to apply to the filter" );
				parlst.addString("csvFileName", "", "Custom TF Filename", "Filename of the transfer function to be loaded, used only if you have chosen the Custom Transfer Function." );
			}
			break;
											
		default : assert(0); break;
	}
}

// The Real Core Function doing the actual mesh processing.
// Apply color to mesh vertexes
bool QualityMapperFilter::applyFilter(QAction *filter, MeshModel &m, FilterParameterSet & par, vcg::CallBackPos *cb)
{
	Q_UNUSED(filter);
	Q_UNUSED(cb);

	EQUALIZER_INFO eqData;
	eqData.minQualityVal = par.getFloat("minQualityVal");
	eqData.midQualityPercentage = par.getFloat("midHandlePos");
	eqData.maxQualityVal = par.getFloat("maxQualityVal");
	eqData.brightness = par.getFloat("brightness");
	TransferFunction *transferFunction = 0;

	if ( par.getEnum("TFsList") != 0 )
	{
		//default TF
		//building new TF object from default Tf type
		int tfId = ((par.getEnum("TFsList")-1) + STARTUP_TF_TYPE) % NUMBER_OF_DEFAULT_TF;
		transferFunction = new TransferFunction( (DEFAULT_TRANSFER_FUNCTIONS) tfId );
	}
	else
	{
		//text TF
		QString csvFileName = par.getString("csvFileName");
		if ( csvFileName != "" &&  loadEqualizerInfo(csvFileName, &eqData) > 0 )
			{
				par.setFloat("minQualityVal", eqData.minQualityVal );
				par.setFloat("maxQualityVal", eqData.maxQualityVal );
				par.setFloat("midHandlePos", _meshMinMaxQuality.minV + ((_meshMinMaxQuality.maxV-_meshMinMaxQuality.minV)/eqData.midQualityPercentage));
				par.setFloat("brightness", eqData.brightness);

				//building new TF object from external file
				transferFunction = new TransferFunction( par.getString("csvFileName") );
			}
			else
			{
				errorMessage = "Something went wrong while trying to open the specified transfer function file";
				return false;
			}
	}
	// Applying colors
	applyColorByVertexQuality(m, transferFunction, par.getFloat("minQualityVal"), par.getFloat("maxQualityVal"), eqData.midQualityPercentage/100.0, par.getFloat("brightness"));

	//all done, deleting transfer function object
	if ( transferFunction )
	{
		delete transferFunction;
		transferFunction = 0;
	}

	return true;
}


Q_EXPORT_PLUGIN(QualityMapperFilter)
