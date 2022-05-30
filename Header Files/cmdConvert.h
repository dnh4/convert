/*!
 * \file cmdConvert.h
 * \brief Commandes de conversion d'objets AutoCAD
 * \author Dinahasina
 * \date 15 juillet 2019
 */


#pragma once
#include "helperFunctions.h"
#include "convertArc.h"
#include "convertCircle.h"
#include "convertEllipse.h"
#include "convertLine.h"
#include "convertPoly2D.h"
#include "convertPoly3D.h"
#include "convertSpline.h"
#include "convertFace.h"
#include "convertEllipse.h"
#include "boundingBox.h"
#include "properties.h"


/**
  * \fn void cmdXxxToPolyline2D();
  * \cmdName XXXTOPOLYLINE2D
  * \desc Convertit n'importe quel élément sélectionné par l'utilisateur en plusieurs lignes.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les formes telles que les cercles et les arcs.
  * \bullet L'utilisateur a le choix de supprimer l'élément à convertir ou non.
  * \section Convertir en lignes
  * \end
  */
void cmdXxxToPolyline2D();

/**
  * \fn void cmdCreateCube();
  * \cmdName CREATECUBE
  * \desc Crée un cube dont le centre est un point.
  * \setion Demandes particulières
  * \end
  */
void cmdCreateCube();

/**
  * \fn void cmdNetPoly3D();
  * \cmdName NETPOLY3D
  * \desc Nettoie une polyligne 3D.
  * \bullet Remarque : La commande supprime les sommets de la polyligne qui se superposent.
  * \section Nettoyage
  * \end
  */
void cmdNetPoly3D();

/**
  * \fn void cmdColiPoly3D();
  * \cmdName COLIPOLY3D
  * \desc Supprime les sommets colinéaires qui se suivent pour les polylignes 3D.
  * \bullet La commande supprime les sommets colinéaires.
  * \bullet La commande demande une valeur de tolérance pour calculer la colinéarité.
  * \bullet La tolérance definit la distance de colinéarité des sommets.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_AVANT.png
  * \captionimg1 AVANT COLIPOLY3D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_APRES.png
  * \captionimg2 APRES COLIPOLY3D
  * \section Nettoyage
  * \end
  */
void cmdColiPoly3D();

/**
  * \fn void cmdColiPoly3D();
  * \cmdName NETCOLIPOLY3D
  * \desc Nettoie une polyligne 3D et supprime les sommets colinéaires qui se suivent pour les polylignes 3D.
  * \free Remarque : voir NETPOLY3D et COLIPOLY3D.
  * \section Nettoyage
  * \end
  */
void cmdNetColiPoly3D();

/**
  * \fn void cmdNetPoly2D();
  * \cmdName NETPOLY2D
  * \desc Nettoie une polyligne 2D.
  * \bullet La commande supprime les sommets de la polyligne qui se superposent.
  * \section Nettoyage
  * \end
  */
void cmdNetPoly2D();

/**
  * \fn void cmdColiPoly2D();
  * \cmdName COLIPOLY2D
  * \desc Suprime les sommets colinéaires qui se suivent pour les polylignes 2D.
  * \bullet La commande supprime les sommets colinéaires avec une tolérance entrée par l'utilisateur.
  * \bullet La tolérance definit la distance de colinéarité des sommets.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_AVANT.png
  * \captionimg1 AVANT COLIPOLY2D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_APRES.png
  * \captionimg2 APRES COLIPOLY2D
  * \section Nettoyage
  * \end
  */
void cmdColiPoly2D();

/**
  * \fn void cmdNetColiPoly2D();
  * \cmdName NETCOLIPOLY2D
  * \desc Nettoie une polyligne 2D et supprime les sommets colinéaires qui se suivent pour les polylignes 2D.
  * \free Remarque : voir NETPOLY3D et COLIPOLY2D.
  * \imgPath C:/Futurmap/Outils/GStarCAD2020/HTM/images/DEFAULT.gif
  * \section Nettoyage
  * \end
  */
