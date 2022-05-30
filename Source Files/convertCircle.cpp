/**
  * \file       convertCircle.cpp
  * \brief      Commande pour convertir des cercles en lignes, en arcs, en polylignes 2D  et en polylignes 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       16 juillet 2019
 */

#pragma once
#define _USE_MATH_DEFINES
#include "convertCircle.h"


long circleToLine( AcDbCircle* circle,
    AcDbBlockTableRecord* blockTable,
    const bool& hasToErase,
    const double& fleche )
{
    //declaration et initialisation des variables
    long nbLine = 0;
    AcGePoint3dArray ptArray = discretize( circle, fleche );
    
    //boucle sur les sommets
    for( ; ptArray.logicalLength() > 1; ptArray.removeFirst() )
    {
        // ajouter � la base de donn�es
        drawLine( ptArray.at( 0 ), ptArray.at( 1 ), blockTable, circle );
        
        // Incr�menter le nombre de lignes converties
        nbLine++;
    }
    
    // supprime le cercle si c'est demand�
    if( hasToErase )
        circle->erase();
        
    // on ferme le cercle
    circle->close();
    
    //on supprime le tableau
    ptArray.setLogicalLength( 0 );
    
    return nbLine;
}


AcDbPolyline* circleToPoly2D( AcDbCircle* circle,
    const bool& hasToErase,
    long const& nbVertex )
{
    // Declarer et initialiser les variables
    long nbArc( nbVertex );
    double angle( 2 * M_PI / nbArc ), startAngle( 0 ), endAngle( angle );
    AcDbPolyline* polyline = new AcDbPolyline();
    AcGePoint2d startPoint, endPoint;
    AcGePoint3d nextPoint;
    
    // Recuperer le premier arc
    AcDbArc* arc = new AcDbArc( circle->center(), circle->radius(), 0, angle );
    
    // Recuperer le premier point de l'arc
    arc->getStartPoint( nextPoint );
    //convertir le point en 2D
    startPoint = getPoint2d( nextPoint );
    
    // Le dernier point du cercle coincide avec le premier point de l'arc
    endPoint = startPoint;
    
    // Boucle sur les arcs
    for( int i = 0;  i < nbArc; i++ )
    {
        // On ajoute le point � la polyligne2D
        polyline->addVertexAt( i, startPoint, getBulgeAt( arc ) );
        
        // On incr�mente l'angle de 2pi/nbArcs
        startAngle += angle;
        endAngle += angle;
        
        // On cr�e l'arc
        arc = new AcDbArc( circle->center(), circle->radius(), startAngle, endAngle );
        
        //on recupere le dernier point de l'arc et l'affecte au premier point du suivant en 2D
        arc->getStartPoint( nextPoint );
        startPoint = getPoint2d( nextPoint );
    }
    
    //On ajoute le dernier point � la polyligne
    polyline->addVertexAt( nbArc, endPoint );
    
    //copier les propri�t�s du cercle
    polyline->setPropertiesFrom( circle );
    
    // supprime le cercle si c'est demand�
    if( hasToErase )
        circle->erase();
        
    // on ferme le cercle
    circle->close();
    
    //retourner le r�sultat
    return polyline;
}


AcDb3dPolyline* circleToPoly3D( AcDbCircle* circle,
    const bool& hasToErase,
    const double& fleche,
    const Adesk::Boolean& isClosed )
{

    //declaration et initialisation des variables
    long nbLine = 0;
    
    //discretiser le cercle
    AcGePoint3dArray ptArray = discretize( circle, fleche );
    
    AcDb3dPolyline* poly3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, ptArray, isClosed );
    
    //on supprime le tableau
    ptArray.setLogicalLength( 0 );
    
    //on copie la propri�t� du cercle
    poly3D->setPropertiesFrom( circle );
    
    // supprime le cercle si c'est demand�
    if( hasToErase )
        circle->erase();
        
    // on ferme le cercle
    circle->close();
    
    return poly3D;
}


long circleToArc( AcDbCircle* circle,
    AcDbBlockTableRecord* blockTable,
    const bool& hasToErase,
    long const& nbArc )
{
    //declarer et initialiser les variables
    double angle( 2 * M_PI / nbArc ), startAngle( 0 ), endAngle( angle ) ;
    
    // On boucle sur le nombre d'arcs
    int i( 0 );
    
    for( ; i < nbArc; i++ )
    {
        // On cr�e un arc
        AcDbArc* arc = new AcDbArc( circle->center(), circle->radius(), startAngle, endAngle );
        
        // on l'ajoute � la Db
        addToModelSpaceAlreadyOpened( blockTable, arc );
        
        // On copie les propri�t�s du cercle et ferme l'arc
        arc->setPropertiesFrom( circle );
        arc->close();
        
        // On d�cale les angles de 2pi/nbArc
        startAngle += angle;
        endAngle += angle;
    }
    
    // supprime le cercle si c'est demand�
    if( hasToErase )
        circle->erase();
        
    // on ferme le cercle
    circle->close();
    
    //retourner le resultat
    return i;
}

AcDbPolyline* circleToPoly2DSeg(
    AcDbCircle* circle,
    const bool& hasToErase,
    const double& fleche,
    const Adesk::Boolean& isClosed )
{
    //declaration et initialisation des variables
    long nbLine = 0;
    
    //discretiser le cercle
    AcGePoint3dArray ptArray = discretize( circle, fleche );
    
    AcDbPolyline* poly = new AcDbPolyline();
    poly->convertFrom( new AcDb2dPolyline( AcDb::k2dSimplePoly,  ptArray, Adesk::kTrue ) );
    
    //on supprime le tableau
    ptArray.setLogicalLength( 0 );
    
    //on copie la propri�t� du cercle
    poly->setPropertiesFrom( circle );
    
    // supprime le cercle si c'est demand�
    if( hasToErase )
        circle->erase();
        
    // on ferme le cercle
    circle->close();
    
    return poly;
}