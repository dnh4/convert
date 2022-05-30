#pragma once
#include "convertFace.h"
#include "geometry.h"
#include "poly2DEntity.h"

int exportFaceToXml(
    ofstream* file,
    string sLayer )
{

    // on demande la sélection
    ads_name faceSelection;
    //acutPrintf( _T( "\n Sélectionner les faces \n" ) );
    long length = getSelectionSet( faceSelection, "X", "3DFACE", sLayer.c_str() );
    
    // récupère le nombre d'objets séletionnés
    if( ( acedSSLength( faceSelection, &length ) != RTNORM ) || ( length == 0 ) )
    {
        //acutPrintf( _T( " Selection failed \n" ) );
        acedSSFree( faceSelection );
        return 0;
    }
    
    //Completer le nom du calque
    *file << "\n\t<Surface name=\"" + sLayer + "\" desc=\"Description\">";
    *file << "\n\t\t<Definition surfType=\"TIN\">";
    *file << "\n\t\t\t<Pnts>";
    
    // vector des positions de la sélection
    vector< AcGePoint3d > positions;
    positions.resize( 0 );
    
    // vecteur avec les index correspondants aux positions des sommets
    vector< vector< int >> index;
    
    //vecteur avec les index de la face i
    vector< int > indexFaceI;
    index.resize( 0 );
    
    ads_name ent;
    AcDbObjectId id = AcDbObjectId::kNull;
    
    /// 1. On parcourt toutes les faces 3D
    
    vector< AcGePoint3d >::iterator it;
    
    // on boucle sur les faces
    for( int i = 0; i < length; i++ )
    {
    
        if( acedSSName( faceSelection, i, ent ) != RTNORM )
            continue;
            
        if( acdbGetObjectId( id, ent ) != Acad::eOk )
            continue;
            
        AcDbEntity* pEnt = NULL;
        
        if( acdbOpenAcDbEntity( pEnt, id, AcDb::kForWrite ) != Acad::eOk )
            continue;
            
        // on récupère la face
        AcDbFace* castEntity = static_cast< AcDbFace* >( pEnt );
        
        indexFaceI.resize( 0 );
        
        // on boucle sur les sommets
        for( int k = 0; k < 3; k++ )
        {
        
            // on récupère la position du sommet
            AcGePoint3d position;
            castEntity->getVertexAt( k, position );
            
            it = find( positions.begin(), positions.end(), position );
            
            if( it == positions.end() )
            {
                positions.push_back( position );
                indexFaceI.push_back( positions.size() - 1 );
            }
            
            else
                indexFaceI.push_back( it - positions.begin() );
        }
        
        index.push_back( indexFaceI );
        
        // on ferme la face
        castEntity->close();
    }
    
    // Liberer la selection
    acedSSFree( faceSelection );
    
    int sizeP = positions.size();
    
    // on note les positions
    for( int i = 0; i < sizeP; i++ )
    {
    
        ACHAR s1[ 255 ], s2[ 255 ], s3[ 255 ];
        acdbRToS( long double( positions[ i ][ 0 ] ), 2, 6, s1 );
        acdbRToS( long double( positions[ i ][ 1 ] ), 2, 6, s2 );
        acdbRToS( long double( positions[ i ][ 2 ] ), 2, 6, s3 );
        
        *file << "\n\t\t\t\t<P id=\"" + to_string( long long int ( i ) ) +
            "\"> " + acStrToStr( s2 ) +
            " " + acStrToStr( s1 ) +
            " " + acStrToStr( s3 ) + "</P>";
    }
    
    *file << "\n\t\t\t</Pnts>";
    *file << "\n\t\t\t<Faces>";
    
    // on note les index
    int size = index.size();
    
    for( int i = 0; i < size; i++ )
    {
        *file << "\n\t\t\t\t<F n=\"" + to_string( long long int( i + sizeP ) ) + "\">" + to_string( long long int( index[ i ][ 0 ] ) ) + " " +
            to_string( long long int( index[ i ][ 1 ] ) ) + " " +
            to_string( long long int( index[ i ][ 2 ] ) ) + "</F>";
    }
    
    //Terminer le fichier
    *file << "\n\t\t\t</Faces>";
    *file << "\n\t\t</Definition>";
    *file << "\n\t</Surface>";
    
    //Sortir
    return length;
}