void cmdNetColiPoly2D();

/**
  * \fn void cmdNetPoly();
  * \cmdName NETPOLY
  * \desc Nettoie une polyligne 2D ou une polyligne 3D.
  * \free La commande supprime les sommets de la polyligne qui se superposent.
  * \section Nettoyage
  * \end
  */
void cmdNetPoly();

/**
  * \fn void cmdColiPoly();
  * \cmdName COLIPOLY
  * \desc Supprime les sommets colinéaires qui se suivent pour les polylignes 2D et les polylignes 3D.
  * \bullet La commande supprime les sommets colinéaires avec une tolérance entrée par l'utilisateur.
  * \bullet La tolérance definit la distance de colinéarité des sommets.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_AVANT.png
  * \captionimg1 AVANT COLIPOLY
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/COLIPOLY3D_APRES.png
  * \captionimg2 APRES COLIPOLY
  * \section Nettoyage
  * \end
  */
void cmdColiPoly();

/**
  * \fn void cmdNetColiPoly();
  * \cmdName NETCOLIPOLY
  * \desc Nettoie une polyligne 2D ou une polyligne 3D et supprime les sommets colinéaires qui se suivent pour les polylignes 2D et les polylignes 3D.
  * \free Voir NETPOLY et COLIPOLY.
  * \section Nettoyage
  * \end
  */
void cmdNetColiPoly();

/**
  * \fn void cmdLineToPoly2D();
  * \cmdName LINETOPOLY2D
  * \desc Convertit une ligne en polyligne 2D.
  * \free L'utilisateur a le choix de supprimer la ligne à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdLineToPoly2D();

/**
  * \fn void cmdLineTo3DPoly2D();
  * \cmdName LINETO3DPOLY2D
  * \desc Convertit une sélection de lignes en polylignes 2D en conservant leur plan, autrement dit en conservant le z des lignes.
  * \free L'utilisateur a le choix de supprimer la ligne à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdLineTo3DPoly2D();

/**
  * \fn void cmdLineToPoly3D();
  * \cmdName LINETOPOLY3D
  * \desc Convertit une ligne en polyligne 3D.
  * \free L'utilisateur a le choix de supprimer la ligne à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdLineToPoly3D();

/**
  * \fn void cmdPoly2DToLine();
  * \cmdName POLY2DTOLINE
  * \desc Convertit une polyligne 2D en lignes.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en lignes
  * \end
  */
void cmdPoly2DToLine();

/**
  * \fn void cmdPoly2DToPoly3D();
  * \cmdName POLY2DTOPOLY3D
  * \desc Convertit une polyligne 2D en polyligne 3D.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdPoly2DToPoly3D();

/**
  * \fn void cmdPoly2DToFace3D();
  * \cmdName POLY2DTOFACE
  * \desc Convertit une polyligne 2D en face 3D.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en faces
  * \end
  */
void cmdPoly2DToFace3D();

/**
  * \fn void cmdPoly2DToPoly2DSeg();
  * \cmdName POLY2DTOPOLY2DSEG
  * \desc Convertit une sélection de polylignes 2D avec arcs en polyligne 2D sans arc.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdPoly2DToPoly2DSeg();

/**
  * \fn void cmdPoly2DToPoly2DSeg();
  * \cmdName POLY3DTOLINE
  * \desc Convertit une sélection de polylignes 3D en lignes.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en lignes
  * \end
  */
void cmdPoly3DToLine();

/**
  * \fn void cmdPoly3DToPoly2D();
  * \cmdName POLY3DTOPOLY2D
  * \desc Convertit une sélection de polylignes 3D en polylignes 2D.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdPoly3DToPoly2D();

/**
  * \fn void cmdPoly3DToFace3D();
  * \cmdName POLY3DTOFACE3D
  * \desc Convertit une sélection de polylignes 3D en faces 3D.
  * \free L'utilisateur a le choix de supprimer la polyligne à convertir ou non.
  * \section Convertir en faces
  * \end
  */
