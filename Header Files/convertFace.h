#pragma once
#include "helperFunctions.h"
#include "faceEntity.h"
#include "lineEntity.h"


int exportFaceToXml(
    ofstream*,
    string );

vector< AcString > getLayers();

/**
  * \fn     int faceToLine(  AcDbFace* face, const bool& hasToErase, AcDbBlockTableRecord* blockTable );
  * \brief  Fait une conversion de face en ligne.
  * \param  face Pointeur sur la face.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  blockTable pointeur sur la base de donne.
  * \return Nombre de ligne obtenu pendant la conversion.
  */
int faceToLine( AcDbFace* face, const bool& hasToErase, AcDbBlockTableRecord* blockTable );


/**
  * \fn     AcDbPolyline* faceToPoly2D( AcDbFace* face, const bool& hasToErase  );
  * \brief  fait une conversion de face en polyligne 2D.
  * \param  face Pointeur sur a face.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \return pointeur sur le polyligne 2D obtenue. 
  */
AcDbPolyline* faceToPoly2D( AcDbFace* face, const bool& hasToErase );


/**
  * \fn     AcDbPolyline* faceTo3DPoly2D( AcDbFace* face, const bool& hasToErase );
  * \brief  fait une conversion de face en 3D polyligne 2D.
  * \param  face Pointeur sur a face.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \param  blocktable la base de donne
  * \return pointeur sur le 3Dpoly2D obtenue.
  */
int faceTo3DPoly2D( AcDbFace* face,
	const bool& hasToErase,
	AcDbBlockTableRecord* blockTable);

/**
  * \fn     AcDb3dPolyline* faceToPoly3D( AcDbFace* face, const bool& hasToErase );
  * \brief  fait une conversion de face polyligne 3D.
  * \param  face Pointeur sur a face.
  * \param  hasToErase True s'il faut supprimer l'arc, false sinon
  * \return pointeur sur le poyligne 3D.
  */
AcDb3dPolyline* faceToPoly3D( AcDbFace* face, 
	const bool& hasToErase);