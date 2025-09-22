/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jan 12 22:15:36 2003
 */
/* Compiler settings for E:\Programming\¿•∞‘¿”\StarterComp\StarterComp.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IStarterCtrl = {0xCAFCF4BC,0x6140,0x4C8C,{0x81,0xCA,0x1C,0x5D,0x5B,0xFE,0x86,0x7D}};


const IID LIBID_STARTERCOMPLib = {0x8E363DA0,0x140D,0x4843,{0x90,0x1D,0x1E,0x25,0xAD,0x68,0xE5,0xF1}};


const IID DIID__IStarterCtrlEvents = {0xAD00194D,0x2840,0x452E,{0xBF,0xF6,0xF7,0xB1,0x1C,0x77,0x65,0x84}};


const CLSID CLSID_StarterCtrl = {0xBBE564F6,0x61BD,0x48D9,{0xBE,0xAE,0x97,0x71,0x7A,0x58,0xC2,0x26}};


#ifdef __cplusplus
}
#endif

