/*!
 * \file convertPoly2D.h
 * \brief commande pour convertir des polylinges2D
 * \author Dinahasina
 * \date 09 juillet 2019
 *
 * Commande pour convertir des arcs en lignes,
 *
 */

#include "helperFunctions.h"
#include "poly2DEntity.h"
#include "lineEntity.h"

/**
  * \brief Transforme une polyline2D en ligne
  * \brief La valeur des z des points de la polyligne 3D est la valeur de l'elevation de la polyligne 2D sous condition que la normal du plan de la poly2d est celui l'axe Z
  * \param poly2D   la polyligne2d que l'on veut changer 
  * \return Polylignes3Ds
  */
AcDb3dPolyline* poly2DtoPoly3D( AcDbPolyline*   poly2D,
	const bool& hasToErase,
	const double& tol );

/**
  * \brief Transforme une polyline2D en ligne
  * \param poly2D   la polyligne2d que l'on veut changer
  * \return Polylignes3Ds
  */

AcDbFace* poly2DToFace( AcDbPolyline* poly2D,
	const bool& hasToErase );

/**
  * \brief Transforme une polyline2D en polyligne 2D avec des segments selment
  * \param poly2D   la polyligne2d que l'on veut changer
  * \param tol   Tolerance de discretisation
  * \return 
  */
void poly2DToPoly2DSeg( 
	AcDbPolyline*& poly2D,
	const double& tol);


void coordinateInViewScu(AcGePoint3d& pt);