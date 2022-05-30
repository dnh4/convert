#pragma once
#include "cmdConvert.h"

class AsdkObjectToNotify : public AcDbObject
{
    public:
        ACRX_DECLARE_MEMBERS( AsdkObjectToNotify );
        AsdkObjectToNotify() {};

        void eLinkage( AcDbObjectId i, double f = 1.0 )
        {
			mId = i; mFactor = f;
		};

        void modified( const AcDbObject* );

        Acad::ErrorStatus dwgInFields( AcDbDwgFiler* );
        Acad::ErrorStatus dwgOutFields( AcDbDwgFiler* ) const;

		void assocLines();

    private:
        AcDbObjectId mId;
        double mFactor;
};


/*
//classe pour le reacteur
class ObjectReactor: public AcDbObject {
    public:
        ACRX_DECLARE_MEMBERS( ObjectReactor );
        ObjectReactor()
        {
            mNbObjectReactor = 0;
        };

        void eLinkage( AcDbObjectId id, bool IsTransformed )
        {
            mId = id;
            mIsTransformed = IsTransformed;
        };

        void modified( const AcDbObject* object );

        Acad::ErrorStatus dwgInFields( AcDbDwgFiler* );
        Acad::ErrorStatus dwgOutFields( AcDbDwgFiler* ) const;

        void assocObject( const bool& isErased, AcDbObject* object1, AcDbObject* object2 );

    private:
        AcDbObjectId  mId;
        bool mIsTransformed;
        int mNbObjectReactor;
};*/