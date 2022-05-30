/**
  * \file       convertPoly3D.cpp
  * \brief      Commande pour convertir des polylignes 3D en lignes, en polyligne2d  et en Face 3D
  * \author     Marielle H.R
  * \version    0.1
  * \date       09 juillet 2019
 */

#pragma once
#include "convertPoly3D.h"


AcDbPolyline* poly3DToPoly2D( AcDb3dPolyline* poly3D, const bool& hasToErase )
{
    //Déclarer les objets
    AcDbPolyline* poly2D = new AcDbPolyline();
    AcDb3dPolylineVertex* vertex = NULL;
    long ivertex = 0;
    AcGePoint2d pt2D;
    
    //Créer une iterateur sur le sommets de la polyligne
    AcDbObjectIterator* iterPoly3D = poly3D->vertexIterator();
    
    //Initialiser l'iteration
    iterPoly3D->start();
    
    //Iterer sur les sommets
    while( !iterPoly3D->done() )
    {
        //Recuperer le vertex puis le sommet
        if( Acad::eOk == poly3D->openVertex( vertex, iterPoly3D->objectId(), AcDb::kForRead ) )
        {
            //Recuperer le sommet
            pt2D = getPoint2d( vertex->position() );
            
            //Ajouter le sommet a lapolyligne
            poly2D->addVertexAt( ivertex, pt2D );
            
            //Fermer le vertex
            vertex->close();
            ivertex++;
        }
        
        //Iterer
        iterPoly3D->step();
    }
    
    //recuperer les infos de la polyline3d pour ajouter à la polyline 2d
    poly2D->setPropertiesFrom( poly3D, Adesk::kTrue );
    poly2D->setClosed( poly3D->isClosed() );
    
    // On supprime la poly3D si le dessinateur le demande
    if( hasToErase )
        poly3D->erase();
        
    // On ferme la poly3D
    poly3D->close();
    
    //Sortir
    return poly2D;
}


long poly3DToLine( AcDbBlockTableRecord* blockTable, AcDb3dPolyline* poly3D, const bool& hasToErase )
{
    //initialiser les compteurs sur les polylignes
    long nbObject = 0;
    
    //declarer les objets
    AcDb3dPolylineVertex* vertex = NULL;
    
    //créer un itérateur sur les sommets de la polyligne 3D
    AcDbObjectIterator* iterPoly3D = poly3D->vertexIterator();
    
    //recuperer le premier sommet
    iterPoly3D->start();
    
    if( Acad::eOk != poly3D->openVertex( vertex, iterPoly3D->objectId(), AcDb::kForRead ) ) return 0;
    
    //recuperer le premier sommet
    AcGePoint3d pt1 = vertex->position();
    AcGePoint3d pt2 = pt1;
    AcGePoint3d pt3 = pt1;
    
    //Fermer le vertex
    vertex->close();
    iterPoly3D->step();
    
    for( ; !iterPoly3D->done(); iterPoly3D->step() )
    {
        //recuperer le sommet
        if( Acad::eOk == poly3D->openVertex( vertex, iterPoly3D->objectId(), AcDb::kForRead ) )
        {
            //dessiner la ligne
            pt2 = vertex->position();
            drawLine( pt1, pt2, blockTable, poly3D );
            
            //transferer le pt2 vers 1
            pt1 = pt2;
            
            //iterer sur le nbre d'objet convertis
            nbObject++;
        }
    }
    
    //cas de polyligne3D fermé
    if( isClosed( poly3D ) )
    {
        //dessiner une ligne
        drawLine( pt1, pt3 );
        
        //iterer le nombre d'objet
        nbObject++;
    }
    
    // On supprime la poly si le dessinateur le demande
    if( hasToErase )
        poly3D->erase();
        
    // On ferme la poly
    poly3D->close();
    
    //retourner le nombre de lignes crées
    return nbObject;
}


AcDbFace* poly3DToFace3D( AcDb3dPolyline* poly3D )
{
    //recuperer le nombre de sommets
    long numVerts = getNumberOfVertex( poly3D );
    AcDbFace* face3D = NULL;
    
    //verifier si le nombre de sommets est egal a trois ou quatre
    if( ( numVerts == 3 ) || ( numVerts == 4 ) )
    {
        //créer un itérateur sur les sommets de la polyligne 3D
        AcDbObjectIterator* iterPoly3D = poly3D->vertexIterator();
        AcDb3dPolylineVertex* vertex;
        AcGePoint3d pt1, pt2, pt3, pt4;
        int i = 1;
        
        //itérer sur les sommets de la polyligne 3D
        for( iterPoly3D->start(); !iterPoly3D->done(); iterPoly3D->step() )
        {
            if( Acad::eOk == poly3D->openVertex( vertex, iterPoly3D->objectId(), AcDb::kForRead ) )
            {
                if( i == 1 ) pt1 = vertex->position();
                
                if( i == 2 ) pt2 = vertex->position();
                
                if( i == 3 ) pt3 = vertex->position();
                
                if( i == 4 ) pt4 = vertex->position();
                
                i++;
            }
        }
        
        //creer la face 3d
        if( numVerts == 3 ) face3D = new AcDbFace( pt1, pt2, pt3 );
        else face3D = new AcDbFace( pt1, pt2, pt3, pt4 );
        
        face3D->setPropertiesFrom( poly3D );
    }
    
    return face3D;
}