void cmdPoly3DToFace3D();

/**
  * \fn void cmdArcToLine();
  * \cmdName ARCTOLINE
  * \desc Convertit une sélection d'arcs en lignes.
  * \free La commande demande une valeur de discrétisation pour discrétiser l'arc.
  * \free L'utilisateur a le choix de supprimer les arcs à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ARCTOLINE_AVANT.png
  * \captionimg1 AVANT ARCTOLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ARCTOLINE_APRES.png
  * \captionimg2 APRES ARCTOLINE
  * \section Convertir en lignes
  * \end
  */
void cmdArcToLine();

/**
  * \fn void cmdArcTo2DPoly2D();
  * \cmdName ARCTOPOLY2D
  * \desc Comnvertit une sélection d'arcs en polylignes 2D.
  * \free La commande demande une valeur de discrétisation pour discrétiser l'arc.
  * \free L'utilisateur a le choix de supprimer les arcs à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ARCTOLINE_AVANT.png
  * \captionimg1 AVANT ARCTOPOLY2D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ARCTOLINE_APRES.png
  * \captionimg2 APRES ARCTOPOLY2D
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdArcTo2DPoly2D();

/**
  * \fn void cmdArcTo2DPoly2D();
  * \cmdName ARCTOPOLY2DSEG
  * \desc Convertit une sélection de polylignes contenant des arcs en segments de polylignes 2D.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les arcs.
  * \bullet L'utilisateur a le choix de supprimer les arcs à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdArcToPoly2DSeg();

/**
  * \fn void cmdArcTo3DPoly2D();
  * \cmdName ARCTO3DPOLY2D
  * \desc Convertit une sélection d'arcs en 3 dimensions en polylignes 2D.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les arcs.
  * \bullet L'utilisateur a le choix de supprimer les arcs à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdArcTo3DPoly2D();

/**
  * \fn void cmdArcToPoly3D();
  * \cmdName ARCTOPOLY3D
  * \desc Convertit une sélection d'arcs en polylignes 3D.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les arcs.
  * \bullet L'utilisateur a le choix de supprimer les arcs à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdArcToPoly3D();

/**
  * \fn void cmdCircleToLine();
  * \cmdName CIRCLETOLINE
  * \desc Convertit une sélection de cercles en lignes.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les arcs.
  * \bullet L'utilisateur a le choix de supprimer les cercles à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_AVANT.png
  * \captionimg1 AVANT CIRCLETOLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_APRES.png
  * \captionimg2 APRES CIRCLETOLINE
  * \section Convertir en lignes
  * \end
  */
void cmdCircleToLine();

/**
  * \fn void cmdCircleToPoly2D();
  * \cmdName CIRCLETOPOLY2D
  * \desc Convertit une sélection de cercles en polylignes 2D.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les cercles.
  * \bullet L'utilisateur a le choix de supprimer les cercles à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_AVANT.png
  * \captionimg1 AVANT CIRCLETOPOLY2D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_APRES.png
  * \captionimg2 APRES CIRCLETOPOLY2D
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdCircleToPoly2D();

/**
  * \fn void cmdCircleToPoly3D();
  * \cmdName CIRCLETOPOLY3D
  * \desc Convertit une sélection de cercles en polylignes 3D.
  * \bullet La commande demande une valeur de discrétisation pour discrétiser les cercles.
  * \bullet L'utilisateur a le choix de supprimer les cercles à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_AVANT.png
  * \captionimg1 AVANT CIRCLETOPOLY3D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOLINE_APRES.png
  * \captionimg2 APRES CIRCLETOPOLY3D
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdCircleToPoly3D();

/**
  * \fn void cmdCircleToPoly3D();
  * \cmdName CIRCLETOARC
  * \desc Convertit une sélection de cercles en arcs.
  * \bullet La commande demande le nombre d'arcs à utiliser pour décomposer le cercle.
  * \bullet L'utilisateur a le choix de supprimer les cercles à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOARC_AVANT.png
  * \captionimg1 AVANT CIRCLETOARC
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/CIRCLETOARC_APRES.png
  * \captionimg2 APRES CIRCLETOARC
  * \section Convertir en arcs
  * \end
  */
