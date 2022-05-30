#include "objectReactor.h"

ACRX_DXF_DEFINE_MEMBERS( AsdkObjectToNotify, AcDbObject, AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 0, ASDKOBJECTTONOTIFY, persreac );

void
AsdkObjectToNotify::modified( const AcDbObject* pObj )
{
    AcDbLine* pLine = AcDbLine::cast( pObj );
    
    if( !pLine )
    {
        acutPrintf( _T( "I only work with lines. Sorry.\n" ) );
        return;
    }
    
    acutPrintf( _T( "\nReactor attached to %lx calling %lx.\n" ), pLine->objectId(), mId );
    
    AcDbLine* pLine2;
    
    if( acdbOpenObject( ( AcDbObject*& )pLine2, mId, AcDb::kForWrite ) == Acad::eOk )
    {
        AcGePoint3d p = pLine->startPoint();
        AcGePoint3d q = pLine->endPoint();
        AcGeVector3d v = q - p;
        double len = v.length();
        
        p = pLine2->startPoint();
        q = pLine2->endPoint();
        v = q - p;
        v = len * mFactor * v.normal();
        pLine2->setEndPoint( p + v );
        pLine2->close();
    }
}


Acad::ErrorStatus AsdkObjectToNotify::dwgInFields( AcDbDwgFiler* filer )
{
    assertWriteEnabled();
    AcDbObject::dwgInFields( filer );
    filer->readItem( &mFactor );
    filer->readItem( ( AcDbSoftPointerId* ) &mId );
    return filer->filerStatus();
}


Acad::ErrorStatus AsdkObjectToNotify::dwgOutFields( AcDbDwgFiler* filer ) const
{
    assertReadEnabled();
    AcDbObject::dwgOutFields( filer );
    filer->writeItem( mFactor );
    filer->writeItem( ( AcDbSoftPointerId& )mId );
    return filer->filerStatus();
}


void AsdkObjectToNotify::assocLines()
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    
    AcDbObjectId aId, bId;
    
    AcDbLine* pLineA = new AcDbLine;
    pLineA->setDatabaseDefaults( pDb );
    pLineA->setStartPoint( AcGePoint3d( 1, 1, 0 ) );
    pLineA->setEndPoint( AcGePoint3d( 2, 1, 0 ) );
    addToModelSpace( pLineA );
    
    acutPrintf( _T( "Line A is %lx from 1,1 to 2,1.\n" ), pLineA->objectId() );
    AcDbLine* pLineB = new AcDbLine;
    pLineB->setDatabaseDefaults( pDb );
    pLineB->setStartPoint( AcGePoint3d( 1, 2, 0 ) );
    pLineB->setEndPoint( AcGePoint3d( 2, 2, 0 ) );
    addToModelSpace( pLineB );
    acutPrintf( _T( "Line B is %lx from 1,2 to 2,2.\n" ), pLineB->objectId() );
    
    AcDbDictionary* pNamedObj;
    AcDbDictionary* pNameList;
    pDb->getNamedObjectsDictionary( pNamedObj,
        AcDb::kForWrite );
        
    if( pNamedObj->getAt( _T( "ASDK_DICT" ), ( AcDbObject*& )pNameList, AcDb::kForWrite ) == Acad::eKeyNotFound )
    {
        pNameList = new AcDbDictionary;
        AcDbObjectId DictId;
        pNamedObj->setAt( _T( "ASDK_DICT" ), pNameList, DictId );
    }
    
    pNamedObj->close();
    
    AsdkObjectToNotify* pObj = new AsdkObjectToNotify();
	bId = pLineB->objectId(); 
    pObj->eLinkage( bId );
    AcDbObjectId objId;
	AcString otnA = _T( "object_to_notify_A" );
    
    if( ( pNameList->getAt( otnA, objId ) ) == Acad::eKeyNotFound )
    {
        pNameList->setAt( otnA, pObj, objId );
        pObj->close();
    }
    
    else
    {
        delete pObj;
        acutPrintf( _T( "object_to_notify_A already exists\n" ) );
    }
    
    pLineA->addPersistentReactor( objId );
    pLineA->close();
    
    // Create the AsdkObjectToNotify for line B.
    //
    pObj = new AsdkObjectToNotify();
	aId = pLineA->objectId(); 
    pObj->eLinkage( aId );
	otnA = _T( "object_to_notify_B" );
    
    if( ( pNameList->getAt( otnA, objId ) ) == Acad::eKeyNotFound )
    {
        pNameList->setAt( otnA, pObj, objId );
        pObj->close();
    }
    
    else
    {
        delete pObj;
        acutPrintf( _T( "object_to_notify_B already exists\n" ) );
    }
    
    pNameList->close();
    // Set up persistent reactor link between line B
    // and AsdkObjectToNotify.
    //
    pLineB->addPersistentReactor( objId );
    pLineB->close();
}


