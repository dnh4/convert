/**
  * \file       convertPoly3D.h
  * \brief      Commande pour convertir des polylignes 3D en lignes, en polyligne2d  et en Face 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       09 juillet 2019
 */

#include "helperFunctions.h"
#include "poly3DEntity.h"
#include "lineEntity.h"
#include "arcEntity.h"


/**
  * \fn     AcDbPolyline* poly3DToPoly2D( AcDb3dPolyline * )
  * \brief  Fait une conversion d'une polyligne 3D en polyligne 2D.
  * \param  poly3D Pointeur sur la polyligne 3D.
  * \return Pointeur sur la polyligne 2D  .
  */
AcDbPolyline* poly3DToPoly2D( AcDb3dPolyline* poly3D, const bool& hasToErase );


/**
  * \fn long poly3DtoLine( AcDb3dPolyline* poly3D )
  * \brief Fait la conversion d'une polyligne 3D en ligne.
  * \param blockTable Pointeur sur la base de données.
  * \param poly3D Pointeur sur le polyligne 3D.
  * \return Nombre de lignes.
 */
long poly3DToLine( AcDbBlockTableRecord* blockTable, AcDb3dPolyline* poly3D, const bool& hasToErase );


/**
  * \fn AcDbFace* poly3DToFace3D( AcDb3dPolyline* poly3D )
  * \brief Fait la conversion d'une polyligne en Face 3D.
  * \param poly3D pointeur sur le polyligne 3D.
  * \return Pointeur sur la face 3D.
 */
AcDbFace* poly3DToFace3D( AcDb3dPolyline* poly3D );


/**
  * \fn Acad::ErrorStatus convertPoly3DToPoly2DArc( AcDb3dPolyline* poly3D )
  * \brief Fait la conversion d'une polyligne3D en polyligne2D avec Arc
  * \param poly3D pointeur sur le polyligne 3D.
  * \param tol valeur de tolerance pour calculer l'equivalence de longueur de segment
  * \return ErrorStatus
 */
Acad::ErrorStatus convertPoly3DToPoly2DArc( AcDb3dPolyline* poly3D,
    const double& tol = 0.01 );