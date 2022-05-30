#include "convertEllipse.h"

long ellipseToLine( AcDbEllipse* ellipse,
    AcDbBlockTableRecord* blockTable,
    const double& tol,
    const bool& hasToErase )
{
    long nbSommet = 0;
    
    //discretiser la courbe
    AcGePoint3dArray ptArray = discretize( ellipse, tol );
    
    //boucler sur les sommets
	int length = ptArray.logicalLength();
    for( int i = 0; i < length; i++ )
    {
		int varI = i + 1;
		if( i == length - 1 )
			varI = 0;

        //dessiner la ligne
        drawLine( ptArray.at( i ), ptArray.at( varI ), blockTable, ellipse );
            
        //incrementer le nombre de sommet
        nbSommet ++;
    }
    
    //suprimer le tableau
    ptArray.setLogicalLength( 0 );
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        ellipse->erase();
        
    //fermer l'ellipse
    ellipse->close();
    
    return nbSommet;
}


AcDbPolyline* ellipseToPoly2D( AcDbEllipse* ellipse,
    const double& tol,
    const bool& hasToErase )
{
    //declarer le polyline
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //recuperer le point array
	AcGePoint3dArray pointArray =  discretize( ellipse, tol );
    
    //boucler sur les pointArray
    for( int i = 0; i < pointArray.size(); i++ )
        poly2D->addVertexAt( i,  pointArray.at( i ).convert2d() );
        
    //convertir l'objet
    poly2D->setPropertiesFrom( ellipse );
    
    //fermer le figure de la polyligne
    poly2D->setClosed( Adesk::kTrue );
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        ellipse->erase();
        
    //fermer l'ellipse
    ellipse->close();
    
    //retourner la valeur du polyline
    return poly2D;
}



long ellipseToArc( AcDbEllipse* ellipse,
    AcDbBlockTableRecord* blockTable,
    const double& tol,
    const bool& hasToErase )
{
    //discretiser l'ellipse
    AcGePoint3dArray ptArray = discretize( ellipse, tol );
    int siz = ptArray.size();
    
    for( int i = 0; i + 2 < siz ; i += 2 )
    {
        AcDbArc* arc;
        
		//Verifier la derniere points avant la derniere
        if( siz - 3 == i || siz - 2 == i )
            arc = arcByThreePoints( ptArray[siz - 3], ptArray[siz - 2], ptArray[0] );
        else
            arc = arcByThreePoints( ptArray.at( i ), ptArray.at( i + 1 ), ptArray.at( i + 2 ) );
            
        //recuperer les propriete de l'ellipse et les mettre sur l'arc
        arc->setPropertiesFrom( ellipse );
        
        //ajouter a la model space deja ouvert
        addToModelSpaceAlreadyOpened( blockTable, arc );
        
        arc->close();
    }
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        ellipse->erase();
        
    //fermer l'ellipse
    ellipse->close();
    
    return 0;
}

AcDb3dPolyline* ellipseToPolyline( AcDbEllipse* ellipse,
    const double& tol,
	const bool& hasToErase )
{
    AcDb3dPolyline* poly = new AcDb3dPolyline( AcDb::k3dSimplePoly, discretize( ellipse, tol ), Adesk::kTrue );
    
    //convertir l'objet
    poly->setPropertiesFrom( ellipse );
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        ellipse->erase();

    //fermer l'ellipse
    ellipse->close();
    
    //retourner la valeur du polyline
    return poly;
}