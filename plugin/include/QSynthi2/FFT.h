#ifndef FFT_H
#define FFT_H

#ifdef __GNUC__
#ifndef __clang__
#pragma GCC system_header
#else
#pragma clang system_header
#endif
#endif

#define POCKETFFT_NO_MULTITHREADING
#include "pocketfft_hdronly.h"

#endif //FFT_H
