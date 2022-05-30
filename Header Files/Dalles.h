#pragma once
#include "blockEntity.h"

struct Decoup
{
    AcString filename;
    AcDbPolyline* polyDalle;
    AcDbExtents dalleBox;
    vector<AcDbEntity*> entityVec;
    AcDbText* textDalle;
};

struct Dalle
{
    AcString dallename;
    AcString dallelayer;
    AcString objecttype;
    AcDbExtents dallebox;
    AcDbObjectIdArray entitiesId;
    
};


/**
  * \brief Exporter les polylignes dalles
  * \param ssdalle Selection des polylignes dans le calque "Dalle"
  * \param sstext  Selection des textes contenus dans le dalle
  * \param vectDalle Liste des structures contenant les dalles
  * \return eOk si la fonction se termine avec succès
  */
Acad::ErrorStatus exportDalles(
    ads_name& ssdalle,
    ads_name& sstext,
    vector<Dalle>& vectDalle );

/**
  * \brief Trier les dalles
  * \param vectDalle Liste des structures contenant les dalles
  * \return eOk si la fonction se termine avec succès
  */
Acad::ErrorStatus sortDalle( vector<Dalle>& vectDalle );


Acad::ErrorStatus ExportEntitiesInDalle(
    ads_name& ssentities,
    vector<Dalle>& vectDalle,
    const bool& tileCurve = false,
    const bool& tileAll = false );



/**
  * \brief Inserer les entités dans des dwg
  * \param vectDalle Liste des structures contenant les dalles
  * \return eOk si la fonction se termine avec succès
  */
Acad::ErrorStatus insertEntitiesInDalle(
    vector<Dalle>& vectDalle,
    const bool& tileCurve = false );

