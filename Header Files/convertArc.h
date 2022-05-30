/**
  * \file       convertArc.h
  * \brief      Fonctions pour convertir des arcs
  * \author     Marielle H.R
  * \version    0.1
  * \date       11 juillet 2019
 */

#include "helperFunctions.h"
#include "arcEntity.h"
#include "lineEntity.h"


/**
  * \fn     long* arcToLine( AcDbBlockTableRecord* blockTable, AcDbArc* arc, const double& fleche = 0.1 )
  * \brief  Fait une conversion d'un arc  en ligne.
  * \param  blockTable Pointeur sur la base de donn�es.
  * \param  arc Pointeur sur l'arc.
  * \param  fleche Fl�che de discretisation.
  * \return Nombre de ligne obtenues apr�s la conversion  .
  */
long arcToLine( AcDbBlockTableRecord* blockTable,
    AcDbArc* arc,
    const bool& hasToErase = true,
    const double& fleche = 0.1 );


/**
  * \fn     AcDbPolyline* arcToPoly2D( AcDbArc* arc );
  * \brief  Fait une conversion d'un arc  en polyligne 2D.
  * \param  arc Pointeur sur l'arc.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  fleche Fleche de discretisation.
  * \return Pointeur sur la polyligne 2D  .
  */
AcDbPolyline* arcTo2DPoly2D( AcDbArc* arc,
    const bool& hasToErase = true,
    const double& fleche = 0.1 );


/**
  * \fn     AcDbPolyline* arcTo3DPoly2D( AcDbArc* arc );
  * \brief  Fait une conversion d'un arc en polyligne 2D.
  * \param  arc Pointeur sur l'arc.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  fleche Fleche de discretisation
  * \return Pointeur sur la polyligne 2D  .
  */
AcDbPolyline* arcTo3DPoly2D( AcDbArc* arc,
    const bool& hasToErase = true,
    const double& fleche = 0.1 );


/**
  * \fn     AcDb3dPolyline* arcToPoly3D( AcDbArc* arc, const double& fleche = 0.1 )
  * \brief  Fait une conversion d'un arc en Polyligne3D.
  * \param  arc Pointeur sur l'arc.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  fleche Flèche de discretisation.
  * \return Pointeur sur une polyligne 3D.
  */
AcDb3dPolyline* arcToPoly3D( AcDbArc* arc,
    const bool& hasToErase = true,
    const double& fleche = 0.1 );

/**
  * \fn     AcDbPolyline* arcToPoly2DSeg( AcDbArc* arc, const double& fleche = 0.1 )
  * \brief  Fait une conversion d'un arc en segment de polyline2D.
  * \param  arc Pointeur sur l'arc.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  fleche Flèche de discretisation.
  * \return Pointeur sur une polyligne 3D.
  */
AcDbPolyline* arcToPoly2DSeg(
	AcDbArc* arc,
    const bool& hasToErase = true,
    const double& fleche = 0.1
);