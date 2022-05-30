/**
  * \file       convertCircle.cpp
  * \brief      Commande pour convertir des splines en lignes, en arcs, en polylignes 2D  et en polylignes 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       18 juillet 2019
 */

#pragma once
#define _USE_MATH_DEFINES
#include "convertSpline.h"


long splineToLine(
    AcDbSpline* spline,
    AcDbBlockTableRecord* blockTable,
    const bool& hasToErase,
    Adesk::Boolean isClosed,
    double const& fleche )
{
    //declaration et initialisation des variables
    long nbLine = 0;
    
    // Discretiser la spline
    AcGePoint3dArray ptArray = discretize( spline, fleche, isClosed );
    
    //recuperer le nombre de points de la spline
    int nbPoint = ptArray.logicalLength() - 1;
    
    //Quand on ne peut pas discretiser le spline
    if( nbPoint < 1 )
    {
        //Afficher une message
        acutPrintf( _T( "Le pas de discrétisation est trop grand \n" ) );
        
        //Fermer le polyligne
        spline->close();
        
        //Retourner nombre de ligne 0
        return nbPoint + 1 ;
    }
    
    for( int i = 0; i < nbPoint ;  i++ )
    {
        drawLine( ptArray.at( i ), ptArray.at( i + 1 ), blockTable, spline );
        nbLine++;
    }
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        spline->erase();
        
    spline->close();
    
    ptArray.setLogicalLength( 0 );
    return nbLine;
}


AcDb3dPolyline* splineToPoly3D(
    AcDbSpline* spline,
    const bool& hasToErase,
    double const& fleche )
{
    //discertiser la spline
    AcGePoint3dArray ptArray = discretize( spline, fleche );
    
    //créer la polylignes 3D
    Adesk::Boolean isClosed = spline->isClosed();
    
    //Si on ne peut pas discretiser le spline
    if( ptArray.size() < 1 )
    {
        //Afficher une message
        acutPrintf( _T( "Le pas de discrétisation est trop grand \n" ) );
        
        //Fermer le polyligne
        spline->close();
        
        //Retourner nombre de ligne 0
        return NULL;
    }
    
    AcDb3dPolyline* polyline3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, ptArray, isClosed );
    
    // on copie les propriétés de la polyligne
    polyline3D->setPropertiesFrom( spline );
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        spline->erase();
        
    spline->close();
    
    // on retourne la polyligne
    return polyline3D;
}


AcDbPolyline* splineToPoly2D(
    AcDbSpline* spline,
    const bool& hasToErase,
    double const& fleche )
{
    // Création de la polyligne
    AcDbPolyline* polyline = new AcDbPolyline();
    
    // Discretiser la spline
    AcGePoint3dArray ptArray = discretize( spline, fleche );
    
    //Si on ne peut pas discretiser le spline
    if( ptArray.size() < 1 )
    {
        //Afficher une message
        acutPrintf( _T( "Le pas de discrétisation est trop grand \n" ) );
        
        //Fermer le polyligne
        spline->close();
        
        //Retourner nombre de ligne 0
        return NULL;
    }
    
    // Boucle sur les points de la polyligne
    int length = ptArray.logicalLength();
    
    // on applique les mêmes propriétés que spline à la polyligne 2D
    polyline->setPropertiesFrom( spline );
    
    for( int i = 0; i < length; i++ )
        polyline->addVertexAt( i, getPoint2d( ptArray.at( i ) ) );
        
    //effacer si l'utilisateur le veut
    if( hasToErase )
        spline->erase();
        
    spline->close();
    
    // Retourne la polyligne
    return polyline;
}