#pragma once
#include "Dalles.h"
#include "print.h"
#include "poly2dEntity.h"
#include "list.h"
#include "lineEntity.h"
#include "arcEntity.h"
#include "boundingBox.h"
#include "layer.h"
#include "blockEntity.h"
#include "hatchEntity.h"



Acad::ErrorStatus exportDalles(
    ads_name& ssdalle,
    ads_name& sstext,
    std::vector<Dalle>& vectDalle )
{
    // Calques des élements de la dalle
    AcString layerDalle = _T( "Dalle" ); // Countour des dalles
    AcString layertxt = layerDalle; // Nom des dalles
    
    // Démander le calque des polylignes de découpage
    GCHAR* value = _T( "Dalle" );
    int es = acedGetFullString( 1, _T( "\nEntrez le  calque des polylignes de découpage :<" ) + layerDalle + _T( ">" ), value );
    
    // Verifier
    if( es == RTCAN )
    {
        print( "Commande annulée." );
        return eNotApplicable;
    }
    
    
    // Récuperer la valeur
    AcString valueAcstr = value;
    
    if( valueAcstr.compareNoCase( "" ) != 0 )
        layerDalle = value;
        
    // Demander à l'utilisateur le calque des noms des dalles
    es = acedGetFullString( 1, _T( "\nEntrez le calque des noms de dalles :<" ) + layerDalle + _T( ">" ), value );
    
    // Verifier
    if( es == RTCAN )
    {
        print( "Commande annulée." );
        return eNotApplicable;
    }
    
    
    // Récuperer la valeur
    layertxt = value;
    
    if( layertxt.compareNoCase( "" ) == 0 )
        layertxt = layerDalle;
        
        
    // Selectionner les polylignes de découpages
    long lndalle = getSsAllPoly2D( ssdalle, layerDalle );
    
    // Selectionner les noms de dalles
    long lntext = getSsAllText( sstext, layertxt );
    
    // Récuperer le nombre d'objets selectionnés
    if( acedSSLength( ssdalle, &lndalle ) != RTNORM || lndalle == 0 )
    {
        print( "Sélection vide, Sortie." );
        return eNotApplicable;
    }
    
    // Récuperer la selection des textes dans la dalle
    acedSSLength( sstext, &lntext );
    acedSSLength( sstext, &lntext );
    
    // Boucler sur les dalles
    for( int idalle = 0; idalle < lndalle; idalle++ )
    {
        // Initialisation de la dalle
        Dalle dalle;
        dalle.dallelayer = layerDalle;
        
        
        // Récuperer la polyligne dalle ( On ne ferme pas cette polyligne dans cette fonction on le ferme dans la fonction où on insert l'insert dans la nouvelle base de données)
        AcDbPolyline* poly = getPoly2DFromSs( ssdalle, idalle );
        
        if( !poly )
            continue;
            
        if( layerDalle.compare( poly->layer() ) != 0 )
        {
            poly->close();
            continue;
        }
        
        
        // Recuperer la bounding box de la dalle
        AcDbExtents dallebox;
        poly->getGeomExtents( dallebox );
        
        dalle.dallebox = dallebox;
        
        
        AcGePoint3d ptmin = dallebox.minPoint();
        AcGePoint3d ptmax = dallebox.maxPoint();
        
        double xmin = ptmin.x;
        double xmax = ptmax.x;
        double ymin = ptmin.y;
        double ymax = ptmax.y;
        
        AcString dallename = "";
        
        
        // Boucler sur les textes
        for( int itext = 0; itext < lntext; itext++ )
        {
            // Recuperer le texte
            AcDbText* text = getTextFromSs( sstext, itext, AcDb::kForRead );
            
            if( !text )
                continue;
                
            if( layertxt.compare( text->layer() ) != 0 )
            {
                text->close();
                continue;
            }
            
            // Recuperer la position du texte
            AcGePoint3d position = text->position();
            double xtext = position.x;
            double ytext = position.y;
            
            // Verifier si le texte est dans la bbox de la dalle
            if( xmin <= xtext && xtext <= xmax && ymin <= ytext && ytext <= ymax )
            {
                // Recuperer le texte
                AcString txt = text->textString();
                
                // Récuperer le nom de dalle
                dalle.dallename = txt;
                
                // Récuperer l'id du texte
                dalle.entitiesId.push_back( text->objectId() );
                
                // Fermer le text
                text->close();
                
                // Sortir de la boucle
                break;
            }
            
            // Fermer l'objet texte
            text->close();
        }
        
        // Récuperer l'id de la polyligne dalle
        dalle.entitiesId.push_back( poly->objectId() );
        poly->close();
        
        vectDalle.push_back( dalle );
    }
    
    //Trier les dalles
    return sortDalle( vectDalle );
    
}

