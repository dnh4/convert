#include "convertPoly2D.h"


AcDb3dPolyline* poly2DtoPoly3D( AcDbPolyline* poly2D,
    const bool& hasToErase,
    const double& tol )
{
    if( !poly2D )
        return NULL;
        
    //recuperer la listes des sommets
    AcGePoint3dArray pointArray;
    
    //declarations des segments
    AcDbPolyline::SegType   segType;
    AcGePoint3d             pt3D;
    AcGeCircArc3d           arcSeg;
    
    //declarer l'objet poly3D
    AcDb3dPolyline* poly3D;
    
    // Recuperer l'elevation de la poly2d
    double elevation = poly2D->elevation();
    
    // discrétise la poly2D
    AcGePoint3dArray ptTemp = discretize3D( poly2D, tol );
    
    for( int i = 0; i < ptTemp.length(); i++ )
    {
        //AcGePoint2d pt2d = ptTemp[ i ];
        AcGePoint3d pt3d = ptTemp[i];
        //AcGePoint3d( pt2d.x, pt2d.y, elevation );
        pointArray.append( pt3d );
    }
    
    // Suprimmer et fermer la polyligne 2D
    if( hasToErase )
        poly2D->erase();
        
    // on crée la poly3D
    poly3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, pointArray, Adesk::kFalse );
    pointArray.setLogicalLength( 0 );
    
    //recuperer la propriete du poly2D
    poly3D->setPropertiesFrom( poly2D );
    
    //Verifier si la poly2D est fermer ou pas
    if( poly2D->isClosed() )
        poly3D->makeClosed();
        
    // On ferme la polyligne
    poly2D->close();
    
    // Retourner la polyligne 3d obtenue
    return poly3D;
}


AcDbFace* poly2DToFace( AcDbPolyline* poly2D,
    const bool& hasToErase )
{
    //verifier si la polyline est fermer
    if( poly2D->isClosed() == Adesk::kFalse )
    {
        poly2D->close();
        return NULL;
    }
    
    //si poly contient un arc retourne null
    if( poly2D->isOnlyLines() == Adesk::kFalse )
    {
        poly2D->close();
        return NULL;
    }
    
    //declarer les sommets
    AcGePoint3d pt1, pt2, pt3, pt4;
    
    poly2D->getPointAt( 0, pt1 );
    poly2D->getPointAt( 1, pt2 );
    poly2D->getPointAt( 2, pt3 );
    poly2D->getPointAt( 3, pt4 );
    
    if( hasToErase )
        poly2D->erase();
        
    AcDbFace* face;
    
    //cas d'une poly2D a 3 sommet
    if( poly2D->numVerts() == 3 )
        face = new AcDbFace( pt1, pt2, pt3 );
        
    //retourner l'objet face3D
    else if( poly2D->numVerts() == 4 )
        face = new AcDbFace( pt1, pt2, pt3, pt4 );
        
    else return NULL;
    
    //on applique les mêmes propriétés que la face à la polyligne 2D
    face->setPropertiesFrom( poly2D );
    poly2D->close();
    
    return face;
}


void poly2DToPoly2DSeg(
    AcDbPolyline*& poly2D,
    const double& tol )
{
    //recuperer la listes de points de discretisation de la poly2D
    AcGePoint2dArray pointArray = discretize( poly2D, tol );
    
    //Mettre le nombre de vertexe du polyline a 0
    poly2D->reset( Adesk::kFalse, 0 );
    
    //boucler sur le point array de la polyline
    int ptArraySize = pointArray.size();
    
    // on rajoute les sommets à la polyligne
    for( int i = 0 ; i < ptArraySize; i++ )
        poly2D->addVertexAt( i, pointArray[i] );
        
}


void coordinateInViewScu(AcGePoint3d& pt)
{
	//Recuperer le view port active
	acedVports2VportTableRecords();
	AcDbViewportTableRecordPointer vp;

	//Recuperer son scu
	AcGeVector3d vpX, vpY;
	AcGePoint3d center;
	vp.open(acedActiveViewportId(), AcDb::kForWrite);

	//Recuperer l'ucs
	vp->getUcs(center, vpX, vpY);

}