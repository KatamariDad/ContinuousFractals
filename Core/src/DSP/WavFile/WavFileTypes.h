#pragma once


/// sample output type (16-bit for PCM)
typedef short SampleValue;
/// 8-bit data type
typedef char  bsInt8;
/// 16-bit data type 
typedef short bsInt16;
/// 32-bit data type 
typedef int   bsInt32;
/// 8-bit unsigned type
typedef unsigned char  bsUint8;
/// 16-bit unsigned type
typedef unsigned short bsUint16;
/// 32-bit unsigned type
typedef unsigned int   bsUint32;
/// transparent data type
typedef void* Opaque;

// Choose one of these as the oscillator phase accumulator type.
// double gives less noise with slightly more calculation time.
/// Type for a phase accumulator
typedef double PhsAccum;
#define sinv sin
//typedef float PhsAccum;
//#define sinv sinf

/// Type for an amplitude value
typedef float AmpValue;
/// Type for a high precision amplitude value
typedef double AmpValue2;
/// Type for a frequency or time value
typedef float FrqValue;
/// Type for a generic parameter
typedef float ParamValue;