void cmdCircleToArc();

/**
  * \fn void cmdSplineToLine();
  * \cmdName SPLINETOLINE
  * \desc Convertit une sélection de splines en lignes.
  * \bullet L'utilisateur a le choix de supprimer les splines à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/SPLINETOLINE_AVANT.png
  * \captionimg1 AVANT SPLINETOLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/SPLINETOLINE_APRES.png
  * \captionimg2 APRES SPLINETOLINE
  * \section Convertir en lignes
  * \end
  */
void cmdSplineToLine();

/**
  * \fn void cmdSplineToPoly2D();
  * \cmdName SPLINETOPOLY2D
  * \desc Convertit une sélection de splines en polylignes 2D.
  * \bullet L'utilisateur a le choix de supprimer les splines à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdSplineToPoly2D();

/**
  * \fn void cmdSplineToPoly3D();
  * \cmdName SPLINETOPOLY3D
  * \desc Convertit une sélection de splines en polylignes 3D.
  * \bullet L'utilisateur a le choix de supprimer les splines à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdSplineToPoly3D();

/**
  * \fn void cmdFaceToXml();
  * \cmdName FACETOXML
  * \desc Exporte un fichier xml à partir d'une sélection de faces.
  * \section Convertir en XML
  * \end
  */
void cmdFaceToXml();

/**
  * \fn void cmdFaceToLine();
  * \cmdName FACETOLINE
  * \desc Convertit une sélection de faces en lignes.
  * \bullet L'utilisateur a le choix de supprimer les faces à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/FACETOLINE_AVANT.png
  * \captionimg1 AVANT FACETOLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/FACETOLINE_APRES.png
  * \captionimg2 APRES FACETOLINE
  * \section Convertir en lignes
  * \end
  */
void cmdFaceToLine();

/**
  * \fn void cmdFaceToPoly2D();
  * \cmdName FACETOPOLY2D
  * \desc Convertit une sélection de faces en polylignes 2D.
  * \bullet L'utilisateur a le choix de supprimer les faces à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdFaceToPoly2D();

/**
  * \fn void cmdFaceTo3DPoly2D();
  * \cmdName FACETO3DPOLY2D
  * \desc Convertit une face en polyligne 2D tout en conservant le plan 3D dans lequel se trouve la face.
  * \bullet L'utilisateur a le choix de supprimer les faces à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdFaceTo3DPoly2D();

/**
  * \fn void cmdFaceToPoly3D();
  * \cmdName FACETOPOLY3D
  * \desc Convertit une face en polyligne 3D.
  * \bullet L'utilisateur a le choix de supprimer les faces à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdFaceToPoly3D();

/**
  * \fn void cmdEllipseToLine();
  * \cmdName ELLIPSETOLINE
  * \desc Convertit une ellipse en lignes.
  * \bullet L'utilisateur a le choix de supprimer l'ellipse à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ELLIPSETOLINE_AVANT.png
  * \captionimg1 AVANT ELLIPSETOLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ELLIPSETOLINE_APRES.png
  * \captionimg2 APRES ELLIPSETOLINE
  * \section Convertir en lignes
  * \end
  */
void cmdEllipseToLine();