/*
void AsdkObjectToNotify::assocObject( const bool& isErased, AcDbObject* objectNotifier, AcDbObject* objectReact )
{
    //On recupere la base de donnée
    AcDbDatabase* database = acdbHostApplicationServices()->workingDatabase();

    //On recupere les ids des deux objets
    AcDbObjectId objectId1 = objectNotifier->id();
    AcDbObjectId objectId2 = objectReact->id();

    AcDbLine* pLineA = AcDbLine::cast( objectNotifier );
    AcDbLine* pLineB = AcDbLine::cast( objectReact );

    pLineA->setDatabaseDefaults( database );
    pLineB->setDatabaseDefaults( database );

    //Creer un dictionnaire pour stocker le reacteur
    AcDbDictionary* pNamedObj;
    AcDbDictionary* pNameList;
    database->getNamedObjectsDictionary( pNamedObj, AcDb::kForWrite );
    AcString nameDict = _T( "ASDK_DICT" ) ;

    if( pNamedObj->getAt( nameDict, ( AcDbObject*& )pNameList, AcDb::kForWrite ) == Acad::eKeyNotFound )
    {
        pNameList = new AcDbDictionary;
        AcDbObjectId dictId;
        pNamedObj->setAt( nameDict, pNameList, dictId );
    }

    pNamedObj->close();

    //Creer l'objectReactor pour l'objet 1
    ObjectReactor* objReact = new ObjectReactor();
    objReact->eLinkage( objectId2, isErased );
    AcDbObjectId objId;
    AcString objectName = _T( "object_to_notify_A" );

    if( ( pNameList->getAt( objectName, objId ) ) == Acad::eKeyNotFound )
    {
        pNameList->setAt( objectName, objReact, objId );
        objReact->close();
    }

    else
    {
        delete objReact;
        acutPrintf( _T( "object_to_notify_A already exists\n" ) );
    }

    //Parametrer le reacteur persistant entre l'objet1 et l'ObjectReactor
    pLineA->addPersistentReactor( objId );
    objectName = _T( "object_to_notify_B" );

    //Creer l'objectReactor pour l'objet 2
    objReact = new ObjectReactor();
    objReact->eLinkage( objectId1, isErased );

    if( ( pNameList->getAt( objectName, objId ) ) == Acad::eKeyNotFound )
    {
        pNameList->setAt( objectName, objReact, objId );
        objReact->close();
    }

    else
    {
        delete objReact;
        acutPrintf( _T( "object_to_notify_B already exists\n" ) );
    }

    //Parametrer le reacteur persistant entre l'objet2 et l'ObjectReactor
    pLineB->addPersistentReactor( objId );
    pLineB->close();

}


/*
void ObjectReactor::assocObject( const bool& isErased, AcDbObject* objectNotifier, AcDbObject* objectReact )
{
    //On recupère la base de données
    AcDbDatabase* database =  acdbHostApplicationServices()->workingDatabase();

    objectNotifier->close();
    objectReact->close();

    //On crée un dictionnaire pour stocker l'objet reacteur
    AcDbDictionary* nameObject;
    AcDbDictionary* nameList;
    database->getNamedObjectsDictionary( nameObject,  AcDb::kForWrite );
    AcString nameDict = _T( "ASDK_DICT" ) ;

    if( nameObject->getAt( nameDict, ( AcDbObject*& )nameList, AcDb::kForWrite ) == Acad::eKeyNotFound )
    {
        nameList = new AcDbDictionary();
        AcDbObjectId dictId;
        nameObject->setAt( nameDict, nameList, dictId );
    }

    nameObject->close();

    //On recupère les id des deux objets
    AcDbObjectId idObjectNotifier =  objectNotifier->objectId();
    AcDbObjectId idObjectToReact =  objectReact->objectId();

    //On lie la pièce avec le bloc
    this->eLinkage( idObjectToReact, true );

    //On enregistre le reacteur dans le dictionnaire
    AcDbObjectId objectId;
    AcString objectName = _T( "object_to_notify_" ) + numberToAcString( mNbObjectReactor );

    while( ( nameList->getAt( objectName, objectId ) ) != Acad::eKeyNotFound )
    {
        mNbObjectReactor++;
        //On enregistre le reacteur dans le dictionnaire
        objectName = _T( "object_to_notify_" ) + numberToAcString( mNbObjectReactor );
    }

    // if( ( nameList->getAt( objectName, objectId ) ) == Acad::eKeyNotFound )
    //{
    Acad::ErrorStatus gfuck = nameList->setAt( objectName, this, objectId );

    this->close();

    //}
    //else
    //{
    // delete this;
    // acutPrintf( _T( "%s already exists\n" ), objectName );
    //}

    objectNotifier->addPersistentReactor( objectId );
}*/