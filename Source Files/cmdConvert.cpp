#pragma once
#include "Dalles.h"
#include "cmdConvert.h"
#include "blockEntity.h"
#include "layer.h"
#include <time.h>
#include "helperFunctions.h"
#include "acString.h"
#include "hatchEntity.h"

using namespace std;
double g_toleranceFleche = 0.1;
double g_toleranceBetweenTwoPoint = 0.01;
double g_tolerance = 0.1;
bool g_hasToErase = true;
double g_blockDistTol = BLOCK_DISTANCE_REF;
#define MIN_DISTANCE_FOR_INSERTION 0.001f;
#define BLOC_NAME_MEPAUTO "MEPAUTO"
double g_toleranceIntersect = 0.01;

#define TOPO_POINTS_HORS_TN_PIQUES_LAYER _T("TOPO_Points_Hors_TN_Piqués")
#define TOPO_POINTS_PIQUES_LAYER _T("TOPO_Points_Piqués")
#define TOPO_POINTS_SURPLUS_PIQUES_LAYER _T("TOPO_Points_Surplus_Piqués")
#define TOPO_POINTS_PIQUES_BLOCK_NAME _T("PTOPO")
#define TOPO_POINTS_PIQUES_BLOCK_LAYER _T("TOPO_Points_Hors_TN_Piqués,TOPO_Points_Piqués,TOPO_Points_Surplus_Piqués")

#define TOPO_FMAP_ELEV_NAME _T("FMAP_Elevation")
#define BLOCK_LAYER _T("TOPO_Bâtiments_Annexes")
#define TOPO_POINTS_SEUILS_PIQUES_LAYER _T("TOPO_Points_Seuil_Piqués")
#define TOPO_POINTS_SEUILS_PIQUES_ALTITUDE_LAYER _T("TOPO_Points_Seuil_Altitudes")
#define TOPO_POINTS_SEUILS_PIQUES_NUMERO_LAYER _T("TOPO_Points_Seuil_Matricules")
#define BLOCK_NAMES _T("SOUPIRAIL,ENTREE")

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616


void test()
{
}


void cmdXxxToPolyline2D()
{
    //Selection dans le dessin
    ads_name ss;
    
    //Declaration des compteurs de conversion et declaration de la tolerance
    int nbLine = 0, nbLineC = 0, nbPoly = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les elements a convertir
    acutPrintf( _T( "Sélectionner les éléments dans le dessin: " ) );
    
    //Nombre d'element dans le dessin et declaration des entites
    long length = getSelectionSet( ss, ( const GCHAR* )NULL, "" );
    AcDbEntity* ent = NULL;
    AcDbPolyline* poly2D = NULL;
    AcDbArc* arc = NULL;
    AcDbCircle* circle = NULL;
    
    // Demander au dessinateur s'il veut supprimer les elements
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les éléments ? :" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de discrétisation:" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Ouverture de la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Boucle sur les elements dans le dessin
    for( int i = 0; i < length; i++ )
    {
        //Recuperer l'i-eme element dans le dessin
        ent = getEntityFromSs( ss, i, AcDb::kForWrite );
        
        //Tester si l'entité est un Arc, si oui on discretise l'Arc
        if( ent->isKindOf( AcDbArc::desc() ) )
            nbLine = arcToLine( blockTable, AcDbArc::cast( ent ), g_hasToErase, g_toleranceFleche );
            
        else if( ent->isKindOf( AcDbPolyline::desc() ) )
        {
            poly2D = AcDbPolyline::cast( ent );
            
            // Tester si l'entité est une polyligne, si oui on discretise la Polyligne
            AcGePoint2dArray ptArray = discretize( poly2D, g_toleranceFleche );
            
            // Boucle sur les sommets discretises de la polyligne pour tracer les lignes
            int size = ptArray.length() - 1;
            
            for( int j = 0; j < size; j++ )
                drawLine( ptArray[ j ], ptArray[ j + 1 ], blockTable, poly2D );
                
            // On incremente le nombre de polyligne discretise
            nbPoly++;
            
            //On supprime la polyligne si le dessinateur le demande
            if( g_hasToErase )
                poly2D->erase( true );
                
            // On ferme la polyligne2D
            poly2D->close();
        }
        
        //Tester si l'entité est un Cercle, si oui on discretise le Cercle
        else if( ent->isKindOf( AcDbCircle::desc() ) )
            nbLineC = circleToLine( AcDbCircle::cast( ent ), blockTable, g_hasToErase, g_toleranceFleche );
            
        // incrementer la barre de progression
        prog.moveUp( i );
    }
    
    //Fermeture de la base de donnee, liberation de la selection et affichage des resultats
    closeModelSpace( blockTable );
    acedSSFree( ss );
    acutPrintf( _T( "Nombre d'éléments convertis : %d Polylignes, %d Arcs, %d Cercles " ), nbPoly, nbLine, nbLineC );
    
}


void cmdCreateCube()
{
    ////Demander de Sélectionner le fichier de coordonnees a ouvrir
    //AcString path = askForFilePath( true,
    //        _T( "pts" ),
    //        _T( "Sélectionner le fichier de coordonnées" ),
    //        getCurrentFileFolder() );
    //
    //// On cree un objet FileReader
    //FileReader* fr = new FileReader( acStrToStr( path ), " " );
    //
    //int length = fr->getLineNumber();
    //
    ////Creer et ouvrir la base de donnees
    //AcDbBlockTableRecord* blockTable = openModelSpace();
    //
    //// Initialistion de la barre de progression
    //ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    //
    ////Iterateur utiliser pour iterer la barre de progression
    //int i = 0;
    //
    //// On parcourt les lignes du fichier
    //while( fr->readNextLine() )
    //{
    //    double xx, yy, zz, ii;
    //    fr->getValue( "X", xx );
    //    fr->getValue( "Y", yy );
    //    fr->getValue( "Z", zz );
    //    fr->getValue( "I", ii );
    //    AcGePoint3d cubeCenter( xx, yy, zz );
    //    createCube( cubeCenter, 1, ii, blockTable );
    //
    //    //Iterer la progression
    //    i++;
    //    prog.moveUp( i );
    //}
    //
    //// On supprime l'objet FileReader
    //delete fr;
    //
    ////Fermeture de la base de donnee
    //closeModelSpace( blockTable );
}


void cmdNetPoly3D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et declaration de la tolerance
    int nbrVertSuppr = 0;
    double tol;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes 3D à nettoyer" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly3D( ss );
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Polyligne
    AcDb3dPolyline* poly3D = NULL;
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction net poly pour nettoyer et retourner le nombre de vertex supprimes
        nbrVertSuppr = netPoly( poly3D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
        
        //Fermeture de la polyligne
        poly3D->close();
        
        //Iteration du nombre de polyligne supprime
        nbrVertSuppr++;
    }
    
    //Message affichant le nombre de point superposes enleves dans le dessin
    acutPrintf( _T( "Nombre de Vertex enlevés: %d" ), nbrVertSuppr );
    
    //Liberation de la selection et Fermeture de la base de donnee
    acedSSFree( ss );
    closeModelSpace( blockTable );
}


void cmdColiPoly3D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et declaration de la tolerance
    int nbrVertFus = 0;
    double tol;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes 3D à fusionner" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly3D( ss );
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Polyligne
    AcDb3dPolyline* poly3D = new AcDb3dPolyline();
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction coliPoly pour fusionner les polylignes colineaires dans le dessin
        nbrVertFus = coliPoly( poly3D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
        
        //Fermeture de la polyligne
        poly3D->close();
        
        //Iterer le nombre de sommets fusionnes dans le dessin
        nbrVertFus++;
    }
    
    //Message affichant le nombre de point superpose enleve dans le dessin
    acutPrintf( _T( "Nombre de Sommets fusionnés: %d" ), nbrVertFus );
    
    //Liberation de la selection et Fermeture de la base de donnee
    acedSSFree( ss );
    closeModelSpace( blockTable );
}


void cmdNetColiPoly3D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et declaration de la tolerance
    int nbrVertSupprFus = 0;
    double tol;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes 3D à nettoyer et/ou à fusionner" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly3D( ss );
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Polyligne
    AcDb3dPolyline* poly3D = NULL;
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction net poly pour nettoyer et retourner le nombre de vertex supprimes
        nbrVertSupprFus = netColiPoly( poly3D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
        
        //Fermeture de la polyligne
        poly3D->close();
    }
    
    //Message affichant le nombre de point superposes enleves dans le dessin
    acutPrintf( _T( "Nombre de Sommets enlevés: %d" ), nbrVertSupprFus );
    
    //Liberation de la selection et Fermeture de la base de donnee
    acedSSFree( ss );
    closeModelSpace( blockTable );
}


void cmdNetPoly2D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et  declaration de la tolerance
    int nbrVertSuppr = 0;
    double tol;
    
    //Mettre la tolerance à 2cm
    g_toleranceFleche = 0.02;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes à nettoyer" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly2D( ss );
    
    //Polyligne
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction net poly pour nettoyer et retourner le nombre de vertex supprimes
        // Après l'opération la polyligne est fermé
        nbrVertSuppr = netPoly( poly2D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
        
    }
    
    //Message affichant le nombre de point superpose enleve dans le dessin
    acutPrintf( _T( "\nNombre de vertex supprimés: %d" ), nbrVertSuppr );
    
    //Liberation de la selection et Fermeture de la base de donnee
    acedSSFree( ss );
}


void cmdColiPoly2D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et declaration de la tolerance
    int nbrVertColi = 0;
    double tol;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes à nettoyer" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly2D( ss );
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        AcDbPolyline* poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction coliPoly pour fusionner les points colineaires
        // après l'opération la polyligne est fermé
        nbrVertColi = coliPoly( poly2D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
    }
    
    //Message affichant le nombre de point superpose enleve dans le dessin
    acutPrintf( _T( "Nombre de Sommets Colinéaires supprimés: %d" ), nbrVertColi );
    
    //Liberation de la selection et Fermeture de la base de donnee
    acedSSFree( ss );
}


