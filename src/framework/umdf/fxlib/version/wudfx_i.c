//
// Copyright (c) Microsoft. All rights reserved.
//





#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

// from public/sdk/inc/Unknwn.h
MIDL_DEFINE_GUID(IID, IID_IUnknown,00000000,0000,0000,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

// autogenerated
MIDL_DEFINE_GUID(IID, IID_IWudfIrp,0x2fe7ac27,0x2bb2,0x420d,0x8e,0x38,0x6b,0x1a,0x8c,0x2e,0xb1,0xd7);
MIDL_DEFINE_GUID(IID, IID_IWudfPnpIrp,0x52AE00B3,0x4067,0x40cb,0xA6,0x79,0xF1,0x81,0x99,0x92,0x1B,0xF7);
MIDL_DEFINE_GUID(IID, IID_IWudfIoIrp,0xa04b21cf,0x8a23,0x45a9,0x9b,0xf4,0x28,0xec,0x6c,0x3a,0x2a,0x1e);
MIDL_DEFINE_GUID(IID, IID_IWudfIoIrp2,0xe07872c5,0xf8ac,0x4abe,0x90,0x49,0xa5,0x7b,0x06,0x54,0x97,0x2e);
MIDL_DEFINE_GUID(IID, IID_IWudfHost,0x2b3a5fa1,0x727f,0x4d7e,0x89,0x56,0x68,0x32,0xcf,0x3f,0x74,0x45);
MIDL_DEFINE_GUID(IID, IID_IWudfHost2,0xe52d7af5,0x7598,0x4308,0x98,0x5d,0x06,0x15,0x3f,0xc4,0x67,0x30);
MIDL_DEFINE_GUID(IID, IID_IWudfDevice,0xA654DA03,0x79D2,0x4600,0xBF,0xD0,0xF0,0x4D,0x50,0x69,0x38,0x17);
MIDL_DEFINE_GUID(IID, IID_IWudfDevice2,0xF23B8610,0xBF55,0x42e2,0x85,0x74,0xC9,0x4E,0x0C,0x4B,0x7B,0x2F);
MIDL_DEFINE_GUID(IID, IID_IWudfRemoteDispatcher,0xa3d0c16e,0xab0b,0x4da5,0x91,0xb2,0xfd,0x0a,0x3f,0x85,0xc4,0xc1);
MIDL_DEFINE_GUID(IID, IID_IWudfTargetCallbackDeviceChange,0xA7BB7B10,0xBA8F,0x4B32,0x8A,0x61,0x93,0x01,0x2C,0xDD,0xDD,0x09);
MIDL_DEFINE_GUID(IID, IID_IWudfIoDispatcher,0xa635b3c3,0xc9b8,0x4fe1,0x82,0x9a,0xaf,0x50,0x11,0xab,0x77,0xda);
MIDL_DEFINE_GUID(IID, IID_IFxMessageDispatch,0x5A84C307,0x3EAA,0x4413,0xA5,0xDB,0x72,0xC8,0xBB,0x9C,0x65,0x86);
MIDL_DEFINE_GUID(IID, IID_IFxMessageDispatch2,0x3b183190,0xd4a2,0x4f96,0x93,0x2d,0xe9,0xc1,0x8e,0x9a,0x52,0x22);
MIDL_DEFINE_GUID(IID, IID_IWudfDeviceStack2,0xf71f77d3,0x9b3c,0x4037,0xa1,0x0f,0x44,0xa4,0xa2,0x2e,0x4d,0x79);
MIDL_DEFINE_GUID(IID, IID_IWudfFile2,0xc2312ef7,0xfa4a,0x4679,0x94,0x70,0xaf,0xb8,0xe2,0xe0,0xef,0x85);
