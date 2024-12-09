/*
AES functions relevant to the SolarRecorder background program
Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#include "AES.h"

// Decrypt the ciphertext
/*
"key" is the AES-128-CTR cryptographic key, "iv" is the 'Initialization Vector' - acts as the nonce in AES-CTR, 
"cipher" is the encrypted input text, "cipLength" is the ciphertext length, "text" is the plaintext output,
textLength is the length of the plain text output - which should be the same length as the 
ciphertext.

OpenSSL/libcrypto functions will return a 1 or 0 indicating success or failure. All error codes should be checked
*/

// Initializer list/constructor
AESdecrypt::AESdecrypt(const byteVector& key) : key_(key) {}

// decryption function
byteVector AESdecrypt::decrypt(const byteVector& cipher, const byteVector& iv) const
{
    // Pad the initialization vector
    byteVector iv_ = padIV(iv);

    // Create EVP_CIPHER_CTX context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        throw RuntimeError("\033[1;31mFailed to create EVP_CIPHER_CTX!\033[0m"); // Throw an error
    }

    // Set algorithm as AES-128-CTR and initialize
    const EVP_CIPHER* cipherType = EVP_aes_128_ctr(); 
    if (!EVP_DecryptInit_ex(ctx, cipherType, NULL, key_.data(), iv_.data()))
    {
        EVP_CIPHER_CTX_free(ctx); // Free up the memory used
        throw RuntimeError("\033[1;31mInitialization failed! (EVP_DecryptInit_ex)\033[0m"); // Throw an error
    }

    byteVector text(cipher.size());
    int length;
    if (!EVP_DecryptUpdate(ctx, text.data(), &length, cipher.data(), cipher.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("\033[1;31mFailed to update! (EVP_DecryptUpdate)\033[0m");
    }

    int textLength = length;
    if (!EVP_DecryptFinal_ex(ctx, text.data() + length, &length))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("\033[1;31mFailed to finalize decryption! (EVP_DecryptFinal_ex)\033[0m");
    }
    textLength += length;

    text.resize(textLength);
    EVP_CIPHER_CTX_free(ctx);
    return text;
}

byteVector AESdecrypt::padIV(const byteVector& iv) const {
    byteVector paddedIV = iv;
    if (paddedIV.size() < 16) {
        paddedIV.resize(16,0x00); // pad with 0's if shorter than 16 bytes
    } else if (paddedIV.size() > 16) {
        paddedIV.resize(16); // truncate if too long
    }
    return paddedIV;
}