void cmdNetColiPoly2D()
{
    //Declaration des variable qui va contenir le nombre de vertex supprime et declaration de la tolerance
    int nbrVertColiPoly = 0;
    double tol;
    
    //Message de selection
    acutPrintf( _T( "Séléctionner les polylignes à netoyer et/ou à combiner" ) );
    
    //Selection dans le dessin
    ads_name ss;
    
    //Recuperer le nombre de polyligne dans le dessin
    long length = getSsPoly2D( ss );
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Polyligne
    AcDbPolyline* poly2D = new AcDbPolyline();
    
    //Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ), length );
    
    //Demander la valeur de la tolerance
    if( askForDouble( _T( "Valeur de tolérance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        closeModelSpace( blockTable );
        return;
    }
    
    //Boucle sur la polyligne
    for( int i = 0; i < length; i++ )
    {
        //i-eme polyligne dans la selection
        poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        //Appeler la fonction netColiPoly pour fusionner les sommets colineaires et nettoyer les sommets superposes
        nbrVertColiPoly = netColiPoly( poly2D, g_toleranceFleche );
        
        //Iterer la progression
        prog.moveUp( i );
        
        //Fermeture de la polyligne
        poly2D->close();
    }
    
    //Message affichant le nombre de point superpose enleve dans le dessin
    acutPrintf( _T( "Nombre de Sommets Superposés Nettoyés et/ou Colinéaires Fusionnés: %d" ), nbrVertColiPoly );
    
    //Liberation de la selection de la calque et Fermeture de la base de donnee
    acedSSFree( ss );
    closeModelSpace( blockTable );
}


void cmdNetPoly()
{
    //Selection dans le dessin
    ads_name ss;
    
    //Declaration du nombre de sommets superposés dans la polyligne
    int nbrSomFus;
    
    //Declaration de la tolerance
    double tol;
    
    //Declaration de l'entité
    AcDbEntity* ent = NULL;
    
    // Demander au dessinateur de Sélectionner les elements a convertir
    acutPrintf( _T( "Sélectionner les éléments dans le dessin: " ) );
    
    //Nombre d'element dans le dessin et declaration des entites
    long length = getSelectionSet( ss, ( const GCHAR* )NULL, "" );
    
    // Demander la tolerance
    if( askForDouble( _T( "Valeur de la tolérance:" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Ouverture de la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Boucle sur les elements dans le dessin
    for( int i = 0; i < length; i++ )
    {
        //Recuperer l'i-eme element dans le dessin
        ent = getEntityFromSs( ss, i, AcDb::kForWrite );
        
        //Recupere le type de polyligne et supprime les sommets superposés
        if( ent->isKindOf( AcDbPolyline::desc() ) )
        {
            AcDbPolyline* poly = AcDbPolyline::cast( ent );
            nbrSomFus = netPoly( poly, g_toleranceFleche );
            poly->close();
        }
        
        else
        {
            if( ent->isKindOf( AcDb3dPolyline::desc() ) )
            {
                AcDb3dPolyline* poly = AcDb3dPolyline::cast( ent );
                nbrSomFus = netPoly( poly, g_toleranceFleche );
                poly->close();
            }
        }
        
        //Iterer la progression
        prog.moveUp( i );
    }
    
    //Fermeture de la base de donnee, liberation de la selection et affichage des resultats
    closeModelSpace( blockTable );
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de sommets superposés supprimés : %d" ), nbrSomFus );
}


void cmdColiPoly()
{
    //Selection dans le dessin
    ads_name ss;
    
    //Declaration du nombre de sommet fusionnés dans la polyligne
    int nbrSomFus;
    
    //Declaration de la tolerance
    double tol;
    
    //Declaration de l'entité
    AcDbEntity* ent = NULL;
    
    // Demander au dessinateur de Sélectionner les elements a convertir
    acutPrintf( _T( "Sélectionner les éléments dans le dessin: " ) );
    
    //Nombre d'element dans le dessin et declaration des entites
    long length = getSelectionSet( ss, ( const GCHAR* )NULL, "" );
    
    // Demander la tolerance
    if( askForDouble( _T( "Valeur de la tolérance:" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Ouverture de la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Boucle sur les elements dans le dessin
    for( int i = 0; i < length; i++ )
    {
        //Recuperer l'i-eme element dans le dessin
        ent = getEntityFromSs( ss, i, AcDb::kForWrite );
        
        //Recupere le type de polyligne (2D ou 3D) et fusionne les sommets colinéaires
        if( ent->isKindOf( AcDbPolyline::desc() ) )
        {
            AcDbPolyline* poly = AcDbPolyline::cast( ent );
            nbrSomFus = coliPoly( poly, g_toleranceFleche );
            poly->close();
        }
        
        else
        {
            if( ent->isKindOf( AcDb3dPolyline::desc() ) )
            {
                AcDb3dPolyline* poly = AcDb3dPolyline::cast( ent );
                nbrSomFus = coliPoly( poly, g_toleranceFleche );
                poly->close();
            }
        }
        
        //Iterer la progression
        prog.moveUp( i );
    }
    
    //Fermeture de la base de donnee, liberation de la selection et affichage des resultats
    closeModelSpace( blockTable );
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de sommets colinéaires fusionnés : %d" ), nbrSomFus );
}


void cmdNetColiPoly()
{
    //Selection dans le dessin
    ads_name ss;
    
    //Declaration du nombre de sommets superposés dans la polyligne
    int nbrSomFus;
    
    //Declaration de la tolerance
    double tol;
    
    //Declaration de l'entité
    AcDbEntity* ent = NULL;
    
    // Demander au dessinateur de Sélectionner les elements a convertir
    acutPrintf( _T( "Sélectionner les éléments dans le dessin: " ) );
    
    //Nombre d'element dans le dessin et declaration des entites
    long length = getSelectionSet( ss, ( const GCHAR* )NULL, "" );
    
    // Demander la tolerance
    if( askForDouble( _T( "Valeur de la tolérance:" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Ouverture de la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Boucle sur les elements dans le dessin
    for( int i = 0; i < length; i++ )
    {
        //Recuperer l'i-eme element dans le dessin
        ent = getEntityFromSs( ss, i, AcDb::kForWrite );
        
        //Recupere le type de polyligne et supprime les sommets superposés
        if( ent->isKindOf( AcDbPolyline::desc() ) )
        {
            AcDbPolyline* poly = AcDbPolyline::cast( ent );
            nbrSomFus = netColiPoly( poly, g_toleranceFleche );
            poly->close();
        }
        
        else
        {
            if( ent->isKindOf( AcDb3dPolyline::desc() ) )
            {
                AcDb3dPolyline* poly = AcDb3dPolyline::cast( ent );
                nbrSomFus = netColiPoly( poly, g_toleranceFleche );
                poly->close();
            }
        }
        
        //Iterer la progression
        prog.moveUp( i );
    }
    
    //Fermeture de la base de donnee, liberation de la selection et affichage des resultats
    closeModelSpace( blockTable );
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de sommets enlevés : %d" ), nbrSomFus );
}


void cmdLineToPoly2D()
{
    acutPrintf( _T( "Sélectionner les lignes \n" ) );
    
    ads_name ss;
    
    //Sélectionner les lignes
    long length = getSsLine( ss, "" );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les lignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //initialiser le nombre de ligne de selection modifie
    int iPolyModifier = 0;
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    for( int i = 0; i < length; i++ )
    {
    
        //Recuperer la ligne i de la selection
        if( AcDbLine* line = getLineFromSs( ss, i, AcDb::kForWrite ) )
        {
        
            //transformer la ligne en poly2D
            AcDbPolyline* poly2D = convertLineToPoly2D( line, g_hasToErase );
            
            //Ajouter la polyline au ModelSpace
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polyline 2d
            poly2D->close();
            
            //incremente le nombre de line modifier
            iPolyModifier++;
        }
        
        // Incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //afficher le nombre de lignes de la selection et le nombre de lignes modifiees
    acutPrintf( _T( "Nombre de lignes de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre de lignes modifiees : %d \n" ), iPolyModifier );
    
    //fermer mon modelSpace
    closeModelSpace( blockTable );
}


void cmdLineTo3DPoly2D()
{
    acutPrintf( _T( "Sélectionner les lignes \n" ) );
    
    ads_name ss;
    
    //Sélectionner les lignes
    long length = getSsLine( ss, "" );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les lignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //initialiser le nombre digne de selection modifie
    int iPolyModifier = 0;
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    for( int i = 0; i < length; i++ )
    {
    
        //Recuperer la ligne i de la selection
        if( AcDbLine* line = getLineFromSs( ss, i, AcDb::kForWrite ) )
        {
        
            //transformer la ligne en poly2D
            AcDbPolyline* poly2D = convertLineTo3DPoly2D( line, g_hasToErase );
            
            //Ajouter la polyline au ModelSpace
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polyline 2d
            poly2D->close();
            
            //incremente le nombre de line modifier
            iPolyModifier++;
        }
        
        // Incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //afficher le nombr de ligne de la selection et le nombre de ligne modifier
    acutPrintf( _T( "Nombre de lignes de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre de lignes modifiees : %d \n" ), iPolyModifier );
    
    //fermer mon modelSpace
    closeModelSpace( blockTable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdLineToPoly3D()
{
    ads_name ss;
    
    // Demander au dessinateur de Sélectionner les lignes
    acutPrintf( _T( "Sélectionner les lignes à convertir" ) );
    
    // Nombre de ligne converties
    long nbLine = 0 ;
    
    //Sélectionner les lignes
    long length = getSsLine( ss, "" );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les lignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouvrir une modelspace
    AcDbBlockTableRecord* blocktable = openModelSpace();
    
    //boucler sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuper l'objet ligne
        if( AcDbLine* line = getLineFromSs( ss, i, AcDb::kForWrite ) )
        {
            //modifier la ligne en poly3d
            AcDb3dPolyline* poly3D = convertLineToPoly3D( line, g_hasToErase );
            
            //ajouter le poly3d a la modelspace
            addToModelSpaceAlreadyOpened( blocktable, poly3D );
            
            //fermer la polyligne et la ligne
            poly3D->close();
            
            nbLine++;
        }
        
        // Incrementer la bare de progression
        prog.moveUp( i );
    }
    
    acutPrintf( _T( "Nombre de lignes de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre de lignes modifiees : %d \n" ), nbLine );
    
    // Fermer mon modelSpace
    closeModelSpace( blocktable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdPoly2DToLine()
{
    ads_name ss;
    int nbLine = 0;
    double tol = 0;
    AcDbLine* line2d = NULL;
    
    // Demander au dessinateur de Sélectionner les polylignes 2D à convertir
    acutPrintf( _T( "Sélectionner les polylignes 2D à convertir:\n" ) );
    long length = getSsPoly2D( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // True si la poly2D contient un arc, false sinon
    bool hasarc = false;
    
    hasarc = hasArc( ss, length );
    
    // Demander le pas de discretisation
    if( hasarc )
    {
        if( ( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN ) )
        {
            acedSSFree( ss );
            return;
        }
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    for( int i = 0; i < length; i ++ )
    {
        //Recuperer la polyligne de la selection
        AcDbPolyline* poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        // Si poly2D n'est pas nul
        if( poly2D )
        {
            // Discretisation de la polyligne
            AcGePoint2dArray ptArray = discretize( poly2D, tol );
            
            // Boucle sur les sommets discretises de la polyligne pour tracer les lignes
            int size = ptArray.length() - 1;
            
            for( int j = 0; j < size; j++ )
                drawLine( ptArray[ j ], ptArray[ j + 1 ], blockTable, poly2D );
                
            // On incremente la progressBar et le compteur de polylignes modifiees
            nbLine++;
        }
        
        // On supprime la polyligne2D si c'est demande par le dessinateur
        if( g_hasToErase )
            poly2D->erase();
            
        // On ferme la polyligne2D
        poly2D->close();
        
        // Incrementer la barre de progression
        prog.moveUp( i );
    }
    
    acutPrintf( _T( "Nombre de polylignes 2D de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre polylignes 2D modifiees : %d \n" ), nbLine );
    
    // Fermer mon modelSpace
    closeModelSpace( blockTable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdPoly2DToPoly3D()
{
    ads_name ss;
    int nbConverts = 0, nbLine = 0;
    double tol = 0;
    AcDb3dPolyline* poly3D = NULL;
    
    //Demander au dessinateur de Sélectionner les polylignes 2Ds à convertir
    acutPrintf( _T( "Sélectionner les polylignes 2D à convertir:\n" ) );
    long length = getSsPoly2D( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // True si la poly2D contient un arc, false sinon
    bool hasarc = false;
    
    hasarc = hasArc( ss, length );
    
    // Demander le pas de discretisation
    if( hasarc )
    {
        if( ( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN ) )
        {
            acedSSFree( ss );
            return;
        }
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Boucler sur la selection
    for( int i = 0 ; i < length; i++ )
    {
        //recuperer la polyligne de la selection
        AcDbPolyline* poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        //si poly2D n'est pas vide
        if( poly2D )
        {
            //modifier la poly2d en poly3d
            poly3D = poly2DtoPoly3D( poly2D, g_hasToErase, g_toleranceFleche );
            
            //ajouter la polyligne dans le modelspace deja ouvert
            addToModelSpaceAlreadyOpened( blockTable, poly3D );
            
            //fermer la polyligne
            poly3D->close();
        }
        
        // Incrementer la barre de progression
        prog.moveUp( i );
        
        nbLine++;
    }
    
    acutPrintf( _T( "Nombre de polylignes 2D de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre polylignes 2D modifiees : %d \n" ), nbLine );
    
    // Fermer mon modelSpace
    closeModelSpace( blockTable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdPoly2DToFace3D()
{
    //Recuperer la selection polyline2d
    ads_name ss;
    int nbConverts = 0;
    AcDbFace* face3D = NULL;
    
    //demander au dessinateur de Sélectionner les polylignes 3Ds à convertir
    acutPrintf( _T( "Sélectionner les polylignes 2D à convertir:\n" ) );
    
    long length = getSsPoly2D( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Boucler sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuperer la polyligne de la selection
        AcDbPolyline* poly2D = getPoly2DFromSs( ss, i, AcDb::kForWrite );
        
        //si poly2D n'est pas vide
        if( poly2D )
        {
            face3D = poly2DToFace( poly2D, g_hasToErase );
            
            //fermer la face
            if( face3D )
            {
            
                //ajouter la face a la modelspace
                addToModelSpaceAlreadyOpened( blockTable, face3D );
                face3D->close();
                nbConverts++;
            }
        }
        
        prog.moveUp( i );
    }
    
    acutPrintf( _T( "Nombre de polylignes 2D de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre polylignes 2D modifiees : %d \n" ), nbConverts );
    
    // Fermer le modelSpace
    closeModelSpace( blockTable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdPoly2DToPoly2DSeg()
{
    ads_name ss;
    int nbConverts = 0;
    double tol = 0;
    
    //demander au dessinateur de Sélectionner les polylignes 2D à convertir
    acutPrintf( _T( "Sélectionner les polylignes 2D à convertir:\n" ) );
    
    //recuperer la selection polylignes 2D
    long length = getSsPoly2D( ss );
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //Boucler sur la selection
    for( int i = 0; i < length; i++ )
    {
        // Recuperer la polyligne de la selection
        AcDbPolyline* poly2D = writePoly2DFromSs( ss, i );
        
        // Si la polyligne n'est pas vide et qu'elle contient des arcs
        if( poly2D && !poly2D->isOnlyLines() )
        {
            // Convertir la polyligne 2D avec arcs en polyligne 2D sans arc
            poly2DToPoly2DSeg( poly2D, tol );
            
            // Ajouter la polyligne au modelspace
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            nbConverts++;
        }
        
        // Fermer la polyligne
        poly2D->close();
        
        // Incrementer la barre de progression
        prog.moveUp( i );
    }
    
    acutPrintf( _T( "Nombre de polylignes 2D de la selection : %d \n" ), length );
    acutPrintf( _T( "Nombre polylignes 2D modifiees : %d \n" ), nbConverts );
    
    // Fermer le modelSpace
    closeModelSpace( blockTable );
    
    // Liberer la selection
    acedSSFree( ss );
}


void cmdPoly3DToLine()
{
    ads_name ss;
    int nbConverts = 0;
    int nbLine = 0;
    
    //demander au dessinateur de Sélectionner les polylignes 3D à convertir
    acutPrintf( _T( "Sélectionner les polylignes 3D à convertir:\n" ) );
    
    long length = getSsPoly3D( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la poly3D
        AcDb3dPolyline* poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur la  polyline3d est null
        if( poly3D )
        {
            //boucle sur les sommets de la poly3d pour ajouter à la polyligne 2d
            nbLine += poly3DToLine( blockTable, poly3D, g_hasToErase );
            
            // Incremente le nombre de poly3D converties
            nbConverts++;
            
            // Incremente la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    // Message final
    acutPrintf( _T( "Nombre de polylignes 3D converties : %d \n Nombre de lignes obtenues: %d\n" ), nbConverts, nbLine );
}


void cmdPoly3DToPoly2D()
{
    //declaration des variables
    ads_name ss;
    int nbConverts = 0;
    
    //demander au dessinateur de Sélectionner les polylignes 3D à convertir
    acutPrintf( _T( "Sélectionner les polylignes 3D à convertir:\n" ) );
    
    //recuperer le nombre de selections
    long length = getSsPoly3D( ss );
    
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable =  openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la poly3D
        AcDb3dPolyline* poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur la  polyline3d est null
        if( poly3D )
        {
            //boucle sur les sommets de la poly3D pour ajouter à la polyligne 2D
            AcDbPolyline* poly2D = poly3DToPoly2D( poly3D, g_hasToErase );
            
            //ajout de la polyligne 2d dans la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polylige 2d et l'ajouter dans la base de donnees
            poly2D->close();
            
            nbConverts++;
        }
    }
    
    // closeModelSpace
    closeModelSpace( blockTable );
    
    //liberer la selection et afficher le resultat
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de polyligne 3D converties : %d \n" ), nbConverts );
}


void cmdPoly3DToFace3D()
{
    ads_name ss;
    // Nombre de polylignes converties
    int nbConverts = 0;
    
    //demander au dessinateur de Sélectionner les polylignes 3D à convertir
    acutPrintf( _T( "Sélectionner les polylignes 3D à convertir:\n" ) );
    
    long length = getSsPoly3D( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les polylignes" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la poly3D
        AcDb3dPolyline* poly3D = getPoly3DFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur la  polyline3d est null
        if( poly3D )
        {
        
            //boucle sur les sommets de la poly3d pour ajouter à la face
            AcDbFace* face3D = poly3DToFace3D( poly3D );
            
            if( face3D )
            {
                //ajout à la base de donnees
                addToModelSpaceAlreadyOpened( blockTable, face3D );
                
                //fermer la face3D
                face3D->close();
                
                //supprimer la polyligne 3D
                poly3D->erase();
                
                //iterer le nombre de polylignes converties
                nbConverts++;
            }
            
            //fermer la polyligne 3d
            poly3D->close();
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de polylignes 3D converties : %d \n" ), nbConverts );
}


void cmdArcToLine()
{
    ads_name ss;
    
    // Nombre d'arcs convertis
    int nbConverts = 0;
    int nbLine = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les arcs 3D à convertir
    acutPrintf( _T( "\nSélectionner les arcs à convertir:\n" ) );
    long length = getSsArc( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les arcs" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Boucle sur les arcs
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbArc* arc = getArcFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( arc )
        {
            // Incremente le nombre de lignes crees
            nbLine += arcToLine( blockTable, arc, g_hasToErase, tol );
            
            // iterer le nombre de polylignes converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //Fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre d'arcs convertis : %d \n Nombre de lignes creees: %d\n" ), nbConverts, nbLine );
}


void cmdArcTo2DPoly2D()
{
    ads_name ss;
    
    // Nombre d' arc converties
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les arcs 3D à convertir
    acutPrintf( _T( "\nSélectionner les arcs à convertir:\n" ) );
    long length = getSsArc( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les arcs." ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la poly3D
        AcDbArc* arc = getArcFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( arc )
        {
            //conversion de l'arc
            AcDbPolyline* poly2D = arcTo2DPoly2D( arc, g_hasToErase, tol );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polyligne 3D
            poly2D->close();
            
            //iterer le nombre d'arc convertis
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre d'arcs convertis : %d \n" ), nbConverts );
}


void cmdArcToPoly2DSeg()
{
    ads_name ss;
    
    // Nombre d'arcs convertis
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les arcs 3D à convertir
    acutPrintf( _T( "\nSélectionner les arcs à convertir:\n" ) );
    long length = getSsArc( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les arcs" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbArc* arc = getArcFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( arc )
        {
            // conversion de l'arc
            AcDbPolyline* poly2D = arcToPoly2DSeg( arc, g_hasToErase, tol );
            
            // ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            // fermer la polyligne 3D
            poly2D->close();
            
            // incrementer le nombre d'arc converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre d'arcs convertis : %d \n" ), nbConverts );
}


void cmdArcTo3DPoly2D()
{
    ads_name ss;
    //Nombre d' arc convertis
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les arcs 3D à convertir
    acutPrintf( _T( "\nSélectionner les arcs à convertir:\n" ) );
    long length = getSsArc( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les arcs" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //on boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // on recupere la poly3D
        AcDbArc* arc = getArcFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( arc )
        {
            //conversion de l'arc
            AcDbPolyline* poly2D = arcTo3DPoly2D( arc, g_hasToErase, tol );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polyligne 3D
            poly2D->close();
            
            //iterer le nombre d'arc converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre d'arcs convertis : %d \n" ), nbConverts );
}


void cmdArcToPoly3D()
{
    ads_name ss;
    // Nombre d' arc converties
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les arcs 3D à convertir
    acutPrintf( _T( "\nSélectionner les arcs à convertir:\n" ) );
    long length = getSsArc( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les arcs" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la poly3D
        AcDbArc* arc = getArcFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( arc )
        {
            //conversion de l'arc
            AcDb3dPolyline* poly3D = arcToPoly3D( arc, g_hasToErase, tol );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly3D );
            
            //fermer la polyligne 3D
            poly3D->close();
            
            //iterer le nombre d'arc converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre d'arcs convertis : %d \n" ), nbConverts );
}


void cmdCircleToLine()
{
    ads_name ss;
    int nbConverts = 0, nbLine = 0;
    double tol = 0;
    
    //demander au dessinateur de Sélectionner les cercles à convertir
    acutPrintf( _T( "\n Sélectionner les cercles à convertir:\n" ) );
    long length = getSsCircle( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) cercle(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbCircle* circle = getCircleFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( circle )
        {
            nbLine += circleToLine( circle, blockTable, g_hasToErase, g_tolerance );
            
            //iterer le nombre de polylignes converties
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de cercles convertis : %d \n Nombre de lignes obtenues: %d\n" ), nbConverts, nbLine );
}


void cmdCircleToPoly2D()
{
    ads_name ss;
    int nbConverts = 0;
    double n = 4.0;
    
    //demander au dessinateur de Sélectionner les cercles à convertir
    acutPrintf( _T( "\n Sélectionner les cercles à convertir:\n" ) );
    long length = getSsCircle( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) cercle(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Nombre d'arcs (superieur à 2 ) :" ), n, n ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere le cercle
        AcDbCircle* circle = getCircleFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur le cercle est null
        if( circle )
        {
            //conversion du cercle
            AcDbPolyline* polyline = circleToPoly2D( circle, g_hasToErase, n );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, polyline );
            
            //fermer la polyligne 3D
            polyline->close();
            
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de cercles convertis : %d \n" ), nbConverts );
}


void cmdCircleToPoly3D()
{
    ads_name ss;
    int nbConverts = 0;
    double tol = 0;
    
    //demander au dessinateur de Sélectionner les cercles à convertir
    acutPrintf( _T( "\n Sélectionner les cercles à convertir:\n" ) );
    long length = getSsCircle( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) cercle(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere le cercle
        AcDbCircle* circle = getCircleFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur le cercle est null
        if( circle )
        {
            //conversion du cercle
            AcDb3dPolyline* poly3D = circleToPoly3D( circle, g_hasToErase, g_toleranceFleche );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly3D );
            
            //fermer la polyligne 3D
            poly3D->close();
            
            //iterer le nombre de cercles convertis
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de cercles convertis : %d \n" ), nbConverts );
}


void cmdCircleToArc()
{
    ads_name ss;
    int nbConverts = 0, nbArc = 0, n = 4;
    
    //demander au dessinateur de Sélectionner les cercles à convertir
    acutPrintf( _T( "\n Sélectionner les cercles à convertir:\n" ) );
    long length = getSsCircle( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) cercle(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    acedGetInt( _T( "Nombre d'arcs demandes (superieur à 1 ): <4>\n" ), &n );
    
    while( n < 1 ) acedGetInt( _T( "Nombre d'arcs demandes (strictement superieur à 1 ): <4>\n" ), &n );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbCircle* circle = getCircleFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( circle )
        {
            nbArc += circleToArc( circle, blockTable, g_hasToErase, n );
            
            //iterer le nombre de polylignes converties
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de cercles convertis : %d \n Nombre d'arcs obtenus: %d\n" ), nbConverts, nbArc );
}


void cmdSplineToLine()
{
    double tol = 0;
    ads_name ss;
    // Nombre de splines converties
    int nbConverts = 0;
    int nbLine = 0;
    
    //demander au dessinateur de Sélectionner les splines à convertir
    acutPrintf( _T( "Sélectionner les splines à convertir:\n" ) );
    
    long length = getSsSpline( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) spline(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbSpline*  spline = getSplineFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( spline )
        {
            Adesk::Boolean isClosed = spline->isClosed();
            nbLine += splineToLine( spline, blockTable, g_hasToErase, isClosed, tol );
            
            //iterer le nombre de polylignes converties
            nbConverts++;
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de splines converties : %d \n Nombre de ligne obtenues: %d\n" ), nbConverts, nbLine );
}


void cmdSplineToPoly2D()
{
    double tol = 0;
    ads_name ss;
    
    // Nombre de splines converties
    int nbConverts = 0;
    
    //demander au dessinateur de Sélectionner les splines à convertir
    acutPrintf( _T( "Sélectionner les splines à convertir:\n" ) );
    
    long length = getSsSpline( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) spline(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbSpline*  spline = getSplineFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( spline )
        {
            //Convertir le polyligne en polyligne 2D
            AcDbPolyline* polyline = splineToPoly2D( spline, g_hasToErase,  tol );
            
            //Ajouter a la modelspace
            addToModelSpaceAlreadyOpened( blockTable, polyline ) ;
            
            //Si polyline n'est pas null
            if( polyline )
                polyline->close();
                
            //iterer le nombre de polylignes converties
            nbConverts++;
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de splines converties : %d \n  " ), nbConverts );
}


void cmdSplineToPoly3D()
{
    double tol = 0;
    ads_name ss;
    // Nombre de splines converties
    int nbConverts = 0;
    
    //demander au dessinateur de Sélectionner les splines à convertir
    acutPrintf( _T( "Sélectionner les splines à convertir:\n" ) );
    
    long length = getSsSpline( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) spline(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere l'arc
        AcDbSpline*  spline = getSplineFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur l'arc est null
        if( spline )
        {
            //Convertir le spline en polyligne 3D
            AcDb3dPolyline* poly3D = splineToPoly3D( spline, g_hasToErase, tol );
            addToModelSpaceAlreadyOpened( blockTable, poly3D ) ;
            
            //Si polyline 3D n'est pas null
            if( poly3D )
                poly3D->close();
                
            //iterer le nombre de polylignes converties
            nbConverts++;
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de splines converties : %d \n  " ), nbConverts );
}


void cmdFaceToXml()
{
    // on ouvre le fichier
    AcString fileName = curDoc()->fileName();
    
    acutPrintf( fileName );
    
    AcString path, name, ext;
    
    splitPath( fileName, path, name, ext );
    
    ofstream file( acStrToStr( path ) + acStrToStr( name ) + ".xml" );
    string test = acStrToStr( path ) + "\\" + acStrToStr( name ) + ".xml";
    
    //Ecrire l'entete
    file << "<?xml version=\"1.0\"\?>";
    file << "\n<LandXML xmlns=\"http://www.landxml.org/schema/LandXML-1.2\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.landxml.org/schema/LandXML-1.2 http://www.landxml.org/schema/LandXML-1.2/LandXML-1.2.xsd\" date=\"2019-08-13\" time=\"11:16:50\" version=\"1.2\" language=\"English\" readOnly=\"false\">";
    file << "\n<Units><Metric areaUnit=\"squareMeter\" linearUnit=\"meter\" volumeUnit=\"cubicMeter\" temperatureUnit=\"celsius\" pressureUnit=\"milliBars\" diameterUnit=\"millimeter\" angularUnit=\"decimal degrees\" directionUnit=\"decimal degrees\"></Metric></Units>";
    file << "\n<Project name=\"MNT\"></Project>";
    file << "\n<Surfaces>";
    
    int iFace = 0;
    vector< AcString > layers = getLayers();
    int size = layers.size();
    
    for( int i = 0; i < size; i++ )
        iFace += exportFaceToXml( &file, acStrToStr( layers[ i ] ) );
        
    //Terminer le fichier
    file << "\n</Surfaces>";
    file << "\n</LandXML>";
    
    // on ferme le fichier
    file.close();
    
    //Afficher le resultat
    acutPrintf( _T( "\n%d faces traitees" ), iFace );
}


void cmdFaceToLine()
{
    ads_name ss;
    // Nombre de faces à convertir
    int nbConverts = 0;
    int nbLine = 0;
    
    //demander au dessinateur de Sélectionner les faces à convertir
    acutPrintf( _T( "Sélectionner les faces à convertir:\n" ) );
    
    long length = getSsFace( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les faces" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuperer l'objet face
        AcDbFace* face = getFaceFromSs( ss, i, AcDb::kForWrite );
        
        if( face )
        {
            nbLine += faceToLine( face, g_hasToErase, blockTable );
            
            //fermer les faces
            nbConverts++;
            
            // incrementer la progression de la commande
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de faces converties : %d \n  " ), nbConverts );
}


void cmdFaceToPoly2D()
{
    ads_name ss;
    
    // Nombre de faces converties
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les faces 3D à convertir
    acutPrintf( _T( "\n Sélectionner les faces à convertir:\n" ) );
    long length = getSsFace( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les faces" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuperer l'objet face
        AcDbFace* face = getFaceFromSs( ss, i, AcDb::kForWrite );
        
        if( face )
        {
            //faire la conversion
            AcDbPolyline* poly2D = faceToPoly2D( face, g_hasToErase );
            
            //Ajouter au modelSpace
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer le poly2D
            poly2D->close();
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    acutPrintf( _T( "Nombre de faces converties : %d \n  " ), nbConverts );
}


void cmdFaceTo3DPoly2D()
{
    ads_name ss;
    
    // Nombre de faces à convertir
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les faces 3D à convertir
    acutPrintf( _T( "\nSélectionner les faces à convertir:\n" ) );
    long length = getSsFace( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les faces" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la face
        AcDbFace* face = getFaceFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur la face est null
        if( face )
        {
            // conversion de la face
            int poly2D = faceTo3DPoly2D( face, g_hasToErase, blockTable );
            
            // incrementer le nombre d'arc converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}


void cmdFaceToPoly3D()
{
    ads_name ss;
    
    // Nombre de faces convertis
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les faces 3D à convertir
    acutPrintf( _T( "\nSélectionner les faces à convertir:\n" ) );
    long length = getSsFace( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les faces" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere la face
        AcDbFace* face = getFaceFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur la face est null
        if( face )
        {
            // conversion de la face
            AcDb3dPolyline* poly3D = faceToPoly3D( face, g_hasToErase );
            
            // ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly3D );
            
            // fermer la polyligne 3D
            poly3D->close();
            
            // incrementer le nombre de face converties
            nbConverts++;
            
            // Incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}


void cmdEllipseToLine()
{
    ads_name ss;
    
    // Nombre de faces à convertir
    int nbConverts = 0, nbSommet = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les ellipses à convertir
    acutPrintf( _T( "\nSélectionner les ellipses à convertir:\n" ) );
    long length = getSsEllipse( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les ellipses" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuperer l'objet ellipse
        AcDbEllipse* ellipse = getEllipseFromSs( ss, i, AcDb::kForWrite );
        
        //si l'objet ellipse contient vraiment de l'ellipse
        if( ellipse )
        {
            //conversion de l'ellipse
            nbSommet += ellipseToLine( ellipse, blockTable, tol, g_hasToErase );
            
            //incrementer le nombre d'arc converties
            nbConverts++;
        }
        
        //incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}


void cmdEllipseToPoly2D()
{
    ads_name ss;
    
    // Nombre de faces à convertir
    int nbConverts = 0;
    double tol = 0;
    
    // Demander au dessinateur de Sélectionner les ellipses 3D à convertir
    acutPrintf( _T( "\nSélectionner les ellipses à convertir:\n" ) );
    long length = getSsEllipse( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les ellipses" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        //recuperer l'objet ellipse
        AcDbEllipse* ellipse = getEllipseFromSs( ss, i, AcDb::kForWrite );
        
        //
        if( ellipse )
        {
            //faire la transformation
            AcDbPolyline* poly2D = ellipseToPoly2D( ellipse, tol, g_hasToErase );
            
            //ajouter le polyligne au modelspace
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //incrementer le compteur d'objet convertis
            nbConverts++;
            
            //fermer le polyligne
            poly2D->close();
        }
        
        //incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}


void cmdEllipseToArc()
{
    acutPrintf( _T( "Sélectionner les ellipses \n" ) );
    
    ads_name ss;
    int nbConverts = 0;
    
    double tol = 0;
    //Sélectionner les ellipses
    long length = getSsEllipse( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les ellipses" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    for( int i = 0; i < length; i++ )
    {
    
        //Recuperer l'ellipse de la selection
        if( AcDbEllipse* ellipse = getEllipseFromSs( ss, i, AcDb::kForWrite ) )
        {
            //convertion de l'ellipse en arc
            ellipseToArc( ellipse, blockTable, tol, g_hasToErase );
        }
        
        //incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}


void cmdEllipseToPoly3D()
{
    acutPrintf( _T( "Sélectionner les ellipses \n" ) );
    
    ads_name ss;
    double tol = 0;
    int nbConverts = 0;
    //Sélectionner les ellipses
    long length = getSsEllipse( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les ellipses" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    //Creer et ouvrir la base de donnee
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    for( int i = 0; i < length; i++ )
    {
    
        //Recuperer l'ellipse de la selection
        if( AcDbEllipse* ellipse = getEllipseFromSs( ss, i, AcDb::kForWrite ) )
        {
            //convertion de l'ellipse en arc
            AcDb3dPolyline* poly = ellipseToPolyline( ellipse, tol, g_hasToErase );
            
            //incrementer le compteur d'objet convertis
            nbConverts++;
            
            //ajouter le polyligne a la modelspace
            addToModelSpaceAlreadyOpened( blockTable, poly );
            
            //incrementer le nombre d'ellipse convertis
            nbConverts++;
            
            //Fermer l'objet polyligne
            poly->close();
        }
        
        //incrementer la bare de progression
        prog.moveUp( i );
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de faces converties : %d \n" ), nbConverts );
}

void cmdCircleToPoly2DSeg()
{
    ads_name ss;
    int nbConverts = 0;
    double tol = 0;
    
    //demander au dessinateur de Sélectionner les cercles à convertir
    acutPrintf( _T( "\n Sélectionner les cercles à convertir:\n" ) );
    long length = getSsCircle( ss );
    
    // Oui s'il faut supprimer les elements, Non sinon
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer le(s) cercle(s)" ) ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de tolerance :" ), g_toleranceFleche, tol ) == RTCAN )
    {
        acedSSFree( ss );
        return;
    }
    
    // Initialistion de la bare de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  length );
    
    //ouverture de la base de donnees
    AcDbBlockTableRecord* blockTable = openModelSpace();
    
    //boucle sur la selection
    for( int i = 0; i < length; i++ )
    {
        // recupere le cercle
        AcDbCircle* circle = getCircleFromSs( ss, i, AcDb::kForWrite );
        
        //tester si le pointeur sur le cercle est null
        if( circle )
        {
            //conversion du cercle
            AcDbPolyline* poly2D = circleToPoly2DSeg( circle, g_hasToErase, g_tolerance );
            
            //ajout à la base de donnees
            addToModelSpaceAlreadyOpened( blockTable, poly2D );
            
            //fermer la polyligne 3D
            poly2D->close();
            
            //iterer le nombre de cercles convertis
            nbConverts++;
            
            // incrementer la barre de progression
            prog.moveUp( i );
        }
    }
    
    //fermer la base de donnees
    closeModelSpace( blockTable );
    
    //liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre de cercles convertis : %d \n" ), nbConverts );
}


void cmdIntersectPoly3D()
{
    // Demander au dessinateur de Sélectionner les polylignes 3D dont les intersections sont à chercher
    acutPrintf( _T( "\n Sélectionner les polylignes 3D" ) );
    
    ads_name ss;
    
    long length = getSsPoly3D( ss );
    
    if( !length )
    {
        // Liberer la selection
        acutPrintf( _T( "\n Aucune polyligne sélectionnée" ) );
        return;
    }
    
    
    long int start = GetTickCount();
    
    /// 1. On vérifie avant tout si on a besoin de translation
    AcGePoint3d orgOfDistance = AcGePoint3d::kOrigin;
    AcGePoint3d nearestPoint;
    
    // Calcule la plus courte distance entre la 1ère polyligne et l'origine
    AcDb3dPolyline* firstPoly3D = getPoly3DFromSs( ss, 0, AcDb::kForWrite );
    firstPoly3D->getClosestPointTo( orgOfDistance, nearestPoint );
    AcGeVector3d vecOfTranslation = AcGeVector3d::kIdentity;
    
    std:: vector<AcDbObjectId> poly3DIdVector;
    long intersectionCount = 0;
    
    // True si on travaille en grandes coordonnées, false sinon
    bool isTranslationNeeded = hasToTranslate( firstPoly3D, BIG_DISTANCE_REF, vecOfTranslation );
    
    // Fermer la 1ère polyligne
    firstPoly3D->close();
    
    // Créer la matrice de translation
    AcGeMatrix3d matrixOfTranslation = AcGeMatrix3d::translation( vecOfTranslation );
    
    // On effectue la translation avant tout si besoin
    
    // On sauvegarde tous les id des polylines sélectionnées
    for( long nPoly3dIndex = 0; nPoly3dIndex < length; nPoly3dIndex++ )
    {
        // Récuperer la poly3D
        if( AcDb3dPolyline* poly3D = writePoly3DFromSs( ss, nPoly3dIndex ) )
        {
            if( isTranslationNeeded ) // Translation des polylines vers l'origine
                poly3D->transformBy( matrixOfTranslation );
                
            // On garde l'id
            poly3DIdVector.push_back( poly3D->objectId() );
            //On ferme la poly
            poly3D->close();
        }
    }
    
    // On sauvegarde tous les id des polylines sélectionnées
    
    
    long size = poly3DIdVector.size();
    
    // Initialistion de la barre de progression avec le nombre de polyline de la section plus 5 pour faire avancer la bare de progression lors des calculs sur de grand fichiers
    ProgressBar* prog = new ProgressBar( _T( "Polyline(s) 3D traitée(s)" ),  size );
    
    // On parcours l'ensemble des poly3D
    for( long nPoly3dIndexPrimary = 0; nPoly3dIndexPrimary < size; nPoly3dIndexPrimary ++ )
    {
        AcDbObjectId currentId = poly3DIdVector.at( nPoly3dIndexPrimary );
        
        for( long nPoly3dIndex = nPoly3dIndexPrimary + 1; nPoly3dIndex < length; nPoly3dIndex ++ )
        {
            // On cherche les intersections
            AcGePoint3dArray intersectPoint3DArray;
            
            AcDbObjectId nextId = poly3DIdVector.at( nPoly3dIndex );
            
            // On cherche les intersections entre les 2 poly3D
            intersectTwoPoly3D( currentId, nextId, intersectPoint3DArray, g_toleranceIntersect );
            
            // On test si on a trouvé ou non au moin une intersection
            // Si aucune intersection on passe à la suivante
            if( intersectPoint3DArray.length() == 0 )
                continue;
                
            // On comptabilise les intersections trouvées
            intersectionCount += intersectPoint3DArray.length();
            
            // On insert les intersections
            insertVertexesToPoly3DId( currentId, intersectPoint3DArray, g_toleranceIntersect );
            
            // On projette les intersections sur l'autre poly3D
            AcGePoint3dArray projectedIntersectPoint3DArray;
            
            projectToPoly3D( nextId, intersectPoint3DArray, projectedIntersectPoint3DArray, g_toleranceIntersect );
            
            // On insert les intersections sur l'autre poly3D
            
            insertVertexesToPoly3DId( nextId, projectedIntersectPoint3DArray, g_toleranceIntersect );
            
            // On efface une fois utilisé
            intersectPoint3DArray.removeAll();
            projectedIntersectPoint3DArray.removeAll();
            
        }
        
        // incrementer la barre de progression
        prog->moveUp( nPoly3dIndexPrimary );
        
    }
    
    delete prog;
    
    // Si besoin on remet les polylines à leurs places
    if( isTranslationNeeded )
    {
        AcGeMatrix3d matrixOfTranslationInverse = matrixOfTranslation.inverse( g_tolerance );
        
        for( long nPoly3dIndex = 0; nPoly3dIndex < length; nPoly3dIndex++ )
        {
            // Récuperer une poly3Din
            AcDb3dPolyline* poly3D = getPoly3DFromSs( ss, nPoly3dIndex, AcDb::kForWrite );
            
            // Tester si le pointeur sur la  polyline3d est null
            if( poly3D )
            {
                poly3D->transformBy( matrixOfTranslationInverse );
                poly3D->close();
            }
        }
        
    }
    
    // Liberer la selection
    acedSSFree( ss );
    
    //afficher les resultats
    acutPrintf( _T( "Nombre d'intersections trouvées : %d \n" ), intersectionCount );
    
    
    int end = GetTickCount();
    
    //afficher le temsp d'exécustion
    acutPrintf( _T( "Temps de calcul  : %d secondes\n" ), ( end - start ) / 1000 );
    
    
}
void cmdIntersectPoly3DLine()
{
    //Selection sur les lignes et les polylignes
    ads_name ssPoly, ssLine;
    
    //Afficher message
    acutPrintf( _T( "Selectionner les polylignes" ) );
    
    //Recuperer les polylignes
    long lenPoly = getSsPoly3D( ssPoly );
    
    //Verifier
    if( lenPoly == 0 )
        return;
        
    //Afficher message
    acutPrintf( _T( "Selectionner les lignes" ) );
    
    //Recuperer les lignes
    long lenLine = getSsLine( ssLine );
    
    //Verifier
    if( lenLine == 0 )
    {
        //Liberer les selections
        acedSSFree( ssPoly );
        return;
    }
    
    //Compteur
    int comp = 0;
    
    //Boucle sur les polylignes
    for( int p = 0; p < lenPoly; p++ )
    {
        //Polyligne3d
        AcDb3dPolyline* poly3D = getPoly3DFromSs( ssPoly, p, AcDb::kForWrite );
        
        if( poly3D )
        {
            //Vecteur des bounding box de chaque segment de polyligne
            vector<AcDbExtents> bbVec;
            
            //Vecteur des segments de polyligne
            vector<Segment> segVec;
            
            //Vecteur qui va contenir les intersections
            AcGePoint3dArray vecNewPoint;
            
            //Vecteur contenant les xMin des boundingBox
            vector<double> bbVecXMin;
            
            //Recuperer les boundings boxs de la poly
            Acad::ErrorStatus er = computeCurveBoundingBox( poly3D,
                    bbVec,
                    bbVecXMin,
                    segVec );
                    
            if( er != Acad::eOk )
            {
                //Fermer la polyligne
                poly3D->close();
                
                continue;
            }
            
            //Trier les boundingsboxes
            er = sortBoundingBox( bbVec,
                    bbVecXMin,
                    segVec );
                    
            //Verifier que le tri est parfait
            if( er != Acad::eOk )
            {
                //Fermer la polyligne
                poly3D->close();
                
                acedSSFree( ssPoly );
                acedSSFree( ssLine );
                
                return;
            }
            
            //Boucle sur les lignes
            for( int l = 0; l < lenLine ; l++ )
            {
                //Recuperer la ligne
                AcDbLine* line = getLineFromSs( ssLine, l );
                
                //Booleen d'intersection
                bool areIntersected = false;
                
                //Segment qui va contenir l'intersection
                Segment segOfIntersection;
                
                //Le point d'intersection
                AcGePoint3dArray intersectionPoint;
                
                //Index de l'objectId du vertex d'avant dans le vecteur
                int indexOfPrevId;
                
                //Recuperer l'intersection
                er = isIntersectedWithCurve( line,
                        bbVec,
                        segVec,
                        areIntersected,
                        segOfIntersection,
                        intersectionPoint );
                        
                if( er != Acad::eOk )
                {
                    //Fermer la polyligne
                    poly3D->close();
                    
                    //Fermer la ligne
                    line->close();
                    
                    continue;
                }
                
                //Fermer la ligne
                line->close();
                
                //Ajouter le point trouvé dans le vecteur²
                if( areIntersected )
                {
                    vecNewPoint.insert( vecNewPoint.begin(),
                        intersectionPoint.begin(),
                        intersectionPoint.end() );
                }
            }
            
            //Recreer la polyligne
            addArVertexOnPoly( vecNewPoint, poly3D, true );
            
            //Fermer la polyligne
            poly3D->close();
            
            //Compter
            comp += vecNewPoint.size();
        }
    }
    
    //Liberer les selections
    acedSSFree( ssPoly );
    acedSSFree( ssLine );
    
    //Afficher message
    print( "Nombre de sommets insérés: %d", comp );
}


void cmdConvertPoly3DToPoly2DArc()
{
    //Valeur de tolerance
    double tol = 0.01;
    double g_tol = 0.01;
    
    //Selection sur les polylignes 3d
    ads_name ssPoly3D;
    
    //Afficher message
    print( "Selectionner les polylignes 3D" );
    
    //Recuperer les polylignes
    long lenPoly3D = getSsPoly3D( ssPoly3D );
    
    //Verifier
    if( lenPoly3D == 0 )
        return;
        
    // Demander au dessinateur s'il veut supprimer les elements
    if( askYesOrNo( g_hasToErase, _T( "Voulez-vous supprimer les éléments ? :" ) ) == RTCAN )
    {
        acedSSFree( ssPoly3D );
        return;
    }
    
    // Demander le pas de discretisation
    if( askForDouble( _T( "Valeur de discrétisation:" ), g_tol, tol ) == RTCAN )
    {
        acedSSFree( ssPoly3D );
        return;
    }
    
    //Barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  lenPoly3D );
    
    //Compteur
    int comp = 0;
    
    //Polyligne 3d
    AcDb3dPolyline* poly3D;
    
    //Boucle sur les polylignes 3D
    for( int i = 0; i < lenPoly3D; i++ )
    {
        //Recuperer la polyligne
        poly3D = getPoly3DFromSs( ssPoly3D, i, AcDb::kForWrite );
        
        if( poly3D )
        {
            //Appeler la fonction qui va changer la polyligne3d en polyligne2d avec arc
            Acad::ErrorStatus er = convertPoly3DToPoly2DArc( poly3D,
                    g_tol );
                    
            //Verifier
            if( er != Acad::eOk )
            {
                //Fermer la polyligne
                poly3D->close();
                
                continue;
            }
            
            //Compter
            comp++;
            
            //Si l'utilisateur veut supprimer la polyligne
            if( g_hasToErase )
                poly3D->erase( true );
                
            //Fermer la polyligne 3d
            poly3D->close();
        }
        
        //Progresser
        prog.moveUp( i );
    }
    
    //Liberer la selection
    acedSSFree( ssPoly3D );
    
    //Afficher message
    print( "Nombre de polylignes converties: %d", comp );
}


void cmdIntersectPoly2DLine()
{
    //Declaration des variables à utiliser
    ads_name ssPoly;
    ads_name ssLine;
    long nbPtIntersect = 0;
    
    //Selectionner les polylignes
    print( "Sélectionner les polylignes 2D:" );
    long lnPoly = getSsPoly2D( ssPoly );
    
    //Selectionner les lignes
    print( "Sélectionner les lignes:" );
    long lnLine = getSsLine( ssLine );
    
    //Verifier les sélections
    if( acedSSLength( ssPoly, &lnPoly ) != RTNORM || acedSSLength( ssLine, &lnLine ) != RTNORM || lnPoly == 0 || lnLine == 0 )
    {
    
        //Liberer les sélections
        acedSSFree( ssPoly );
        acedSSFree( ssLine );
        
        //Sortir
        return;
    }
    
    //Boucler sur la sélection de la polyligne
    for( int iPoly = 0; iPoly < lnPoly; iPoly++ )
    {
        //Recuperer la ième polyligne de la sélection
        AcDbPolyline* poly = getPoly2DFromSs( ssPoly, iPoly, AcDb::kForWrite );
        
        if( !poly )
            continue;
            
        //Vecteur des bounding box de chaque segment de polyligne
        vector<AcDbExtents> bbVec;
        
        //Vecteur des segments de polyligne
        vector<Segment> segVec;
        
        //Vecteur qui va contenir les intersections
        AcGePoint3dArray vecNewPoint;
        
        //Vecteur contenant les xMin des boundingBox
        vector<double> bbVecXMin;
        
        //Recuperer les boundings boxs de la poly
        Acad::ErrorStatus er = computeCurveBoundingBox( poly,
                bbVec,
                bbVecXMin,
                segVec );
                
        if( er != Acad::eOk )
        {
            //Fermer la polyligne
            poly->close();
            
            continue;
        }
        
        //Trier les boundingsboxes
        er = sortBoundingBox( bbVec,
                bbVecXMin,
                segVec );
                
        //Verifier que le tri est parfait
        if( er != Acad::eOk )
        {
            //Fermer la polyligne
            poly->close();
            
            acedSSFree( ssPoly );
            acedSSFree( ssLine );
            
            return;
        }
        
        //Boucler sur la sélection des lignes
        for( int iLine = 0; iLine < lnLine; iLine++ )
        {
            //Recuperer la ième ligne
            AcDbLine* line = getLineFromSs( ssLine, iLine );
            
            if( !line )
                continue;
                
            //Booleen d'intersection
            bool areIntersected = false;
            
            //Segment qui va contenir l'intersection
            Segment segOfIntersection;
            
            //Le point d'intersection
            AcGePoint3dArray intersectionPoint;
            
            //Index de l'objectId du vertex d'avant dans le vecteur
            int indexOfPrevId;
            
            //Recuperer l'intersection
            er = isIntersectedWithCurve( line,
                    bbVec,
                    segVec,
                    areIntersected,
                    segOfIntersection,
                    intersectionPoint );
                    
            if( er != Acad::eOk )
            {
                //Fermer la polyligne
                poly->close();
                
                //Fermer la ligne
                line->close();
                
                continue;
            }
            
            //Fermer la ligne
            line->close();
            
            //Ajouter le point trouvé dans le vecteur
            if( areIntersected )
            {
                vecNewPoint.insert( vecNewPoint.begin(),
                    intersectionPoint.begin(),
                    intersectionPoint.end() );
            }
        }
        
        //Recréer la polyligne
        er = addArVertexOnPoly( vecNewPoint, poly, true );
        
        if( er != Acad::eOk )
        {
            //Fermer la polyligne
            poly->close();
            
            //Liberer les selections
            acedSSFree( ssPoly );
            acedSSFree( ssLine );
            return ;
        }
        
        
        //Fermer l'objet AcDbPolyline
        poly->close();
        
        //Compter les nombres d'intersections ajoutées
        nbPtIntersect += vecNewPoint.size();
    }
    
    //Afficher message
    print( "Nombre de sommet insérés : %d", nbPtIntersect );
    
    //Liberer les sélections
    acedSSFree( ssPoly );
    acedSSFree( ssLine );
}

void cmdTileAll()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, ssall;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    // Récuperer tous les objets du dessin
    long lnall = getSsAllObject( ssall );
    
    // Verifier la selection
    if( acedSSLength( ssall, &lnall ) != RTNORM || lnall == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( ssall );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les objets et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( ssall, vectdalle, false, true ) )
    {
        print( "Commande annulée." );
        acedSSFree( ssall );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    
    // Liberer la selection
    acedSSFree( ssall );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
    
}

void  cmdTilePoly()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, sspoly;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    
    // Récuperer tous les polylignes 3d du dessin
    long lnpoly = getSsAllObject( sspoly );
    
    // Verifier la selection
    if( acedSSLength( sspoly, &lnpoly ) != RTNORM || lnpoly == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( sspoly );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les polylignes 3d et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( sspoly, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( sspoly );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Liberer la selection
    acedSSFree( sspoly );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
}


void cmdTileCurve()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, sscurve;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    // Récuperer tous les curves du dessin
    long lncurve = getSsAllObject( sscurve );
    
    // Verifier la selection
    if( acedSSLength( sscurve, &lncurve ) != RTNORM || lncurve == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( sscurve );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les curves et ajouter les dans leurs dalles respectives
    if( eOk !=  ExportEntitiesInDalle( sscurve, vectdalle, true, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( sscurve );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    
    // Liberer la selection
    acedSSFree( sscurve );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
    
}

void cmdTileLine()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, ssline;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    // Récuperer tous les lignes du dessin
    long lnline = getSsAllLine( ssline );
    
    // Verifier la selection
    if( acedSSLength( ssline, &lnline ) != RTNORM || lnline == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( ssline );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les lignes et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( ssline, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( ssline );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    
    // Liberer la selection
    acedSSFree( ssline );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
    
}

void cmdTileBlock()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, ssblock;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    
    // Récuperer tous les blocks  du dessin
    long lnblock = getSsAllBlock( ssblock );
    
    // Verifier la selection
    if( acedSSLength( ssblock, &lnblock ) != RTNORM || lnblock == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( ssblock );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les blocks et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( ssblock, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( ssblock );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Liberer la selection
    acedSSFree( ssblock );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
}

void cmdTilePoint()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, sspoint;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    // Récuperer tous les points 3d du dessin
    long lnpoint = getSsAllPoint( sspoint, _T( "POINT-DENS_ALT" ) );
    
    // Verifier la selection
    if( acedSSLength( sspoint, &lnpoint ) != RTNORM || lnpoint == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( sspoint );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les points et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( sspoint, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( sspoint );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Liberer la selection
    acedSSFree( sspoint );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
}

void cmdTileText()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, sstxt;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    
    // Récuperer tous les textes  du dessin
    long lntxt = getSsAllText( sstxt, _T( "POINT-DENS_ALT" ) );
    
    // Verifier la selection
    if( acedSSLength( sstxt, &lntxt ) != RTNORM || lntxt == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( sstxt );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les textes et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( sstxt, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( sstxt );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Liberer la selection
    acedSSFree( sstxt );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
}

void cmdTileHatch()
{
    // Déclarer les selections
    ads_name ssdalle, sstext, sshatch;
    vector<Dalle> vectdalle;
    
    // Exporter les dalles
    if( eOk != exportDalles( ssdalle, sstext, vectdalle ) )
    {
        // Libérer la sélection
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        print( "Commande annulée." );
        return;
    }
    
    
    // Récuperer tous les hachures  du dessin
    long lnhatch = getSsAllHatch( sshatch );
    
    // Verifier la selection
    if( acedSSLength( sshatch, &lnhatch ) != RTNORM || lnhatch == 0 )
    {
        print( "Sélection vide, sortie." );
        acedSSFree( sshatch );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Verifier les hachures et ajouter les dans leurs dalles respectives
    if( eOk != ExportEntitiesInDalle( sshatch, vectdalle, false, false ) )
    {
        print( "Commande annulée." );
        acedSSFree( sshatch );
        acedSSFree( ssdalle );
        acedSSFree( sstext );
        return;
    }
    
    // Liberer la selection
    acedSSFree( sshatch );
    acedSSFree( ssdalle );
    acedSSFree( sstext );
}

void cmdSuperposePoly()
{
    //Selection sur les polylignes
    ads_name ssUsr;
    ads_point point;
    
    //Polyligne
    AcDbEntity* ent;
    AcDb3dPolyline* selPoly = NULL;
    
    acedInitGet( 128, NULL );
    
    //Demander au dessinateur de Sélectionner la FMPoly3D et ou insérer le point
    int es = acedEntSel( _T( "\nCliquez sur une polyligne ou cliquer un point pour dessiner" ), ssUsr, point );
    
    //On test le retour
    if( es == RTNORM )
    {
        AcDbObjectId id = AcDbObjectId::kNull;
        
        //On récupère l'Id de la pièce sélectionnée
        acdbGetObjectId( id, ssUsr );
        
        //On récupère un pointeur sur l'entity sélectionnée
        AcDbEntity* pEnt = NULL;
        acdbOpenAcDbEntity( pEnt, id, AcDb::kForWrite );
        
        if( pEnt )
        {
            // Vérification du type de class
            if( pEnt->isKindOf( AcDb3dPolyline::desc() ) )
                selPoly = AcDb3dPolyline::cast( pEnt );
            else
                pEnt->close();
        }
    }
    
    else if( es == RTERROR )
        acedSSFree( ssUsr );
        
    else
    {
        //Liberer la selection et afficher le resultat
        acedSSFree( ssUsr );
        print( _T( "Opération annulée" ) );
        
        return;
    }
    
    //Recuperer le point
    AcGePoint3d ptRef = adsToAcGe( point );
    
    //On récupère le point dans le SCU général
    getWcsPoint( &ptRef );
    
    // Tester si le pointeur sur la polyligne est bon
    if( selPoly )
    {
        bool sameLayer = false;
        bool sameColor = false;
        bool sameLine = false;
        bool sameLineWeight = false;
        
        //Changer la propriétés de la polyligne
        // Calque
        AcDbObjectId idLayer =  getCurrentLayerId();
        
        if( selPoly->layerId() == idLayer )
            sameLayer = true;
        else
        {
            ErrorStatus es = selPoly->setLayer( idLayer );
            
            if( es != eOk )
            {
                selPoly->close();
                
                return;
            }
        }
        
        // Couleur
        if( selPoly->color() == getCurrentColor() )
            sameColor = true;
        else
        {
            AcCmColor color = getCurrentColor();
            es = selPoly->setColor( color );
            
            if( es != eOk )
            {
                selPoly->close();
                
                return;
            }
        }
        
        // Type de ligne
        if( selPoly->linetypeId() == getCurrentLineTypeId() )
            sameLine = true;
        else
        {
            AcDbObjectId idLineType = getCurrentLineTypeId();
            es = selPoly->setLinetype( idLineType );
            
            if( es != eOk )
            {
                selPoly->close();
                
                return;
            }
        }
        
        // Epaisseur de ligne
        if( selPoly->lineWeight() == getCurrentLineWeight() )
            sameLineWeight = true;
        else
        {
            AcDb::LineWeight lineWeight = getCurrentLineWeight();
            
            es = selPoly->setLineWeight( lineWeight );
            
            if( es != eOk )
            {
                selPoly->close();
                
                return;
            }
        }
        
        //Continuer la polyligne si elles ont la même propriété
        if( sameLayer && sameColor && sameLine && sameLineWeight )
            continuePoly3D( selPoly );
            
        selPoly->close();
    }
    
    else
    {
        //On continue de dessiner la polyligne
        drawNewPoly3D( ptRef );
    }
    
    //Liberer les selections
    acedSSFree( ssUsr );
    
    //Sortir de la fonction
    return;
}


void cmdAddBlocAmor()
{
    //Demander à l'utilisateur de cliquer sur un point
    AcGePoint3d pt1 = AcGePoint3d::kOrigin;
    
    //Demander à l'utilisateur de cliquer le point début
    if( RTNORM != getWcsPoint( _T( "\nCliquer sur un point (utiliser accrochage)" ), &pt1 ) )
    {
        //Afficher le message
        print( "Commande annulée" );
        
        //Sortir
        return;
    }
    
    //Declaration des deux points pour calculer l'angle d'orientation du bloc
    AcGePoint3d ptClic1;
    AcGePoint3d ptClic2;
    
    //Demander à l'utilisateur de cliquer un point sur la polyligne
    if( RTNORM != getWcsPoint( _T( "\nCliquer le premier point ( angle )" ), &ptClic1 ) )
    {
        //Afficher le message
        print( "Commande annulée, Rotation = 0°" );
        
        //Sortir
        return;
    }
    
    //Demander à l'utilisateur de cliquer un second point sur la polyligne
    if( RTNORM != getWcsPoint( _T( "\nCliquer le second point ( angle )" ), &ptClic2, ptClic1 ) )
    {
        //Afficher le message
        print( "Commande annulée, Rotation = 0°" );
        
        //Sortir
        return;
    }
    
    //Creer un vecteur entre le point 1 et le point 2
    AcGeVector2d vec = getVector2d( getPoint2d( ptClic1 ), getPoint2d( ptClic2 ) );
    
    //Recuperer l'angle
    double angle = vec.angle() + ( 3.1415926353 / 2 ) ;
    
    //Ajouter un bloc sur le point
    AcDbBlockReference* block3D = insertBlockReference( _T( "DP_CHLIM" ),
            _T( "DELIM~DP~AMOR~-" ),
            pt1,
            angle );
            
    //Verifier qu'on a bien recuperer le bloc
    if( !block3D )
    {
        //Afficher message
        print( "Impossible de recuperer le bloc, Sortie" );
        
        //Sortir
        return;
    }
    
    //Changer l'angle
    block3D->setRotation( angle );
    
    //Fermer le bloc
    block3D->close();
}



void cmdAbyNetPoly()
{
    //Selection sur les polylignes 2d
    ads_name ssPoly2D;
    
    //Recuperer les polylignes
    int lenPoly2d = getSsPoly2D( ssPoly2D );
    
    //Verifier
    if( !lenPoly2d )
        return;
        
    //Variables
    double def = 0.01;
    double newDef = 0;
    
    //Demander la tolerance
    askForDouble( _T( "Entrez la tolerance" ), def, newDef );
    
    //Polyligne 2d
    AcDbPolyline* poly2D = NULL;
    
    //Compteur
    int count = 0;
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  lenPoly2d );
    
    //Boucle sur les polyligne
    for( int i = 0; i < lenPoly2d; i++ )
    {
        //Recuperer la polyligne
        poly2D = getPoly2DFromSs( ssPoly2D, i, AcDb::kForWrite );
        
        //Verifier
        if( !poly2D )
            continue;
            
        //Nettoyer et compter
        count += netPoly2( poly2D, true, newDef );
        
        //Progresser
        prog.moveUp( i );
    }
    
    //Liberer les selections
    acedSSFree( ssPoly2D );
    
    //Afficher message
    print( "Nombre de sommets supprimés: %d", count );
}



void cmdAbyColiPoly()
{
    //Selection sur les polylignes 2d
    ads_name ssPoly2D;
    
    //Recuperer les polylignes
    int lenPoly2d = getSsPoly2D( ssPoly2D );
    
    //Verifier
    if( !lenPoly2d )
        return;
        
    //Variables
    double def = 0.01;
    double newDef = 0;
    
    //Demander la tolerance
    askForDouble( _T( "Entrez la tolerance" ), def, newDef );
    
    //Polyligne
    AcDbPolyline* poly2D = NULL;
    
    //Compteur
    int count = 0;
    
    // Initialistion de la barre de progression
    ProgressBar prog = ProgressBar( _T( "Progression" ),  lenPoly2d );
    
    //Boucle sur les polylignes
    for( int i = 0; i < lenPoly2d; i++ )
    {
        //Recuperer la polyligne
        poly2D = getPoly2DFromSs( ssPoly2D, i, AcDb::kForWrite );
        
        //Verifier
        if( !poly2D )
            continue;
            
        //Nettoyer la polyligne et compter
        count += coliPoly2( poly2D, true, newDef );
        
        //Progresser
        prog.moveUp( i );
    }
    
    //Liberer la selection
    acedSSFree( ssPoly2D );
    
    //Afficher message
    print( "Nombre de sommets supprimés: %d", count );
}


void cmdCleanTopo()
{
    // Selection des poitns topo
    ads_name ssBlockTopo;
    print( "Selectionner les blocs concernés" );
    
    int lenghBlock = getSsBlock( ssBlockTopo, TOPO_POINTS_PIQUES_BLOCK_LAYER, TOPO_POINTS_PIQUES_BLOCK_NAME );
    
    if( lenghBlock == 0 )
    {
        //Liberer la selection
        acedSSFree( ssBlockTopo );
        return;
    }
    
    AcDbObjectIdArray arIdToDelete;
    
    // Recuperation des points d'isertions
    AcGePoint3dArray arblockPosToKeep, arblockPosToDelete;
    vector<double> vecXToKeep;
    
    for( int counter = 0; counter < lenghBlock; counter++ )
    {
        AcDbBlockReference* block = getBlockFromSs( ssBlockTopo, counter, AcDb::kForWrite );
        
        if( block )
        {
            AcGePoint3d pt = block->position();
            AcString layer = block->layer();
            
            if( ( layer.compare( TOPO_POINTS_HORS_TN_PIQUES_LAYER ) == 0 ) || ( layer.compare( TOPO_POINTS_PIQUES_LAYER ) == 0 ) )
            {
                arblockPosToKeep.append( pt );
                vecXToKeep.push_back( pt.x );
            }
            
            else
                arIdToDelete.append( block->objectId() );
                
            block->close();
        }
    }
    
    //Liberer la selection
    acedSSFree( ssBlockTopo );
    
    // Tri
    if( sortList( arblockPosToKeep, vecXToKeep ) != eOk )
        return;
        
    // Suppression
    int size = arIdToDelete.size();
    
    if( size == 0 )
    {
        print( "Aucun bloc à supprimer" );
        return;
    }
    
    int deleted = 0;
    
    for( int counter = 0; counter < size; counter++ )
    {
        AcDbBlockReference* block = NULL;
        
        if( openBlock( arIdToDelete[counter], block, AcDb::kForWrite ) == eOk )
        {
            AcGePoint3d blockPos = block->position();
            int index;
            
            if( isInList( blockPos, vecXToKeep, arblockPosToKeep, index ) )
            {
                block->erase();
                deleted++;
            }
            
            block->close();
        }
    }
    
    print( "Nombre de blocs supprimés: %d", deleted );
}

void cmdCleanDoublonSP()
{
    // On récupère tous les points topo
    ads_name ssBlockTopo;
    int lenghBlock = getSsBlock( ssBlockTopo, TOPO_POINTS_SURPLUS_PIQUES_LAYER, TOPO_POINTS_PIQUES_BLOCK_NAME );
    
    //Supprimer les doublons des points levés
    cleanSelectionFromDoublons( ssBlockTopo );
    
    acedSSFree( ssBlockTopo );
}

void cmdChangePtTopoWithAtt()
{
    ads_name ssBlkConcerned, ssBlkSoupirailSeuil, ssBlkFmapElv;
    
    print( "Selectionner les blocs seuils, soupirails" );
    int lenBlk = getSsBlock( ssBlkSoupirailSeuil, BLOCK_LAYER, BLOCK_NAMES );
    
    if( lenBlk == 0 )
    {
        print( "Aucun soupirail ni seuil sélectionnées" );
        acedSSFree( ssBlkSoupirailSeuil );
        
        return;
    }
    
    print( "Selectionner les blocs FMAP_Elevation" );
    int lenBlkFE = getSsBlock( ssBlkFmapElv, BLOCK_LAYER, TOPO_FMAP_ELEV_NAME );
    
    if( lenBlkFE == 0 )
    {
        print( "Aucun FMAP_Elevation sélectionnées" );
        acedSSFree( ssBlkFmapElv );
        
        return;
    }
    
    AcGePoint3dArray arPtSeuilEntree;
    AcDbObjectIdArray arIdSeuilEntree;
    //std::vector<double> xPosSeuildEntree;
    
    // Sauvegarde des points d'insertions et orientations des seuils et soupirail
    for( int counter = 0; counter < lenBlk; counter++ )
    {
        AcDbBlockReference* block = getBlockFromSs( ssBlkSoupirailSeuil, counter, AcDb::kForWrite );
        
        if( block )
        {
            AcGePoint3d pt = block->position();
            //xPosSeuildEntree.push_back(pt.x);
            arPtSeuilEntree.append( pt );
            arIdSeuilEntree.append( block->id() );
            block->close();
        }
    }
    
    // Changement des Fmap_Elevation
    for( int idx = 0; idx < lenBlkFE; idx++ )
    {
        AcDbBlockReference* block = getBlockFromSs( ssBlkFmapElv, idx, AcDb::kForWrite );
        
        if( block )
        {
            AcGePoint3d blockPos = block->position();
            
            int index = findInVector3D( arPtSeuilEntree, blockPos );
            
            if( index >= 0 )
            {
                AcDbObjectId blockId = arIdSeuilEntree[index];
                AcDbBlockReference* entreeBlock;
                
                // Modification des attributs
                if( openBlock( blockId, entreeBlock ) == eOk )
                {
                    double rotation = entreeBlock->rotation();
                    entreeBlock->close();
                    bool shouldMiroring = false;
                    AcGeVector3d vec = AcGeVector3d::kXAxis;
                    double dist = 0.0f;
                    
                    if( rotation > M_PI )
                        rotation = rotation - M_PI;
                    else
                    {
                        //rotation = rotation + M_PI;
                        rotation = rotation; -M_PI;
                        
                        shouldMiroring = true;
                        vec.rotateBy( rotation, AcGeVector3d::kZAxis );
                        
                    }
                    
                    AcDbBlockReference* newBlock = insertBlockReference( TOPO_POINTS_PIQUES_BLOCK_NAME, TOPO_POINTS_SEUILS_PIQUES_LAYER, blockPos, rotation - M_PI_2 );
                    
                    if( shouldMiroring )
                        dist = 1.0;
                        
                    bool res1 = setAttributValue( newBlock,
                            _T( "ALTITUDE" ),
                            blockPos.z,
                            TOPO_POINTS_SEUILS_PIQUES_ALTITUDE_LAYER,
                            rotation,
                            dist );
                            
                    res1 = setAttributValue( newBlock,
                            _T( "NUMERO" ),
                            _T( "0" ),
                            TOPO_POINTS_SEUILS_PIQUES_NUMERO_LAYER,
                            rotation );
                            
                            
                    newBlock->close();
                }
                
                block->erase();
                
            }
            
            block->close();
        }
    }
    
}