vector< AcString > getLayers()
{

    // on récupère la table des calques
    AcDbLayerTable* pTable = NULL;
    Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLayerTable( pTable, AcDb::kForRead );
    
    // on vérifie qu'on ait bien récupéré la table
    if( es != Acad::eOk )
    {
        acutPrintf( _T( "Ne peut pas ouvrir la table des calques" ) );
        vector< AcString > vec( 0, _T( "" ) );
        return vec;
    }
    
    // pRecord est la variable dans laquelle on va stocker le calque
    AcDbObjectId currentId = acdbHostApplicationServices()->workingDatabase()->clayer();
    AcDbLayerTableRecord* pRecord = NULL;
    AcDbObjectId recordId;
    
    // on itère sur les calques de la table
    AcDbLayerTableIterator* pIter;
    
    // on passe le calque 0 car il commence par ABY_
    pTable->newIterator( pIter );
    pIter->step();
    
    // vecteur avec les calques
    vector< AcString > layers( 0 );
    
    for( pIter; !pIter->done(); pIter->step() )
    {
        // on récupère l'Id du calque courant
        es = pIter->getRecordId( recordId );
        
        if( es != Acad::eOk )
        {
            acutPrintf( _T( "Ne peut pas récupérer le calque" ) );
            continue;
        }
        
        // on cast un object AcDbEntity en objet calque grâce à l'Id qu'on a récupéré
        AcDbObject* pObj = NULL;
        es = acdbOpenAcDbObject( pObj, recordId, AcDb::kForWrite );
        pRecord = AcDbLayerTableRecord::cast( pObj );
        
        // si le pointeur sur le calque est nul pn continue la boucle
        if( es != Acad::eOk || pRecord == NULL )
        {
        
            acutPrintf( _T( "Ne peut pas récupérer le calque" ) );
            
            if( pObj != NULL )
                pObj->close();
                
            continue;
        }
        
        // on ajoute le calque au vecteur
        layers.push_back( pRecord->getName() );
    }
    
    //Si le pRecord n'est pas vide
    if( pRecord )
        pRecord->close();
        
    delete pIter;
    pTable->close();
    
    // renvoie la liste des calques
    return layers;
}

int faceToLine( AcDbFace* face,
    const bool& hasToErase,
    AcDbBlockTableRecord* blockTable )
{
    AcGePoint3d pt1, pt2, pt3, pt4;
    int nbLigne = 3;
    
    //recuperer les quatres sommets du face
    face->getVertexAt( 0, pt1 );
    face->getVertexAt( 1, pt2 );
    face->getVertexAt( 2, pt3 );
    face->getVertexAt( 3, pt4 );
    
    //tracer les quatres lignes
    drawLine( pt1, pt2, blockTable, face );
    drawLine( pt2, pt3, blockTable, face );
    
    
    //si la face a trois ou quatre cote
    if( pt3 == pt4 )
        drawLine( pt3, pt1, blockTable, face );
    else
    {
        drawLine( pt3, pt4, blockTable, face );
        drawLine( pt1, pt4, blockTable, face );
        nbLigne = 4;
    }
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        face->erase();
        
    // fermer la face
    face->close();
    
    return nbLigne;
}


AcDbPolyline* faceToPoly2D( AcDbFace* face,
    const bool& hasToErase )
{
    int nbLigne = 3;
    AcGePoint3d pt1, pt2, pt3, pt4;
    
    // Création de la polyligne
    AcDbPolyline* polyline = new AcDbPolyline();
    
    //recuperer les quatres sommets du face
    face->getVertexAt( 0, pt1 );
    face->getVertexAt( 1, pt2 );
    face->getVertexAt( 2, pt3 );
    face->getVertexAt( 3, pt4 );
    
    //recuperer les quatres sommets du face
    face->getVertexAt( 0, pt1 );
    face->getVertexAt( 1, pt2 );
    face->getVertexAt( 2, pt3 );
    face->getVertexAt( 3, pt4 );
    
    //point array
    AcGePoint3dArray pointArray;
    pointArray.append( pt1 );
    pointArray.append( pt2 );
    
    //si la face n'a que trois cote
    if( pt3 == pt4 )
        pointArray.append( pt3 );
        
    //si la face a quatre cote
    else
    {
        pointArray.append( pt3 );
        pointArray.append( pt4 );
        nbLigne = 4;
    }
    
    //boucler sur les pointArray
    for( int i = 0; i < nbLigne; i++ )
        polyline->addVertexAt( i, getPoint2d( pointArray.at( i ) ) );
        
    // Fermer la polyligne
    polyline->setClosed( Adesk::kTrue );
    
    // on applique les mêmes propriétés que la face à la polyligne 2D
    polyline->setPropertiesFrom( face );
    
    //mettre l'elevation a 0
    polyline->setElevation( 0 );
    
    //effacer si l'utilisateur le veut
    if( hasToErase )
        face->erase();
        
    // fermer la face
    face->close();
    
    //retourne l'objet polyline
    return polyline;
}


