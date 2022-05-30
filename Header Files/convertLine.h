/*!
 * \file convertLine.h
 * \brief Fonctions pour convertir des lignes
 * \author Dinahasina
 * \date 15 juillet 2019
 */

#include "helperFunctions.h"
#include "lineEntity.h"

/**
  * \brief Fait une selection quelconque sur les lignes 2d
  * \param line une ligne que l'on veut modifier
  * \return l'objet polyline convertis
  */
AcDbPolyline* convertLineToPoly2D( AcDbLine* line, const bool& hasToErase );


/**
  * \brief  Fonction pour convertir une ligne en polyligne 2D dans le meme plan que la ligne.
  * C'est-a-dire que si la ligne est dans un plan oblique, la polyligne2D creee sera superposee sur la ligne.
  * \image  html C:\Futurmap\Dev-Outils\GStarCAD\GRX\convert\documentation\images\convertLineToPoly2D.png
  * Concretement,
  * \f[
        \vec{ ||OO'|| } = || \vec { OA } + \vec { AO' } ||
  \f]
  * \f[
        (\vec{OA} + \vec{AO'}).\vec{N} = ||\vec{OO'}.\vec{N}||
  \f]
  * Tout ca pour justifier l'expression suivante de l'elevation:
  * \f[
        \vec{OA}.\vec{N} = ||\vec{OO'}|| = E
  \f]

  * \f Changement de repere:
  \f]
  * \f[
        x' = cos(\theta) \vec{y} - sin(\theta) \vec{x}
  \f]
  * \param  line la ligne que l'on veut modifier
  * \param  hasToErase True s'il faut supprimer la ligne, false sinon
  * \return l'objet polyligne converti
  */
AcDbPolyline* convertLineTo3DPoly2D( AcDbLine* line, const bool& hasToErase );


/**
  * \brief Convertit une ligne en polyligne2D et la projette dans le plan horizontal z = 0,
  * La ligne est supprimee
  * \param line Ligne a convertir
  * \return Pointeur sur la polyligne2D
  */
AcDb3dPolyline* convertLineToPoly3D( AcDbLine* line, const bool& hasToErase );