/**
  * \fn void cmdEllipseToPoly2D();
  * \cmdName ELLIPSETOPOLY2D
  * \desc Convertit une ellipse en polyligne 2D.
  * \bullet L'utilisateur a le choix de supprimer l'ellipse à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdEllipseToPoly2D();

/**
  * \fn void cmdEllipseToArc();
  * \cmdName ELLIPSETOARC
  * \desc Convertit une ellipse en arcs.
  * \bullet L'utilisateur a le choix de supprimer l'ellipse à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ELLIPSETOARC_AVANT.png
  * \captionimg1 AVANT ELLIPSETOARC
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/ELLIPSETOARC_APRES.png
  * \captionimg2 APRES ELLIPSETOARC
  * \section Convertir en arcs
  * \end
  */
void cmdEllipseToArc();

/**
  * \fn void cmdEllipseToPoly3D();
  * \cmdName ELLIPSETOPOLY3D
  * \desc Comnvertit une ellipse en polyligne 3D.
  * \bullet L'utilisateur a le choix de supprimer l'ellipse à convertir ou non.
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdEllipseToPoly3D();

/**
  * \fn void cmdIntersectPoly3D();
  * \cmdName INTERSECTPOLY3D
  * \desc Ajoute des sommets sur les intersections d'une sélection de polylignes 3D.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg1 AVANT INTERSECTPOLY3D
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg2 APRES INTERSECTPOLY3D
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdIntersectPoly3D();

/**
  * \fn void cmdIntersectPoly3DLine();
  * \cmdName INTERSECTPOLY3DLINE
  * \desc Calcule les intersections entre des polylignes 3D et des lignes sélectionnées avec un filtre en distance.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg1 AVANT INTERSECTPOLY3DLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg2 APRES INTERSECTPOLY3DLINE
  * \section Convertir en lignes
  * \end
  */
void cmdIntersectPoly3DLine();

/**
  * \fn void cmdCircleToPoly2DSeg();
  * \cmdName CIRCLETO2DPOLYSEG
  * \desc Convertir une sélection de cercles en polylignes 2D sans arc.
  * \bullet L'utilisateur a le choix de supprimer les cercles à convertir ou non.
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdCircleToPoly2DSeg();

/**
  * \fn void cmdConvertPoly3DToPoly2DArc();
  * \cmdName POLY3DTOPOLY2DARC
  * \desc Convertit une sélection de polylignes 3D en polylignes 2D avec arcs.
  * \sous_desc L'utilisateur a le choix de supprimer les polylignes 3D à convertir ou non.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/POLY3DTOPOLY2DARC_AVANT.png
  * \captionimg1 AVANT POLY3DTOPOLY2DARC
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/POLY3DTOPOLY2DARC_AVANT.png
  * \captionimg2 APRES POLY3DTOPOLY2DARC
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdConvertPoly3DToPoly2DArc();

/**
  * \fn void test();
  * \cmdName TESTCONVERT
  * \desc Commande de test pour les développeurs.
  * \section Dev
  * \end
  */
void test();


/**
  * \fn void cmdIntersectPoly2DLine();
  * \cmdName INTERSECTPOLY2DLINE
  * \desc Calcule les intersections entre une polyligne 2D et une sélection de lignes et ajoute ces intersections sur la polyligne 2D.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg1 AVANT INTERSECTPOLY2DLINE
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/INTERSECTPOLY3D_AVANT.png
  * \captionimg2 APRES INTERSECTPOLY2DLINE
  * \section Convertir en polylignes 2D
  * \end
  */
void cmdIntersectPoly2DLine();


