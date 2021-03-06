/** @file
 * Copyright (c) 2019-2020 Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    psa_key_type_t          key_type;
    uint8_t                 key_data[32];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    size_t                  iv_size;
    size_t                  expected_iv_length;
    psa_status_t            expected_status;
} test_data;

static const test_data check1[] = {
#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
{"Test psa_cipher_generate_iv 16 Byte AES\n", PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0xD0}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR, 16, 16,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_192
{"Test psa_cipher_generate_iv 24 Byte AES\n", PSA_KEY_TYPE_AES,
{0x24, 0x13, 0x61, 0x47, 0x61, 0xB8, 0xC8, 0xF0, 0xDF, 0xAB, 0x5A, 0x0E, 0x87,
 0x40, 0xAC, 0xA3, 0x90, 0x77, 0x83, 0x52, 0x31, 0x74, 0xF9}, AES_24B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR, 16, 16,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_AES_256
{"Test psa_cipher_generate_iv 32 Byte AES\n", PSA_KEY_TYPE_AES,
{0xEA, 0xD5, 0xE6, 0xC8, 0x51, 0xF9, 0xEC, 0xBB, 0x9B, 0x57, 0x7C, 0xED, 0xD2,
 0x4B, 0x82, 0x84, 0x9F, 0x9F, 0xE6, 0x73, 0x21, 0x3D, 0x1A, 0x05, 0xC9, 0xED,
 0xDF, 0x25, 0x17, 0x68, 0x86, 0xAE}, AES_32B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR, 16, 16,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_generate_iv DES 64 bit key\n", PSA_KEY_TYPE_DES,
 {0x70, 0x24, 0x55, 0x0C, 0x14, 0x9D, 0xED, 0x29},
 DES_8B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 8, 8,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_2KEY
{"Test psa_cipher_generate_iv Triple DES 2-Key\n", PSA_KEY_TYPE_DES,
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
 DES3_2KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 8, 8,
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_DES_3KEY
{"Test psa_cipher_generate_iv Triple DES 3-Key\n", PSA_KEY_TYPE_DES,
{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
 0xF1, 0xE0, 0xD3, 0xC2, 0xB5, 0xA4, 0x97, 0x86,
 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
 DES3_3KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 8, 8,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
{"Test psa_cipher_generate_iv AES - small iv buffer\n", PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0xD0}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR, 8, 16,
 PSA_ERROR_BUFFER_TOO_SMALL
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_generate_iv DES - small iv buffer\n", PSA_KEY_TYPE_DES,
 {0x70, 0x24, 0x55, 0x0C, 0x14, 0x9D, 0xED, 0x29},
 DES_8B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 4, 8,
 PSA_ERROR_BUFFER_TOO_SMALL
},
#endif
#endif

#ifdef ARCH_TEST_CIPER_MODE_CTR
#ifdef ARCH_TEST_AES_128
{"Test psa_cipher_generate_iv AES - large iv buffer\n", PSA_KEY_TYPE_AES,
{0x49, 0x8E, 0xC7, 0x7D, 0x01, 0x95, 0x0D, 0x94, 0x2C, 0x16, 0xA5, 0x3E, 0x99,
 0x5F, 0xC9, 0xD0}, AES_16B_KEY_SIZE,
 PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CTR, 32, 16,
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_CBC_NO_PADDING
#ifdef ARCH_TEST_DES_1KEY
{"Test psa_cipher_generate_iv DES - large iv buffer\n", PSA_KEY_TYPE_DES,
 {0x70, 0x24, 0x55, 0x0C, 0x14, 0x9D, 0xED, 0x29},
 DES_8B_KEY_SIZE, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_CBC_NO_PADDING,
 16, 8,
 PSA_SUCCESS
},
#endif
#endif
};
