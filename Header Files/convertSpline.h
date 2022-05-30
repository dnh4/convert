/**
  * \file       convertSpline.h
  * \brief      Commande pour convertir des splines en lignes, en arcs, en polylignes 2D  et en polylignes 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       16 juillet 2019

 */

#pragma once

#include "helperFunctions.h"
#include "splineEntity.h"
#include "lineEntity.h"


/**
  * \brief  Fait une conversion d'un spline en lignes.
  * \param  spline Pointeur sur le spline.
  * \param  blockTable Pointeur sur la base de données.
  * \param  fleche Flèche de discretisation.
  * \return long Nombre de lignes obtenues.
  */

long splineToLine( 
	AcDbSpline* spline, 
	AcDbBlockTableRecord* blockTable,
	const bool& hasToErase, 
	Adesk::Boolean isClosed ,double const& fleche = 0.1 );



/**
  * \brief  Fait une conversion d'un spline en polyligne 3D.
  * \param  spline Pointeur sur le spline.
  * \param  fleche Flèche de discretisation.
  * \return Pointeur sur la polyline 3D.
  */

AcDb3dPolyline* splineToPoly3D( 
	AcDbSpline* spline,
	const bool& hasToErase, 
	double const& fleche = 0.1 );

/**
  * \brief  Fait une conversion d'un spline en polyline 3D.
  * \param  spline Pointeur sur le spline.
  * \param  fleche Flèche de discretisation.
  * \return Pointeur sur la polyline 3D.
  */

AcDbPolyline* splineToPoly2D( 
	AcDbSpline* spline,  
	const bool& hasToErase,
	double const& fleche = 0.1 );