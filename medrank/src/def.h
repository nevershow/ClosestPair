#ifndef __DEF_H
#define __DEF_H

// -----------------------------------------------------------------------------
//  Typedefs
// -----------------------------------------------------------------------------
typedef char Block[];

// -----------------------------------------------------------------------------
//  Macros
// -----------------------------------------------------------------------------

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define PI 3.14159265358979323846
#define MINFREQ 0.5
#define M 50
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
// size of type <int>
const int   SIZEINT = (int) sizeof(int);
// size of type <char>
const int   SIZECHAR = (int) sizeof(char);
// size of type <float>
const int   SIZEDOUBLE = (int) sizeof(double);

const int   SIZEUNSIGNEDSHORT = (int)sizeof(unsigned short);

// file header size
const int   BFHEAD_LENGTH = (int)(sizeof(int) * 2);

#endif
