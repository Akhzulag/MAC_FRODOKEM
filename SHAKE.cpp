#include "SHAKE.h"
#include <cstdint>
#include <iostream>
#include <openssl/evp.h>
#include <string>

#include "FrodoSupport.h"

int shake(const uint8_t* in, size_t in_size, uint8_t* out, size_t outbytes_size,
          int shake_type)
{
    // Ensure valid parameters
    if (in == NULL || out == NULL || (shake_type != 128 && shake_type != 256))
    {
        fprintf(stderr, "Invalid arguments.\n");
        return -1;
    }

    // Determine the algorithm
    const EVP_MD* md = (shake_type == 128) ? EVP_shake128() : EVP_shake256();
    if (md == NULL)
    {
        fprintf(stderr, "Failed to load SHAKE algorithm.\n");
        return -1;
    }

    // Initialize context
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == NULL)
    {
        fprintf(stderr, "Failed to create context.\n");
        return -1;
    }

    // Perform the hash
    if (EVP_DigestInit_ex(ctx, md, NULL) != 1 ||
        EVP_DigestUpdate(ctx, in, in_size) != 1 ||
        EVP_DigestFinalXOF(ctx, out, outbytes_size) != 1)
    {
        fprintf(stderr, "SHAKE computation failed.\n");
        EVP_MD_CTX_free(ctx);
        return -1;
    }

    // Clean up
    EVP_MD_CTX_free(ctx);
    return 0;
}
