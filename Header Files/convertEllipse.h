/**
  * \file       convertCircle.h
  * \brief      Commande pour convertir des cercles en lignes, en arcs, en polylignes 2D  et en polylignes 3D
  * \author     Dinahasina
  * \version    0.1
  * \date       25 juillet 2019
 */
#pragma once
#include "selectEntity.h"
#include "ellipseEntity.h"
#include "helperFunctions.h"
#include "arcEntity.h"

/**
  * \brief  Convertir des ellipses en ligne
  * \param  ellipse pointeur sur l'ellipse
  * \param  blockTable Pointeur sur la base de donnees.
  * \param  tol Fleche de discretisation.
  * \param  hasToErase Bolleen pour connaitre si on veut suprimer ou pas l'ellipse
  * \return long Nombre de lignes obtenues.
  */
long ellipseToLine( AcDbEllipse* ellipse,
    AcDbBlockTableRecord* blockTable,
    const double& tol,
    const bool& hasToErase );
/**
  * \brief  Convertir des ellipses en poly2D
  * \param  ellipse pointeur sur l'ellipse
  * \param  tol Fleche de discretisation
  * \param  hasToErase Bolleen pour connaitre si on veut suprimer ou pas l'ellipse
  * \return AcDbPolyline Objet polyligne transforme
  */
AcDbPolyline* ellipseToPoly2D(
    AcDbEllipse*    ellipse,
    const double& tol,
    const bool& hasToErase );

/**
  * \brief  Convertir des ellipses en ligne
  * \param  ellipse pointeur sur l'ellipse
  * \param  blockTable Pointeur sur la base de donnees.
  * \param  tol Fleche de discretisation
  * \param  hasToErase Bolleen pour connaitre si on veut suprimer ou pas l'ellipse
  * \return long Nombre de arc obtenue.
  */
long ellipseToArc( AcDbEllipse* ellipse,
    AcDbBlockTableRecord* blockTable,
    const double& tol,
    const bool& hasToErase );

/**
  * \brief  Convertir des ellipses en polyligne 3d
  * \param  ellipse pointeur sur l'ellipse
  * \param  tol Fleche de discretisation
  * \param  hasToErase Bolleen pour connaitre si on veut suprimer ou pas l'ellipse
  * \return long Nombre de arc obtenue.
  */
AcDb3dPolyline* ellipseToPolyline( AcDbEllipse* ellipse,
    const double& tol,
    const bool& hasToErase );