Acad::ErrorStatus sortDalle( vector<Dalle>& vectDalle )
{
    // Trier les dalles suivants x d'ordre croissant
    int length = vectDalle.size();
    
    if( length == 0 )
        return eNotApplicable;
        
    for( int i = 0; i < length - 1; i++ )
    {
        // Recuperer l'index du min
        int min = i;
        
        // Boucle de test
        for( int j = i + 1; j < length; j++ )
        {
            if( vectDalle[j].dallebox.minPoint().x < vectDalle[min].dallebox.minPoint().x )
                min = j;
        }
        
        // Swaper le vecteur et les AcArrays
        if( min != i )
            std::swap( vectDalle[i], vectDalle[min] );
            
    }
    
    return Acad::eOk;
}

Acad::ErrorStatus ExportEntitiesInDalle(
    ads_name& ssentities,
    vector<Dalle>& vectDalle,
    const bool& tileCurve,
    const bool& tileAll )
{
    // Récupere la selection des entités
    long lnents = 0;
    
    if( acedSSLength( ssentities, &lnents ) != RTNORM || lnents == 0 )
        return eNotApplicable;
        
    // Parcourir les dalles
    int size = vectDalle.size();
    
    for( int i = 0; i < size; i++ )
    {
        // Recuperer le ième Dalle
        Dalle dalle = vectDalle[i];
        
        // Recuperer la bounding box
        AcDbExtents dalleBox = dalle.dallebox;
        AcString layerdalle = dalle.dallelayer;
        
        // Recuperer les point min et le max
        AcGePoint3d ptMin = dalleBox.minPoint();
        AcGePoint3d ptMax = dalleBox.maxPoint();
        
        for( int ient = 0; ient < lnents; ient++ )
        {
            // Recuperer le ième hachure
            AcDbEntity* entity = getEntityFromSs( ssentities, ient, AcDb::kForWrite );
            
            if( !entity )
                continue;
                
            if( layerdalle.compare( entity->layer() ) == 0 )
            {
                entity->close();
                continue;
            }
            
            if( tileCurve )
            {
                if( !entity->isKindOf( AcDbCurve::desc() ) )
                {
                    entity->close();
                    continue;
                }
                
            }
            
            // Récuperer le bounding box de l'entité
            AcDbExtents bbox;
            entity->getGeomExtents( bbox );
            AcGePoint2d ptRef = midPoint2d( bbox.minPoint(), bbox.maxPoint() );
            double x = ptRef.x;
            double y = ptRef.y;
            
            if( tileCurve &&  entity->isKindOf( AcDbCurve::desc() ) )
                vectDalle[i].objecttype = _T( "curve" );
            else if( entity->isKindOf( AcDb3dPolyline::desc() ) || entity->isKindOf( AcDbPolyline::desc() ) )
                vectDalle[i].objecttype = _T( "poly" );
            else if( entity->isKindOf( AcDbLine::desc() ) )
                vectDalle[i].objecttype = _T( "ligne" );
            else if( entity->isKindOf( AcDbBlockReference::desc() ) )
                vectDalle[i].objecttype = _T( "bloc" );
            else if( entity->isKindOf( AcDbPoint::desc() ) )
                vectDalle[i].objecttype = _T( "point" );
            else if( entity->isKindOf( AcDbText::desc() ) )
                vectDalle[i].objecttype = _T( "texte" );
            else if( entity->isKindOf( AcDbHatch::desc() ) )
                vectDalle[i].objecttype = _T( "hachure" );
            else if( tileAll )
                vectDalle[i].objecttype = _T( "all" );
                
                
            // Verifier si le centre de bounding box de l'entité est dans la bounding box de la dalle
            if( ptMin.x <= x && x <= ptMax.x && ptMin.y <= y && y <= ptMax.y )
            {
                // Enregistrer la polyligne dans la bounding box
                vectDalle[i].entitiesId.push_back( entity->objectId() );
            }
            
            // Sinon fermer l'entité
            entity->close();
            
        }
    }
    
    // Inserer les entités dans leurs dalles respectives
    
    return insertEntitiesInDalle( vectDalle );
}

