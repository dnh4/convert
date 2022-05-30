#include "helperFunctions.h"
#include "blockEntity.h"

#include <boost\algorithm\string.hpp>
#include <algorithm>

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616


std::pair<bool, int> findInVector( const std::vector<int>& vecOfElements, const int& element )
{
    std::pair<bool, int > result;
    
    // Find given element in vector
    auto it = std::find( vecOfElements.begin(), vecOfElements.end(), element );
    
    if( it != vecOfElements.end() )
    {
        result.second = distance( vecOfElements.begin(), it );
        result.first = true;
    }
    
    else
    {
        result.first = false;
        result.second = -1;
    }
    
    return result;
}

bool findStringInVector( std::vector<string>& vecOfElements, const AcString& element )
{
    bool res = false;
    
    string elt = acStrToStr( element );
    
    vector<string>::iterator  it = vecOfElements.begin();
    
    for( it = vecOfElements.begin(); it != vecOfElements.end(); it ++ )
    {
        string oneElt = ( string ) * it;
        
        if( oneElt.compare( elt ) == 0 )
        {
            res = true;
            break;
        }
        
        else
        {
            oneElt.erase( 0, 1 );
            
            if( oneElt.compare( elt ) == 0 )
            {
                res = true;
                break;
            }
            
            continue;
        }
        
    }
    
    // Find given element in vector
    /*vector<string>::iterator  it = std::find( vecOfElements.begin(), vecOfElements.end(), elt ) ;
    
    if( it != vecOfElements.end())
       res = true;*/
    
    return res;
}

std::vector<string> fmlFileAsVector( const AcString& filePath, const AcString& sep )
{
    std::vector<string> resVec ;
    
    string path = acStrToStr( filePath );
    
    if( !isFileExisting( path ) )
        return resVec;
        
    // 1. récupération du contenu du fichier
    std::ifstream filsStream( path );
    std::stringstream strBuffer;
    
    strBuffer << filsStream.rdbuf();
    string strToSplit = strBuffer.str();
    
    string splitChar = acStrToStr( sep );
    
    // 2. Découpage
    boost::split( resVec, strToSplit, boost::is_any_of( splitChar ) );
    
    // 3. vérification si on a une chaine vide à la fin si oui on supprime
    vector<string>::const_iterator it = resVec.end();
    
    
    
    if( ( *it ).empty() )
        resVec.erase( it );
        
        
    return resVec;
}

int findInVector3D( AcGePoint3dArray& vecOfElements, const AcGePoint3d& element, const double& tol )
{
    int result = -1;
    pair<bool, AcGePoint3d> res;
    AcGePoint3d ptRes = AcGePoint3d( 0, 0, 0 );
    int counter = 0;
    AcGePoint3dArray::iterator  it = vecOfElements.begin();
    
    for( it = vecOfElements.begin(); it != vecOfElements.end(); it++ )
    {
        AcGePoint3d temp = ( AcGePoint3d ) * it;
        
        //Comparaison
        if( isEqual3d( temp, element, tol ) )
        {
            result = counter;
            ptRes = element;
            ptRes.z = temp.z;
            break;
        }
        
        counter++;
        continue;
    }
    
    return result;
}

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const AcString&           value,
    const AcString&           layer,
    const double&           rotation )
{
    //Recupeer l'attribut
    AcDbAttribute* att = getAttributObject( blockRef, attName, AcDb::kForWrite );
    
    //Verifier que l'attribut existe
    if( att )
    {
        double height = att->height();
        
        AcGeMatrix3d trans = blockRef->blockTransform();;
        
        //changer la valeur
        Acad::ErrorStatus es = att->setTextString( value );
        double txtLenght = att->widthFactor();
        es = att->setLayer( layer );
        double rot = rotation;
        
        if( rot >= M_PI )
            rot = rot - M_PI;
            
        if( rotation > M_PI )
        {
            AcGePoint3d pt = att->position();
            
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt = pt.transformBy( trans );
            att->setPosition( pt );
            att->setRotation( rotation + M_PI_2 );
        }
        
        else
        {
            AcGePoint3d pt = att->position();
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt = pt.transformBy( trans );
            att->setPosition( pt );
            att->setRotation( rotation - M_PI_2 );
        }
        
        //Fermer l'attribut
        att->close();
        
        return true;
    }
    
    //Sortir
    return false;
}

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const double&           value,
    const AcString&           layer,
    const double&           rotation )
{
    //Recupeer l'attribut
    AcDbAttribute* att = getAttributObject( blockRef, attName, AcDb::kForWrite );
    
    //Verifier que l'attribut existe
    if( att )
    {
        double height = att->height();
        
        AcGeMatrix3d trans = blockRef->blockTransform();;
        
        AcString strVal = _T( "S:" );
        AcString strTemp;
        strTemp = strTemp.format( _T( "S:%.2f" ), value );
        strVal = strTemp;
        //changer la valeur
        Acad::ErrorStatus es = att->setTextString( strVal );
        double txtLenght = att->widthFactor();
        es = att->setLayer( layer );
        double rot = rotation;
        
        if( rot >= M_PI )
            rot = rot - M_PI;
            
        if( rotation > M_PI )
        {
            AcGePoint3d pt = att->position();
            AcGePoint3d pts = blockRef->position();
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt.y = -height / 2;
            pt = pt.transformBy( trans );
            att->setPosition( pt );
        }
        
        else
        {
            AcGePoint3d pt = att->position();
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt.y = -height / 2;
            pt = pt.transformBy( trans );
            att->setPosition( pt );
        }
        
        //Fermer l'attribut
        att->close();
        
        return true;
    }
    
    //Sortir
    return false;
}

bool setAttributValue(
    AcDbBlockReference*      blockRef,
    const AcString&           attName,
    const double&           value,
    const AcString&           layer,
    const double&           rotation,
    const double&           translation )
{
    //Recupeer l'attribut
    AcDbAttribute* att = getAttributObject( blockRef, attName, AcDb::kForWrite );
    
    //Verifier que l'attribut existe
    if( att )
    {
        double height = att->height();
        
        AcGeMatrix3d trans = blockRef->blockTransform();;
        
        AcString strVal = _T( "S:" );
        AcString strTemp;
        strTemp = strTemp.format( _T( "S:%.2f" ), value );
        strVal = strTemp;
        
        // Calcul du dcalage
        double dist = 0.0;
        int lenghtTxt = strVal.length();
        
        if( translation != 0.0 )
        {
            dist = lenghtTxt * 0.21;
            dist += translation;
        }
        
        //changer la valeur
        Acad::ErrorStatus es = att->setTextString( strVal );
        double txtLenght = att->widthFactor();
        es = att->setLayer( layer );
        double rot = rotation;
        
        if( rot >= M_PI )
            rot = rot - M_PI;
            
        if( rotation > M_PI )
        {
            AcGePoint3d pt = att->position();
            AcGePoint3d pts = blockRef->position();
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt.y = -height / 2;
            pt.x = pt.x - dist;
            pt = pt.transformBy( trans );
            att->setPosition( pt );
        }
        
        else
        {
            AcGePoint3d pt = att->position();
            AcGeMatrix3d transInv = trans.inverse();
            pt = pt.transformBy( transInv );
            pt.y = -height / 2;
            pt.x = pt.x - dist;
            pt = pt.transformBy( trans );
            att->setPosition( pt );
        }
        
        //Fermer l'attribut
        att->close();
        
        return true;
    }
    
    //Sortir
    return false;
}