Acad::ErrorStatus convertPoly3DToPoly2DArc( AcDb3dPolyline* poly3D,
    const double& tol )
{
    //Resultat par defaut
    Acad::ErrorStatus er = Acad::eNotApplicable;
    
    //Recuperer les sommets des polylignes 3d
    AcGePoint3dArray polyVertexes;
    vector<double> xVec;
    getVertexesPoly( poly3D,
        polyVertexes,
        xVec );
        
    //Taille du vecteur
    int taille = polyVertexes.size();
    
    //Vecteur qui va contenir les indexs des arcs
    vector<int> indexArc;
    
    //Vecteur qui va contenir les indexs des segments
    vector<int> indexSegment;
    
    //Indexs
    int next = 1;
    int nextNext = 2;
    
    int starting  = -1;
    int ending = -1;
    
    //Boucle pour remplir le vecteurs d'indexs des arcs et de segments
    for( int counter = 0; counter < taille; counter ++ )
    {
        //Le point courant
        AcGePoint3d curPt = polyVertexes[counter];
        
        //Aller sur le point suivant
        next = counter + 1;
        
        //Verifier
        if( next >= taille )
            break;
            
        //Recuperer le second point
        AcGePoint3d nextPt = polyVertexes[next];
        
        //Distance
        double distCur = getDistance2d( curPt, nextPt );
        
        //Aller sur le point suivant
        nextNext = next + 1;
        
        //Verifier
        if( nextNext >= taille )
            break;
            
        //Recuperer le troisieme point
        AcGePoint3d nexNextPt = polyVertexes[nextNext];
        
        //REcuperer la distance
        double distNext = getDistance2d( nextPt, nexNextPt );
        
        //Tester
        while( isTheSame( distCur, distNext, tol ) )
        {
            //Tester
            if( starting == -1 )
                starting = counter;
                
            //Iterer l'index
            next ++;
            
            //Test de sortie
            if( nextNext + 1 >= taille )
            {
                ending = nextNext;
                break;
            }
            
            //Iter l'index
            nextNext++;
            
            //Recuperer le point
            nextPt = polyVertexes[next];
            nexNextPt = polyVertexes[nextNext];
            
            //Recuperer la nouvelle valeur de la distance
            distNext = getDistance2d( nextPt, nexNextPt );
        }
        
        //Verifier
        if( ( starting != -1 ) && ( ending == -1 ) )
            ending = next;
            
        //Verifier
        if( ( starting != -1 ) && ( ending != -1 ) )
        {
            //REcuperer l'index
            counter = ending - 1;
            
            //Recuperer l'index central
            int mid = ( int )( ( starting + ending ) / 2 );
            
            //Ajouter les indexs dans le vecteur
            indexArc.push_back( starting );
            indexArc.push_back( mid );
            indexArc.push_back( ending );
            
            //Reinitialiser les indexs
            starting  = -1;
            ending = -1;
        }
        
    }
    
    //La polyligne 2d
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //Boucle sur les sommets des polylignes
    for( int p = 0; p < taille; p++ )
    {
        //Recuperer le premier point
        AcGePoint2d currentPoint = getPoint2d( polyVertexes[p] );
        
        //Rechercher si c'est un sommet Arc
        std::vector<int>::iterator itArc = std::find( indexArc.begin(), indexArc.end(), p );
        
        //Tester l'index
        if( itArc != indexArc.end() )
        {
            //Iterateur
            vector<int>::iterator itPt1, itPt2, itPt3;
            
            itPt1 = itArc;
            itArc++;
            itPt2 = itArc;
            itArc++;
            itPt3 = itArc;
            
            //Recuperer l'arc à trois point
            AcDbArc* arc = arcByThreePoints( polyVertexes[*itPt1],
                    polyVertexes[*itPt2],
                    polyVertexes[*itPt3] );
                    
            //Recuperer le bulge
            double bulge = getBulgeAt( arc );
            
            //Recuperer le sens de l'arc si c'est antihoraire ou horaire
            double clock = ( polyVertexes[*itPt2].x - polyVertexes[*itPt1].x ) * ( polyVertexes[*itPt3].y - polyVertexes[*itPt1].y ) - ( polyVertexes[*itPt2].y - polyVertexes[*itPt1].y ) * ( polyVertexes[*itPt3].x - polyVertexes[*itPt1].x );
            
            //Recuperer le bulge correspondant au sens de l'arc
            if( clock > 0 )
                bulge = bulge;
            else if( clock < 0 )
                bulge = -bulge;
            else
                bulge = 0.0;
                
            //Ajouter le premier point
            poly2D->addVertexAt( poly2D->numVerts(), currentPoint, bulge );
            
            if( indexArc.size() > 3 )
                p = *itPt3 - 1 ;
                
            else
            {
                p = *itPt3;
                
                //Ajouter le premier point
                poly2D->addVertexAt( poly2D->numVerts(), getPoint2d( polyVertexes[p] ) );
            }
            
            indexArc.erase( indexArc.begin(), indexArc.begin() + 3 );
            
            continue;
        }
        
        //Segment
        else
            poly2D->addVertexAt( poly2D->numVerts(), currentPoint );
    }
    
    //Fermer la polyligne si la polyligne3D l'est
    poly2D->setClosed( poly3D->isClosed() );
    
    //Recuperer les propriétés de la polyligne 3D
    poly2D->setPropertiesFrom( poly3D );
    
    //Ajouter la polyligne 2D dans le modelspace
    addToModelSpace( poly2D );
    
    //Fermer la polyligne
    poly2D->close();
    
    //Retourner eOk
    return Acad::eOk;
}