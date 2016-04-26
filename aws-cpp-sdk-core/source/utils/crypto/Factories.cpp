/*
  * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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


#include <aws/core/utils/crypto/Factories.h>

#if ENABLE_BCRYPT_ENCRYPTION
    #include <aws/core/utils/crypto/bcrypt/CryptoImpl.h>
#elif ENABLE_OPENSSL_ENCRYPTION
    #include <aws/core/utils/crypto/openssl/CryptoImpl.h>
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
    #include <aws/core/utils/crypto/commoncrypto/CryptoImpl.h>
#endif

using namespace Aws::Utils;
using namespace Aws::Utils::Crypto;

static const char *s_allocationTag = "CryptoFactory";

static std::shared_ptr<HashFactory> s_MD5Factory(nullptr);
static std::shared_ptr<HashFactory> s_Sha256Factory(nullptr);
static std::shared_ptr<HMACFactory> s_Sha256HMACFactory(nullptr);

static std::shared_ptr<SymmetricCipherFactory> s_AES_CBCFactory(nullptr);
static std::shared_ptr<SymmetricCipherFactory> s_AES_CTRFactory(nullptr);
static std::shared_ptr<SymmetricCipherFactory> s_AES_GCMFactory(nullptr);

static std::shared_ptr<SecureRandomFactory<uint64_t>> s_SecureRandom64Factory(nullptr);
static std::shared_ptr<SecureRandomFactory<uint32_t>> s_SecureRandom32Factory(nullptr);

class DefaultMD5Factory : public HashFactory
{
public:
    std::shared_ptr<Hash> CreateImplementation() const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<MD5BcryptImpl>(s_allocationTag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<MD5OpenSSLImpl>(s_allocationTag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<MD5CommonCryptoImpl>(s_allocationTag);
#else
    return nullptr;
#endif
    }

    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultSHA256Factory : public HashFactory
{
public:
    std::shared_ptr<Hash> CreateImplementation() const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<Sha256BcryptImpl>(s_allocationTag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<Sha256OpenSSLImpl>(s_allocationTag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<Sha256CommonCryptoImpl>(s_allocationTag);
#else
    return nullptr;
#endif
    }

    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultSHA256HmacFactory : public HMACFactory
{
public:
    std::shared_ptr<HMAC> CreateImplementation() const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<Sha256HMACBcryptImpl>(s_allocationTag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<Sha256HMACOpenSSLImpl>(s_allocationTag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<Sha256HMACCommonCryptoImpl>(s_allocationTag);
#else
    return nullptr;
#endif
    }

    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultAES_CBCFactory : public SymmetricCipherFactory
{
public:
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CBC_BcryptImpl>(s_allocationTag, key);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_OpenSSL>(s_allocationTag, key);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_CommonCrypto>(s_allocationTag, key);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key, const CryptoBuffer& iv, const CryptoBuffer&) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CBC_BcryptImpl>(s_allocationTag, key, iv);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_OpenSSL>(s_allocationTag, key, iv);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_CommonCrypto>(s_allocationTag, key, iv);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(CryptoBuffer&& key, CryptoBuffer&& iv, CryptoBuffer&&) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CBC_BcryptImpl>(s_allocationTag, key, iv);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_OpenSSL>(s_allocationTag, key, iv);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CBC_Cipher_CommonCrypto>(s_allocationTag, key, iv);
#else
        return nullptr;
#endif
    }

    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultAES_CTRFactory : public SymmetricCipherFactory
{
public:
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CTR_BcryptImpl>(s_allocationTag, key);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_OpenSSL>(s_allocationTag, key);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_CommonCrypto>(s_allocationTag, key);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key, const CryptoBuffer& iv, const CryptoBuffer&) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CTR_BcryptImpl>(s_allocationTag, key, iv);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_OpenSSL>(s_allocationTag, key, iv);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_CommonCrypto>(s_allocationTag, key, iv);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(CryptoBuffer&& key, CryptoBuffer&& iv, CryptoBuffer&&) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_CTR_BcryptImpl>(s_allocationTag, key, iv);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_OpenSSL>(s_allocationTag, key, iv);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_CTR_Cipher_CommonCrypto>(s_allocationTag, key, iv);
#else
        return nullptr;
#endif
    }



    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultAES_GCMFactory : public SymmetricCipherFactory
{
public:
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_GCM_BcryptImpl>(s_allocationTag, key);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_OpenSSL>(s_allocationTag, key);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_CommonCrypto>(s_allocationTag, key);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(const CryptoBuffer& key, const CryptoBuffer& iv, const CryptoBuffer& tag) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_GCM_BcryptImpl>(s_allocationTag, key, iv, tag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_OpenSSL>(s_allocationTag, key, iv, tag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_CommonCrypto>(s_allocationTag, key, iv, tag);
#else
        return nullptr;
#endif
    }
    /**
     * Factory method. Returns cipher implementation. See the SymmetricCipher class for more details.
     */
    std::shared_ptr<SymmetricCipher> CreateImplementation(CryptoBuffer&& key, CryptoBuffer&& iv, CryptoBuffer&& tag) const override
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<AES_GCM_BcryptImpl>(s_allocationTag, key, iv, tag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_OpenSSL>(s_allocationTag, key, iv, tag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<AES_GCM_Cipher_CommonCrypto>(s_allocationTag, key, iv, tag);
#else
        return nullptr;
#endif
    }

    /**
     * Opportunity to make any static initialization calls you need to make.
     * Will only be called once.
     */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultSecureRand64Factory : public SecureRandomFactory<uint64_t>
{
    /**
     * Factory method. Returns SecureRandom implementation.
     */
    std::shared_ptr<SecureRandom<uint64_t>> CreateImplementation() const
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<SecureRand64_BcryptImpl<uint64_t>>(s_allocationTag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<SecureRandomOpenSSLImpl<uint64_t>>(s_allocationTag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<SecureRandom_CommonCrypto<uint64_t>>(s_allocationTag, key, iv, tag);
#else
        return nullptr;
#endif
    }

    /**
 * Opportunity to make any static initialization calls you need to make.
 * Will only be called once.
 */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

class DefaultSecureRand32Factory : public SecureRandomFactory<uint32_t>
{
    /**
     * Factory method. Returns SecureRandom implementation.
     */
    std::shared_ptr<SecureRandom<uint32_t>> CreateImplementation() const
    {
#if ENABLE_BCRYPT_ENCRYPTION
        return Aws::MakeShared<SecureRand64_BcryptImpl<uint32_t>>(s_allocationTag);
#elif ENABLE_OPENSSL_ENCRYPTION
        return Aws::MakeShared<SecureRandomOpenSSLImpl<uint32_t>>(s_allocationTag);
#elif ENABLE_COMMONCRYPTO_ENCRYPTION
        return Aws::MakeShared<SecureRandom_CommonCrypto<uint32_t>>(s_allocationTag, key, iv, tag);
#else
        return nullptr;
#endif
    }

    /**
 * Opportunity to make any static initialization calls you need to make.
 * Will only be called once.
 */
    void InitStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.EnterRoom(&OpenSSL::init_static_state);
#endif
    }

    /**
     * Opportunity to make any static cleanup calls you need to make.
     * will only be called at the end of the application.
     */
    void CleanupStaticState() override
    {
#if ENABLE_OPENSSL_ENCRYPTION
        OpenSSL::getTheLights.LeaveRoom(&OpenSSL::cleanup_static_state);
#endif
    }
};

void Aws::Utils::Crypto::InitCrypto()
{
    if(s_MD5Factory)
    {
        s_MD5Factory->InitStaticState();
    }
    else
    {
        s_MD5Factory = Aws::MakeShared<DefaultMD5Factory>(s_allocationTag);
        s_MD5Factory->InitStaticState();
    }

    if(s_Sha256Factory)
    {
        s_Sha256Factory->InitStaticState();
    }
    else
    {
        s_Sha256Factory = Aws::MakeShared<DefaultSHA256Factory>(s_allocationTag);
        s_Sha256Factory->InitStaticState();
    }

    if(s_Sha256HMACFactory)
    {
        s_Sha256HMACFactory->InitStaticState();
    }
    else
    {
        s_Sha256HMACFactory = Aws::MakeShared<DefaultSHA256HmacFactory>(s_allocationTag);
        s_Sha256HMACFactory->InitStaticState();
    }

    if(s_AES_CBCFactory)
    {
        s_AES_CBCFactory->InitStaticState();
    }
    else
    {
        s_AES_CBCFactory = Aws::MakeShared<DefaultAES_CBCFactory>(s_allocationTag);
        s_AES_CBCFactory->InitStaticState();
    }

    if(s_AES_CTRFactory)
    {
        s_AES_CTRFactory->InitStaticState();
    }
    else
    {
        s_AES_CTRFactory = Aws::MakeShared<DefaultAES_CTRFactory>(s_allocationTag);
        s_AES_CTRFactory->InitStaticState();
    }

    if(s_AES_GCMFactory)
    {
        s_AES_GCMFactory->InitStaticState();
    }
    else
    {
        s_AES_GCMFactory = Aws::MakeShared<DefaultAES_GCMFactory>(s_allocationTag);
        s_AES_GCMFactory->InitStaticState();
    }

    if(s_SecureRandom64Factory)
    {
        s_SecureRandom64Factory->InitStaticState();
    }
    else
    {
        s_SecureRandom64Factory = Aws::MakeShared<DefaultSecureRand64Factory>(s_allocationTag);
        s_SecureRandom64Factory->InitStaticState();
    }

    if(s_SecureRandom32Factory)
    {
        s_SecureRandom32Factory->InitStaticState();
    }
    else
    {
        s_SecureRandom32Factory = Aws::MakeShared<DefaultSecureRand32Factory>(s_allocationTag);
        s_SecureRandom32Factory->InitStaticState();
    }
}

void Aws::Utils::Crypto::CleanupCrypto()
{
    if(s_MD5Factory)
    {
        s_MD5Factory->CleanupStaticState();
        s_MD5Factory = nullptr;
    }

    if(s_Sha256Factory)
    {
        s_Sha256Factory->CleanupStaticState();
        s_Sha256Factory = nullptr;
    }

    if(s_Sha256HMACFactory)
    {
        s_Sha256HMACFactory->CleanupStaticState();
        s_Sha256HMACFactory =  nullptr;
    }

    if(s_AES_CBCFactory)
    {
        s_AES_CBCFactory->CleanupStaticState();
        s_AES_CBCFactory = nullptr;
    }

    if(s_AES_CTRFactory)
    {
        s_AES_CTRFactory->CleanupStaticState();
        s_AES_CTRFactory = nullptr;
    }

    if(s_AES_GCMFactory)
    {
        s_AES_GCMFactory->CleanupStaticState();
        s_AES_GCMFactory = nullptr;
    }

    if(s_SecureRandom64Factory)
    {
        s_SecureRandom64Factory->CleanupStaticState();
        s_SecureRandom64Factory = nullptr;
    }

    if(s_SecureRandom32Factory)
    {
        s_SecureRandom32Factory->CleanupStaticState();
        s_SecureRandom32Factory = nullptr;
    }
}

void Aws::Utils::Crypto::SetMD5Factory(const std::shared_ptr<HashFactory>& factory)
{
    s_MD5Factory = factory;
}

void Aws::Utils::Crypto::SetSha256Factory(const std::shared_ptr<HashFactory>& factory)
{
    s_Sha256Factory = factory;
}

void Aws::Utils::Crypto::SetSha256HMACFactory(const std::shared_ptr<HMACFactory>& factory)
{
    s_Sha256HMACFactory = factory;
}

void Aws::Utils::Crypto::SetAES_CBCFactory(const std::shared_ptr<SymmetricCipherFactory>& factory)
{
    s_AES_CBCFactory = factory;
}

void Aws::Utils::Crypto::SetAES_CTRFactory(const std::shared_ptr<SymmetricCipherFactory>& factory)
{
    s_AES_CTRFactory = factory;
}

void Aws::Utils::Crypto::SetAES_GCMFactory(const std::shared_ptr<SymmetricCipherFactory>& factory)
{
    s_AES_GCMFactory = factory;
}

std::shared_ptr<Hash> Aws::Utils::Crypto::CreateMD5Implementation()
{
    return s_MD5Factory->CreateImplementation();
}

std::shared_ptr<Hash> Aws::Utils::Crypto::CreateSha256Implementation()
{
    return s_Sha256Factory->CreateImplementation();
}

std::shared_ptr<HMAC> Aws::Utils::Crypto::CreateSha256HMACImplementation()
{
    return s_Sha256HMACFactory->CreateImplementation();
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CBCImplementation(const CryptoBuffer& key)
{
    return s_AES_CBCFactory->CreateImplementation(key);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CBCImplementation(const CryptoBuffer& key, const CryptoBuffer& iv)
{
    return s_AES_CBCFactory->CreateImplementation(key, iv);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CBCImplementation(CryptoBuffer&& key, CryptoBuffer&& iv)
{
    return s_AES_CBCFactory->CreateImplementation(std::move(key), std::move(iv));
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CTRImplementation(const CryptoBuffer& key)
{
    return s_AES_CTRFactory->CreateImplementation(key);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CTRImplementation(const CryptoBuffer& key, const CryptoBuffer& iv)
{
    return s_AES_CTRFactory->CreateImplementation(key, iv);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_CTRImplementation(CryptoBuffer&& key, CryptoBuffer&& iv)
{
    return s_AES_CTRFactory->CreateImplementation(std::move(key), std::move(iv));
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_GCMImplementation(const CryptoBuffer& key)
{
    return s_AES_GCMFactory->CreateImplementation(key);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_GCMImplementation(const CryptoBuffer& key, const CryptoBuffer& iv, const CryptoBuffer& tag)
{
    return s_AES_GCMFactory->CreateImplementation(key, iv, tag);
}

std::shared_ptr<SymmetricCipher> Aws::Utils::Crypto::CreateAES_GCMImplementation(CryptoBuffer&& key, CryptoBuffer&& iv, CryptoBuffer&& tag)
{
    return s_AES_GCMFactory->CreateImplementation(std::move(key), std::move(iv), std::move(tag));
}

std::shared_ptr<SecureRandom<uint64_t>> Aws::Utils::Crypto::Create64BitSecureRandomImplementation()
{
    return s_SecureRandom64Factory->CreateImplementation();
}

std::shared_ptr<SecureRandom<uint32_t>> Aws::Utils::Crypto::Create32BitSecureRandomImplementation()
{
    return s_SecureRandom32Factory->CreateImplementation();
}


