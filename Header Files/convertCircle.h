/**
  * \file       convertCircle.h
  * \brief      Fonctions pour convertir des cercles en lignes, en arcs, en polylignes 2D  et en polylignes 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       16 juillet 2019
 */

#pragma once

#include "helperFunctions.h"
#include "circleEntity.h"
#include "lineEntity.h"
#include "poly2DEntity.h"

/**
  * \brief  Fait une conversion d'un cercle en ligne.
  * \param  circle Pointeur sur le cercle.
  * \param  blockTable Pointeur sur la base de données.
  * \param  hasToErase True s'il faut supprimer le cercle, false sinon
  * \param  fleche Flèche de discretisation.
  * \return long Nombre de lignes obtenues.
  */
long circleToLine( AcDbCircle* circle,
	AcDbBlockTableRecord* blockTable,
	const bool& hasToErase = true,
	const double& fleche = 0.1 );

/**
  * \brief	Fait une conversion d'un cercle en polylignes en portion d'arcs
  * \param	circle Pointeur sur le cercle.
  * \param	blockTable Pointeur sur la base de données.
  * \param  hasToErase True s'il faut supprimer le cercle, false sinon
  * \param	nbVertex Nombre d'arcs demandés.
  * \return	Pointeur sur la polyligne 2D.
  */
AcDbPolyline* circleToPoly2D( AcDbCircle* circle,
	const bool& hasToErase = true,
	long const& nbVertex = 4 );


/**
  * \brief  Fait une conversion d'un cercle en polyligne.
  * \param  circle Pointeur sur le cercle.
  * \param  fleche Flèche de discretisation.
  * \param  hasToErase True s'il faut supprimer le cercle, false sinon
  * \param  isClosed booleen pour tester si la polyligne 3D est fermee.
  * \return AcDb3dPolyline* Pointeur sur la polyligne 3D.
  */
AcDb3dPolyline* circleToPoly3D( AcDbCircle* circle,
	const bool& hasToErase = true,
	const double& fleche = 0.1,
	const Adesk::Boolean& isClosed = kTrue );


/**
  * \brief  Fait une conversion d'un cercle en arc.
  * \param  circle Pointeur sur le cercle.
  * \param  blockTable Pointeur sur la base de données.
  * \param  hasToErase True s'il faut supprimer le cercle, false sinon
  * \param  nbArc Nombre d'arcs convertis.
  * \return long Nombre d'arcs  convertis.
  */
long circleToArc( AcDbCircle* circle,
	AcDbBlockTableRecord* blockTable,
	const bool& hasToErase = true,
	long const& nbArc = 4 ); 

/**
  * \brief  Fait une conversion d'un cercle en polyligne.
  * \param  circle Pointeur sur le cercle.
  * \param  fleche Flèche de discretisation.
  * \param  hasToErase True s'il faut supprimer le cercle, false sinon
  * \param  isClosed booleen pour tester si la polyligne 3D est fermee.
  * \return AcDb3dPolyline* Pointeur sur la polyligne 3D.
  */
AcDbPolyline* circleToPoly2DSeg( 
	AcDbCircle* circle,
	const bool& hasToErase = true,
	const double& fleche = 0.1,
	const Adesk::Boolean& isClosed = kTrue);