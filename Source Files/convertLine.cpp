#pragma once

#include "convertLine.h"

AcDbPolyline* convertLineToPoly2D( AcDbLine* line, const bool& hasToErase )
{
    //recuperer les points de la ligne
    AcGePoint3d pt1 = line->startPoint();
    AcGePoint3d pt2 = line->endPoint();
    
    //Creer un objet polyligne2D
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //ajouter deux sommet de la polyline
    poly2D->addVertexAt( 0, getPoint2d( pt1 ) );
    poly2D->addVertexAt( 1, getPoint2d( pt2 ) );
    
    //mettre l'elevation de la polyline a 0
    poly2D->setElevation( 0 );
    
    //recuperer les proprietes de la ligne
    poly2D->setPropertiesFrom( line );
    
    //effacer puis fermer la ligne
	if( hasToErase )
		line->erase();

	// on ferme la ligne
    line->close();
    
    //returne la polyline transformer
    return poly2D;
}


AcDbPolyline* convertLineTo3DPoly2D( AcDbLine* line, const bool& hasToErase )
{

    //recuperer les points de la ligne
    AcGePoint3d pt1 = line->startPoint();
    AcGePoint3d pt2 = line->endPoint();
    
    //Creer un objet polyligne2D
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //elevation
    double elevation;
    
	//changer juste l'elevation
    if( pt1.z == pt2.z )
        elevation = pt1.z;    
    
    else
    {
        // longeur de la ligne
        double distance = getDistance2d( pt1, pt2 );
        
        //calcul du vecteur normal
        AcGeVector3d vecteurNormal;
        vecteurNormal.x = ( pt1.y - pt2.y ) / distance;
        vecteurNormal.y = ( pt2.x - pt1.x ) / distance;
        vecteurNormal.z = 0.0;
        
        //modifier le vecteur normal de l'objet polyligne
        poly2D->setNormal( vecteurNormal );
        
        //calcul de l'elevation
        elevation = ( pt1.x * vecteurNormal.x ) + ( pt1.y * vecteurNormal.y );
        
        // Calculer les coordonnées des extrêmités de la ligne dans le repère de la ligne
        pt1.x = ( vecteurNormal.x * pt1.y ) - ( vecteurNormal.y * pt1.x );
        pt1.y = pt1.z;
        pt2.x = ( vecteurNormal.x * pt2.y ) - ( vecteurNormal.y * pt2.x );
        pt2.y = pt2.z;
    }
    
    //ajouter deux sommets de la polyligne
    poly2D->addVertexAt( 0, getPoint2d( pt1 ) );
    poly2D->addVertexAt( 1, getPoint2d( pt2 ) );
    
    //recuperer la propriete de la ligne vers la polyligne
    poly2D->setPropertiesFrom( line );
    
    // Setter la valeur d'élévation
    poly2D->setElevation( elevation );

	if( hasToErase )
		line->erase();
		
	line->close();

    //returne la polyligne créée
    return poly2D;
}


AcDb3dPolyline* convertLineToPoly3D( AcDbLine* line, const bool& hasToErase )
{
    //creation ) la liste de sommets
    AcGePoint3dArray point3D;
    point3D.insertAt( 0, line->endPoint() );
    point3D.insertAt( 1, line->startPoint() );

    //si l'utilisateur choisi de supprimer les lignes
    if( hasToErase )
        line->erase();

	//declaration du polyline 3D
	AcDb3dPolyline* poly3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, point3D, Adesk::kFalse );

	//copier la propriete de l'arc
    poly3D->setPropertiesFrom( line );

	line->close();
    
    //Creation de la polyligne
    return poly3D;
}