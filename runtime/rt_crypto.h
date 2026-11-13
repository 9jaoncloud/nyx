#ifndef RT_CRYPTO_H
#define RT_CRYPTO_H
#include <stdint.h>

void nyx_sha256(const uint8_t* data, uint64_t len, uint8_t* output);
void nyx_sha512(const uint8_t* data, uint64_t len, uint8_t* output);
void nyx_md5(const uint8_t* data, uint64_t len, uint8_t* output);
void nyx_hmac_sha256(const uint8_t* key, uint64_t key_len, const uint8_t* data, uint64_t data_len, uint8_t* output);
void nyx_random_bytes(uint8_t* buf, uint64_t len);
int64_t nyx_aes_encrypt(const uint8_t* key, uint64_t key_len, const uint8_t* data, uint64_t data_len, uint8_t* output);
int64_t nyx_aes_decrypt(const uint8_t* key, uint64_t key_len, const uint8_t* data, uint64_t data_len, uint8_t* output);

#endif

