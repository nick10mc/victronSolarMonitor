/*
Header file for AES functions relevant to the SolarRecorder background program
Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#ifndef AES_H
#define AES_H

    // Common Library Includes...
    #include "common.h"

    // Function Declarations
    class AESdecrypt {
        public:
            AESdecrypt(const byteVector& key);
            byteVector decrypt(const byteVector& cipher, const byteVector& iv) const;

        private:
            byteVector key_;
            byteVector iv;
            byteVector padIV(const byteVector& iv) const;
    };

#endif // AES_H