/**
  * \fn void cmdTileAll();
  * \cmdName TILEALL
  * \desc Répartit un fichier en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileAll();


/**
  * \fn void cmdTilePoly();
  * \cmdName TILEPOLYLINE
  * \desc Répartit les polylignes 3D en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTilePoly();


/**
  * \fn void cmdTileCurve();
  * \cmdName TILECURVE
  * \desc Répartit les courbes en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileCurve();

/**
  * \fn void cmdTileLine();
  * \cmdName TILELINE
  * \desc Répartit les lignes en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileLine();

/**
  * \fn void cmdTileBlock();
  * \cmdName TILEBLOCK
  * \desc Répartit les en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileBlock();

/**
  * \fn void cmdTileHatch();
  * \cmdName TILEHATCH
  * \desc Répartit les hachures en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileHatch();

/**
  * \fn void void cmdTilePoint();
  * \cmdName TILEPOINT
  * \desc Répartit les points en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTilePoint();

/**
  * \fn void cmdTileText();
  * \cmdName TILETEXT
  * \desc Répartit les textes en plusieurs fichiers.
  * \bullet La séparation est faite en fonction d'une sélection de polylignes 2D pour lesquelles on renseigne un nom de calque.
  * \section Répartir en plusieurs fichiers
  * \end
  */
void cmdTileText();

/**
  * \fn void cmdSuperposePoly();
  * \cmdName SUPERPOSEPOLY
  * \descSuperpose une polyligne 3D avec une autre polyligne 3D.
  * \img1 C:/Futurmap/Outils/GStarCAD2020/HTM/images/SUPERPOSEPOLY3D_AVANT.png
  * \captionimg1 AVANT SUPERPOSEPOLY
  * \img2 C:/Futurmap/Outils/GStarCAD2020/HTM/images/SUPERPOSEPOLY3D_AVANT.png
  * \captionimg2 APRES SUPERPOSEPOLY
  * \section Convertir en polylignes 3D
  * \end
  */
void cmdSuperposePoly();

/**
  * \fn void cmdAddBlocAmor();
  * \cmdName ADDAMOR
  * \desc Ajoute un bloc amorce.
  * \section Demandes particulières
  * \end
  */
void cmdAddBlocAmor();

/**
  * \fn void cmdAbyNetPoly();
  * \cmdName ABYNETPOLY
  * \desc Nettoie une polyligne Abyla.
  * \section Nettoyage
  * \end
  */
void cmdAbyNetPoly();

/**
  * \fn void cmdAbyColiPoly();
  * \cmdName ABYCOLIPOLY
  * \desc supprime les sommets colinéaires sur une polyligne Abyla.
  * \section Nettoyage
  * \end
  */
void cmdAbyColiPoly();

/**
  * \fn void cmdFuturmapPoly3dPlus();
  * \cmdName CLEANPTTOPO
  * \icons DC:/Futurmap/Outils/GStarCAD2020/HTM/images/Convert.png
  * \desc Supprime les blocs PTTOPO du calque TOPO_Points_Surplus_Piqué en superposition avec les blocs PTTOPO de calques TOPO_Points_Hors_TN_Piqués et TOPO_Points_Piqués.
  * \imgPath C:/Futurmap/Outils/GStarCAD2020/HTM/images/CLEANPTTOPO.gif
  * \section Demandes particulières
  * \end
  */
void cmdCleanTopo();

/**
  * \fn void cmdFuturmapPoly3dPlus();
  * \cmdName CLEANDOUBLONSP
  * \iconsC:/Futurmap/Outils/GStarCAD2020/HTM/images/Convert.png
  * \desc Supprime les doublons de blocs PTTOPO des calque TOPO_Points_Surplus_Piqué.
  * \imgPath C:/Futurmap/Outils/GStarCAD2020/HTM/images/CLEANDOUBLONSP.gif
  * \section Demandes particulières
  * \end
  */
void cmdCleanDoublonSP();

/**
  * \fn void cmdFuturmapPoly3dPlus();
  * \cmdName CGPTTOPO
  * \icons C:/Futurmap/Outils/GStarCAD2020/HTM/images/Convert.png
  * \desc Change les blocs Fmap_Elevation des soupiraux et seuils en blocs correspondants aux points TOPO de ces derniers.
  * \imgPath C:/Futurmap/Outils/GStarCAD2020/HTM/images/CGPTTOPO.gif
  * \section Demandes particulières
  * \end
  */
void cmdChangePtTopoWithAtt();