int faceTo3DPoly2D( AcDbFace* face,
    const bool& hasToErase,
    AcDbBlockTableRecord* blockTable
)
{
    int nbPoly = 1;
    AcGePoint3d pt1, pt2, pt3, pt4;
    
    //declaration de l'elevation
    AcDbPolyline* poly2D1 = new AcDbPolyline();
    
    //declarer un deuxieme polyline
    AcDbPolyline* poly2D2 = new AcDbPolyline();
    
    //recuperer les quatres sommets de la faces
    face->getVertexAt( 0, pt1 );
    face->getVertexAt( 1, pt2 );
    face->getVertexAt( 2, pt3 );
    face->getVertexAt( 3, pt4 );
    
    AcGePoint3dArray pointArray;
    pointArray.append( pt1 );
    pointArray.append( pt2 );
    pointArray.append( pt3 );
    
    //si la face a quatre côtés
    if( pt3 != pt4 )
        pointArray.append( pt4 );
        
    //recuperation des deux plans
    AcGePlane plan1 = AcGePlane( pt1, pt2, pt3 );
    AcGePlane plan2 = AcGePlane( pt1, pt4, pt3 );
    
    //teste si les deux plan soit confondues
    if( plan1.isParallelTo( plan2 ) )
    {
        //recuperer le polyligne 2D dans le repere
        poly2D1 = draw3Dpoly2D( pointArray );
        
        //fermer le figure du polyligne
        poly2D1->setClosed( Adesk::kTrue );
    }
    
    else
    {
        //creer deux nouvelle pointsArray
        AcGePoint3dArray ptArray1, ptArray2;
        ptArray1.append( pt1 );
        ptArray1.append( pt2 );
        ptArray1.append( pt3 );
        ptArray2.append( pt1 );
        ptArray2.append( pt4 );
        ptArray2.append( pt3 );
        
        //recuperer les deux polylignes
        poly2D1 = draw3Dpoly2D( ptArray1 );
        poly2D2 = draw3Dpoly2D( ptArray2 );
        
        poly2D1->setPropertiesFrom( face );
        poly2D2->setPropertiesFrom( face );
    }
    
    //ajouter a la modelSpace
    addToModelSpaceAlreadyOpened( blockTable, poly2D1 );
    addToModelSpaceAlreadyOpened( blockTable, poly2D2 );
    
    //fermer les deux poylines
    poly2D1->close();
    poly2D2->close();
    
    //suprimer si l'utilisateur le veut
    if( hasToErase )
        face->erase();
        
    //fermer la face
    face->close();
    return 0;
}


AcDb3dPolyline* faceToPoly3D( AcDbFace* face,
    const bool& hasToErase )
{
    AcGePoint3d pt1, pt2, pt3, pt4;
    
    // Création de la polyligne
    AcDb3dPolyline* poly3D;
    
    //recuperer les quatres sommets du face
    face->getVertexAt( 0, pt1 );
    face->getVertexAt( 1, pt2 );
    face->getVertexAt( 2, pt3 );
    face->getVertexAt( 3, pt4 );
    
    //point array
    AcGePoint3dArray pointArray;
    pointArray.append( pt1 );
    pointArray.append( pt2 );
    
    //si la face n'a que trois cote
    if( pt3 == pt4 )
        pointArray.append( pt3 );
        
    //si la face a quatre cote
    else
    {
        pointArray.append( pt3 );
        pointArray.append( pt4 );
    }
    
    poly3D = new AcDb3dPolyline( AcDb::k3dSimplePoly, pointArray, Adesk::kTrue );
    
    //fermer la figure du polyline
    poly3D->makeClosed();
    
    // on applique les mêmes propriétés que la face à la polyligne 3D
    poly3D->setPropertiesFrom( face );
    
    //suprimer si l'utilisateur le veut
    if( hasToErase )
        face->erase();
        
    //fermer la face
    face->close();
    
    return poly3D;
}