Acad::ErrorStatus insertEntitiesInDalle(
    vector<Dalle>& vectDalle,
    const bool& tileCurve
)
{
    // Récuperer le dossier contenant le dwg courant
    AcString folder = getCurrentFileFolder();
    Acad::ErrorStatus er = eNotApplicable;
    
    // Parcourir la dalle
    for( int i = 0; i < vectDalle.size(); i++ )
    {
        // Recupere le ième dalle et ses informations
        Dalle dalle = vectDalle[i];
        
        // Nom de la dalle
        AcString dallename = dalle.dallename;
        AcString filename = "";
        AcString entityname = "";
        AcString objecttype = dalle.objecttype;
        AcDbObjectIdArray entitiesId = dalle.entitiesId;
        
        
        
        // Nommer le nouveau fichier
        if( dallename.compare( "" ) == 0 || objecttype.compare( "" ) == 0 )
            filename = folder + _T( "\\Dalle_" ) + numberToAcString( i + 1 ) + _T( ".dwg" );
            
        else
        {
            if( objecttype.compare( "curve" ) == 0 )
            {
                entityname = _T( "objet" );
                filename = folder + _T( "\\" ) + dallename + _T( "_curve" ) + _T( ".dwg" );
                
            }
            
            else if( objecttype.compare( "poly" ) == 0 )
            {
                entityname = _T( "objet" );
                filename = folder + _T( "\\" ) + dallename + _T( "_poly" ) + _T( ".dwg" );
                
            }
            
            else if( objecttype.compare( "ligne" ) == 0 )
            {
                entityname = _T( "objet" );
                filename = folder + _T( "\\" ) + dallename + _T( "_ligne" ) + _T( ".dwg" );
                
            }
            
            else if( objecttype.compare( "bloc" ) == 0 )
            {
                entityname = _T( "bloc" );
                filename = folder + _T( "\\" ) + dallename + _T( "_bloc" ) + _T( ".dwg" );
            }
            
            else if( objecttype.compare( "point" ) == 0 )
            {
                entityname = _T( "point" );
                filename = folder + _T( "\\" ) + dallename + _T( "_point" ) + _T( ".dwg" );
            }
            
            else if( objecttype.compare( "texte" ) == 0 )
            {
                entityname = _T( "texte" );
                filename = folder + _T( "\\" ) + dallename + _T( "_texte" ) + _T( ".dwg" );
            }
            
            else if( objecttype.compare( "hachure" ) == 0 )
            {
                entityname = _T( "hachure" );
                filename = folder + _T( "\\" ) + dallename + _T( "_hachure" ) + _T( ".dwg" );
            }
            
            else if( objecttype.compare( "all" ) == 0 )
            {
                entityname = _T( "all" );
                filename = folder + _T( "\\" ) + dallename + _T( ".dwg" );
            }
        }
        
        //Créer une nouvelle base de données
        AcDbDatabase* database = new AcDbDatabase( false );
        er = database->readDwgFile( filename );
        
        if( er != eOk )
        {
            delete database;
            database = new AcDbDatabase( true );
        }
        
        AcDbIdMapping idMap;
        
        // Créer une nouvelle blocktable dans cette base de données pour stocker les entités
        AcDbBlockTable* blocktable = NULL;
        database->getSymbolTable( blocktable, AcDb::kForWrite );
        
        // TableRecord
        AcDbBlockTableRecord* blocktablerecord = NULL;
        blocktable->getAt( ACDB_MODEL_SPACE, blocktablerecord, AcDb::kForWrite );
        blocktable->close();
        
        // Ajouter les objets dans la base de données
        database->wblockCloneObjects( entitiesId, blocktablerecord->id(), idMap, AcDb::kDrcIgnore, false );
        
        // Fermer la tableRecord
        blocktablerecord->close();
        
        // Enregistrer la base de données
        er = database->saveAs( filename, false, AcDb::kDHL_1800 );
        
        // Verifier
        if( er != eOk )
            print( "Impossible d'enregistrer le fichier." );
        else
        {
            AcString msg = numberToAcString( entitiesId.size() ) + _T( " objet(s) exporté(s) dans " ) + filename;
            print( msg );
        }
        
        // Fermer la base de données
        database->close();
    }
    
    return er;
}
