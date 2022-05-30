/**
  * \file       convertArc.cpp
  * \brief      Commande pour convertir des arc  en lignes
  * \author     Marielle H.R
  * \version    0.1
  * \date       11 juillet 2019
 */


#include "convertArc.h"


long arcToLine( AcDbBlockTableRecord* blockTable,
	AcDbArc* arc,
	const bool& hasToErase,
	const double& fleche )
{
	//initialiser le compteur sur la ligne
	long nLine = 0;
	
	//discretiser la courbe
	AcGePoint3dArray ptArray = discretize( *getAcGeArc( arc ), fleche );
	
	//boucle sur les sommets
	for( ; ptArray.logicalLength() > 1; ptArray.removeFirst() )
	{
		//ajouter à la base de données
		drawLine( ptArray.at( 0 ), ptArray.at( 1 ), blockTable, arc );
		
		//incrémenter le nombre de lignes
		nLine++;
	}
	
	//on supprime le tableau
	ptArray.setLogicalLength( 0 );
	
	// on le supprimpe si demander
	if( hasToErase )
		arc->erase();
		
	// on ferme l'objet
	arc->close();
	
	return nLine;
}


AcDbPolyline* arcTo2DPoly2D( AcDbArc* arc,
	const bool& hasToErase,
	const double& fleche )
{
	// Recuperer le premier point et le dernier point de l'arc dans le système de coordonées général
	AcGePoint3d pt1, pt2 ;
	arc->getStartPoint( pt1 );
	arc->getEndPoint( pt2 );
	
	// on crée une polyligne2D
	AcDbPolyline* poly2D = new AcDbPolyline();
	
	// Si l'arc est dans un plan horizontal on le transforme directement en polyligne2D par arcs
	if( pt1.z == pt2.z )
	{
		poly2D->addVertexAt( 0, getPoint2d( pt1 ), getBulgeAt( arc ) );
		poly2D->addVertexAt( 1, getPoint2d( pt2 ) );
		poly2D->setElevation( pt1.z );
	}
	
	else
	{
	
		// discretiser l'arc
		AcGePoint3dArray ptArray = discretize( arc , fleche );
		
		// on la met dans le plan z = 0
		poly2D->setElevation( 0.0 );
		
		// boucle sur les sommets
		for( ; ptArray.logicalLength() > 0; ptArray.removeFirst() )
			poly2D->addVertexAt( 0, getPoint2d( ptArray.at( 0 ) ) );
			
		// on supprime le tableau
		ptArray.setLogicalLength( 0 );
	}
	
	// supprime l'arc si c'est demandé
	if( hasToErase )
		arc->erase();
		
	//copier la propriete de l'arc
	poly2D->setPropertiesFrom( arc );
	
	// on ferme l'objet
	arc->close();
	
	return poly2D;
}


AcDbPolyline* arcTo3DPoly2D( AcDbArc* arc,
	const bool& hasToErase,
	const double& fleche )
{
	AcDbPolyline* poly2D = new AcDbPolyline();
	
	// Recuperer le premier point et le dernier point de l'arc dans le système de coordonées général
	AcGePoint3d pt1, pt2 ;
	arc->getStartPoint( pt1 );
	arc->getEndPoint( pt2 );
	
	// Points 1 et 2 dans le système de coordonées de l'arc
	AcGePoint2d pt21, pt22;
	
	// On définit l'élévations et le vecteur normal de l'arc
	double elevation = 0.0;
	AcGeVector3d normal ;
	
	// Si le plan de l'arc est horizontal, l'élévation correspond à l'altitude des points de l'arc
	if( pt1.z == pt2.z )
	{
		// On set l'élévation et le vecteur normal
		elevation = pt1.z;
		normal = arc->normal();
		
		// Calculer les cordonnées de deux points 2D
		pt21 = getPoint2d( pt1 );
		pt22 = getPoint2d( pt2 );
		
	}
	
	// Si l'arc est dans un plan oblique
	else
	{
		double distance = getDistance2d( pt1, pt2 );
		
		// Calcul de la normale
		normal.x = ( pt1.y - pt2.y ) / distance;
		normal.y = ( pt2.x - pt1.x ) / distance;
		normal.z = 0.0;
		
		// Calcul de l'elevation
		elevation = ( pt1.x * normal.x ) + ( pt1.y * normal.y );
		
		// Calcul des cordonnées des deux extrêmités de la polyligne mais dans
		// le repère associé à l'arc
		pt21.x = ( normal.x * pt1.y ) - ( normal.y * pt1.x );
		pt21.y = pt1.z;
		pt22.x = ( normal.x * pt2.y ) - ( normal.y * pt2.x );
		pt22.y = pt2.z;
	}
	
	// Création de la polyligne 2D
	poly2D->addVertexAt( 0, pt21, getBulgeAt( arc ) );
	poly2D->addVertexAt( 1, pt22 );
	poly2D->setPropertiesFrom( arc );
	poly2D->setElevation( elevation );
	poly2D->setNormal( normal );
	
	// on le supprimpe si demander
	if( hasToErase )
		arc->erase();
		
	// on ferme l'objet
	arc->close();
	
	return poly2D;
}


AcDb3dPolyline* arcToPoly3D( AcDbArc* arc,
	const bool& hasToErase,
	const double& fleche )
{
	AcDb3dPolyline* poly3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, discretize(  *getAcGeArc(arc) , fleche ), Adesk::kFalse );

	poly3D->setPropertiesFrom( arc );
	
	// on le supprimpe si demander
	if( hasToErase )
		arc->erase();
		
	// on ferme l'objet
	arc->close();
	
	return poly3D;
}


AcDbPolyline* arcToPoly2DSeg( 
	AcDbArc* arc,
	const bool& hasToErase,
	const double& fleche
)
{
	AcDbPolyline* poly2D = new AcDbPolyline();
	poly2D->convertFrom( new AcDb2dPolyline( AcDb::k2dSimplePoly,  discretize(  *getAcGeArc(arc) , fleche ), Adesk::kFalse ));
	poly2D->setPropertiesFrom( arc );
	
	// on le supprimpe si demander
	if( hasToErase )
		arc->erase();
		
	// on ferme l'objet
	arc->close();
	
	return poly2D;
}