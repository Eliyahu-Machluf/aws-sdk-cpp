/*
  * Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
  *
  * Licensed under the Apache License, Version 2.0 (the "License").
  * You may not use this file except in compliance with the License.
  * A copy of the License is located at
  *
  *  http://aws.amazon.com/apache2.0
  *
  * or in the "license" file accompanying this file. This file is distributed
  * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
  * express or implied. See the License for the specific language governing
  * permissions and limitations under the License.
  */

#pragma once

#include <aws/core/Core_EXPORTS.h>
#include <aws/core/utils/Array.h>
#include <memory>

namespace Aws
{
    namespace Utils
    {
        namespace Crypto
        {
            class Hash;
            class HMAC;
            class SymmetricCipher;
            class HashFactory;
            class HMACFactory;
            class SymmetricCipherFactory;
            template <typename DataType>
            class SecureRandom;
            template<typename DataType>
            class SecureRandomFactory;

            /**
             * You need to call this before using any of the cryptography libs. Should be called after setting the factories.
             */
            AWS_CORE_API void InitCrypto();
            /**
             * You need to call this upon program shutdown.
             */
            AWS_CORE_API void CleanupCrypto();

            /**
             * Create an MD5 Hash provider
             */
            AWS_CORE_API std::shared_ptr<Hash> CreateMD5Implementation();
            /**
             * Create a Sha256 Hash provider
             */
            AWS_CORE_API std::shared_ptr<Hash> CreateSha256Implementation();
            /**
             * Create a Sha256 HMACHash provider
             */
            AWS_CORE_API std::shared_ptr<HMAC> CreateSha256HMACImplementation();

            /**
             * Create AES in CBC mode off of a 256 bit key. Auto Generates a 16 byte secure random IV
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CBCImplementation(const CryptoBuffer& key);
            /**
             * Create AES in CBC mode off of a 256 bit key and 16 byte IV
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CBCImplementation(const CryptoBuffer& key, const CryptoBuffer& iv);
            /**
             * Create AES in CBC mode off of a 256 bit key and 16 byte IV
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CBCImplementation(CryptoBuffer&& key, CryptoBuffer&& iv);

            /**
             * Create AES in CTR mode off of a 256 bit key. Auto Generates a 16 byte IV in the format
             * [nonce 4bytes ] [securely random iv 8 bytes] [ CTR init 4bytes ]
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CTRImplementation(const CryptoBuffer& key);
            /**
             * Create AES in CTR mode off of a 256 bit key and 16 byte IV
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CTRImplementation(const CryptoBuffer& key, const CryptoBuffer& iv);
            /**
             * Create AES in CTR mode off of a 256 bit key and 16 byte IV
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_CTRImplementation(CryptoBuffer&& key, CryptoBuffer&& iv);

            /**
             * Create AES in GCM mode off of a 256 bit key. Auto Generates a 16 byte secure random IV.
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_GCMImplementation(const CryptoBuffer& key);
            /**
             * Create AES in GCM mode off of a 256 bit key, a 16 byte secure random IV, and an optional 16 byte Tag. If you are using this
             * cipher to decrypt an encrypted payload, you must set the tag here.
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_GCMImplementation(const CryptoBuffer& key, const CryptoBuffer& iv,
                                                                                      const CryptoBuffer& tag = CryptoBuffer(0));
            /**
             * Create AES in GCM mode off of a 256 bit key, a 16 byte secure random IV, and an optional 16 byte Tag. If you are using this
             * cipher to decrypt an encrypted payload, you must set the tag here.
             */
            AWS_CORE_API std::shared_ptr<SymmetricCipher> CreateAES_GCMImplementation(CryptoBuffer&& key, CryptoBuffer&& iv,
                                                                                      CryptoBuffer&& tag = std::move(CryptoBuffer(0)));

            /**
             * Create SecureRandom instance for 64 bit unsigned integer
             */
            AWS_CORE_API std::shared_ptr<SecureRandom<uint64_t>> Create64BitSecureRandomImplementation();

            /**
            * Create SecureRandom instance for 32 bit unsigned integer
            */
            AWS_CORE_API std::shared_ptr<SecureRandom<uint32_t>> Create32BitSecureRandomImplementation();

            /**
             * Set the global factory for MD5 Hash providers
             */
            AWS_CORE_API void SetMD5Factory(const std::shared_ptr<HashFactory>& factory);
            /**
             * Set the global factory for Sha256 Hash providers
             */
            AWS_CORE_API void SetSha256Factory(const std::shared_ptr<HashFactory>& factory);
            /**
             * Set the global factory for Sha256 HMAC Hash providers
             */
            AWS_CORE_API void SetSha256HMACFactory(const std::shared_ptr<HMACFactory>& factory);
            /**
             * Set the global factory for AES in CBC mode providers
             */
            AWS_CORE_API void SetAES_CBCFactory(const std::shared_ptr<SymmetricCipherFactory>& factory);
            /**
             * Set the global factory for AES in CTR mode providers
             */
            AWS_CORE_API void SetAES_CTRFactory(const std::shared_ptr<SymmetricCipherFactory>& factory);
            /**
             * Set the global factory for AES in GCM mode providers
             */
            AWS_CORE_API void SetAES_GCMFactory(const std::shared_ptr<SymmetricCipherFactory>& factory);
            /**
             * Set the global factory for 64 bit unsigned integer
             */
            AWS_CORE_API void Set64BitSecureRandomFactory();

            /**
             * Set the global factory for 32 bit unsigned integer
             */
            AWS_CORE_API void Set32BitSecureRandomFactory();

        } // namespace Crypto
    } // namespace Utils
} // namespace Aws

