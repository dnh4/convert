/*!
 * \file helperFunctions.h
 * \brief Fichier dans lequel on place tous les include utiles
 * \author Dinahasina
 * \date 15 juillet 2019
 */

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <dbhatch.h>
#include <aced.h>
#include <acedads.h>
#include <tchar.h>


#include "pointEntity.h"
#include "progressBar.h"
#include "userInput.h"
#include "dataBase.h"
#include "file.h"
#include "acString.h"
#include "geometry.h"
#include "print.h"

using namespace std;

/**
  * \brief recherche un entier dans un vecteur d'entier
  * \param vecOfElements vecteur d'entier
  * \param element élément à chercher
  * \return pair contenant le résulat de la recherche et l'index si trouvé -1 sinon
  */
std::pair<bool, int> findInVector( const std::vector<int>& vecOfElements, const int& element );

/**
  * \brief recherche un Actring dans un vecteur de string
  * \param vecOfElements vecteur de string
  * \param element élément à chercher
  * \return bool , true pour trouver sinon false
  */
bool findStringInVector( std::vector<string>& vecOfElements, const AcString& element );


/**
  * \brief ouvre et lit un fichier de calque fml et renvoie les éléments (séparés par un;) dans un vecteur
  * \param filePath chemin du fichier de calque
  * \param element élément à chercher
  * \return vecteur contenant la liste des calques
  */
std::vector<string> fmlFileAsVector( const AcString& filePath, const AcString& sep = _T( ";" ) );

/**
  * \brief recherche un AcGePoint3d dans un tableau d'AcGePoint3d
  * \param AcGePoint3dArray tableau  d'AcGePoint3d
  * \param element élément à chercher
  * \return int, -1 si pas trouver et >= 0
  */
int findInVector3D( AcGePoint3dArray& vecOfElements,
    const AcGePoint3d& element,
    const double& tol = 0.01 );

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const double&           value,
    const AcString&           layer,
    const double&           rotation );

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const double&           value,
    const AcString&           layer,
    const double&           rotation,
    const double&           translation );

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const AcString&           value,
    const AcString&           layer,
    const double&           rotation );
