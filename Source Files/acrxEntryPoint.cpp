#include "acrxEntryPoint.h"

void initApp()
{
    // Convertir en lignes
    initCmd( "POLY2DTOLINE", cmdPoly2DToLine );
    initCmd( "POLY3DTOLINE", cmdPoly3DToLine );
    initCmd( "ARCTOLINE", cmdArcToLine );
    initCmd( "CIRCLETOLINE", cmdCircleToLine );
    initCmd( "SPLINETOLINE", cmdSplineToLine );
    initCmd( "FACETOLINE", cmdFaceToLine );
    initCmd( "ELLIPSETOLINE", cmdEllipseToLine );
    initCmd( "XXXTOPOLYLINE2D", cmdXxxToPolyline2D );
    
    // Convertir en polylignes 2D
    initCmd( "LINETOPOLY2D", cmdLineToPoly2D );
    initCmd( "LINETO3DPOLY2D", cmdLineTo3DPoly2D );
    initCmd( "POLY2DTOPOLY2DSEG", cmdPoly2DToPoly2DSeg );
    initCmd( "POLY3DTOPOLY2D", cmdPoly3DToPoly2D );
    initCmd( "ARCTOPOLY2D", cmdArcTo2DPoly2D );
    initCmd( "ARCTO3DPOLY2D", cmdArcTo3DPoly2D );
    initCmd( "ARCTOPOLY2DSEG", cmdArcToPoly2DSeg );
    initCmd( "CIRCLETOPOLY2D", cmdCircleToPoly2D );
    initCmd( "CIRCLETO2DPOLYSEG", cmdCircleToPoly2DSeg );
    initCmd( "SPLINETOPOLY2D", cmdSplineToPoly2D );
    initCmd( "FACETOPOLY2D", cmdFaceToPoly2D );
    initCmd( "FACETO3DPOLY2D", cmdFaceTo3DPoly2D );
    initCmd( "ELLIPSETOPOLY2D", cmdEllipseToPoly2D );
    initCmd( "POLY3DTOPOLY2DARC", cmdConvertPoly3DToPoly2DArc );
    initCmd( "INTERSECTPOLY2DLINE", cmdIntersectPoly2DLine );
    
    // Convertir en polylignes 3D
    initCmd( "LINETOPOLY3D", cmdLineToPoly3D );
    initCmd( "POLY2DTOPOLY3D", cmdPoly2DToPoly3D );
    initCmd( "ARCTOPOLY3D", cmdArcToPoly3D );
    initCmd( "CIRCLETOPOLY3D", cmdCircleToPoly3D );
    initCmd( "SPLINETOPOLY3D", cmdSplineToPoly3D );
    initCmd( "FACETOPOLY3D", cmdFaceToPoly3D );
    initCmd( "ELLIPSETOPOLY3D", cmdEllipseToPoly3D );
    initCmd( "INTERSECTPOLY3D", cmdIntersectPoly3D );
    initCmd( "INTERSECTPOLY3DLINE", cmdIntersectPoly3DLine );
    initCmd( "SUPERPOSEPOLY", cmdSuperposePoly );
    
    // Convertir en arcs
    initCmd( "CIRCLETOARC", cmdCircleToArc );
    initCmd( "ELLIPSETOARC", cmdEllipseToArc );
    
    // Convertir en faces
    initCmd( "POLY2DTOFACE", cmdPoly2DToFace3D );
    initCmd( "POLY3DTOFACE3D", cmdPoly3DToFace3D );
    
    // Convertir en XML
    initCmd( "FACETOXML", cmdFaceToXml );
    
    // Demandes particulières
    initCmd( "CREATECUBE", cmdCreateCube );
    initCmd( "ADDAMOR", cmdAddBlocAmor );
    initCmd( "CLEANPTTOPO", cmdCleanTopo );
    initCmd( "CLEANDOUBLONSP", cmdCleanDoublonSP );
    initCmd( "CGPTTOPO", cmdChangePtTopoWithAtt );
    
    // Nettoyage
    initCmd( "NETPOLY2D", cmdNetPoly2D );
    initCmd( "COLIPOLY2D", cmdColiPoly2D );
    initCmd( "NETCOLIPOLY2D", cmdNetColiPoly2D );
    initCmd( "NETPOLY3D", cmdNetPoly3D );
    initCmd( "COLIPOLY3D", cmdColiPoly3D );
    initCmd( "NETCOLIPOLY3D", cmdNetColiPoly3D );
    initCmd( "NETPOLY", cmdNetPoly );
    initCmd( "COLIPOLY", cmdColiPoly );
    initCmd( "NETCOLIPOLY", cmdNetColiPoly );
    initCmd( "ABYNETPOLY", cmdAbyNetPoly );
    initCmd( "ABYCOLIPOLY", cmdAbyColiPoly );
    
    // Dev
    initCmd( "TESTCONVERT", test );
    
    // Répartir en plusieurs fichiers
    initCmd( "TILEALL", cmdTileAll );
    initCmd( "TILEPOLY", cmdTilePoly );
    initCmd( "TILECURVE", cmdTileCurve );
    initCmd( "TILELINE", cmdTileLine );
    initCmd( "TILEBLOCK", cmdTileBlock );
    initCmd( "TILEPOINT", cmdTilePoint );
    initCmd( "TILETEXT", cmdTileText );
    initCmd( "TILEHATCH", cmdTileHatch );
}


void initCmd( AcString sCmdName, GcRxFunctionPtr FunctionAddr )
{
    //Declarer le global name
    AcString sGlobalName = sGroupName + _T( "_" ) + sCmdName;
    
    //Creer le nom
    acedRegCmds->addCommand(
        sGroupName,
        sGlobalName,
        sCmdName,
        ACRX_CMD_MODAL + ACRX_CMD_USEPICKSET + ACRX_CMD_REDRAW,
        FunctionAddr );
}


void unloadApp()
{
    acedRegCmds->removeGroup( sGroupName );
}


extern "C" AcRx::AppRetCode
acrxEntryPoint( AcRx::AppMsgCode msg, void* appId )
{
    switch( msg )
    {
    
        case AcRx::kInitAppMsg:
            acrxDynamicLinker->unlockApplication( appId );
            acrxDynamicLinker->registerAppMDIAware( appId );
            
            initApp();
            
            acrxBuildClassHierarchy();
            
            break;
            
        case AcRx::kUnloadAppMsg:
            unloadApp();
            
            break;
    }
    
    return AcRx::kRetOK;
}