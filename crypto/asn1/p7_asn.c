/* p7_asn.c */
/* Written by Dr Stephen N Henson (shenson@bigfoot.com) for the OpenSSL
 * project 2000.
 */
/* ====================================================================
 * Copyright (c) 2000 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#include <stdio.h>
#include "cryptlib.h"
#include <openssl/asn1t.h>
#include <openssl/pkcs7.h>

/* PKCS#7 ASN1 module */

/* This is the ANY DEFINED BY table for the top level PKCS#7 structure */

ASN1_ADB_TEMPLATE(p7default) = ASN1_EXP_OPT(PKCS7, d.other, ASN1_ANY, 0);

ASN1_ADB(PKCS7) = {
	ADB_ENTRY(NID_pkcs7_data, ASN1_EXP_OPT(PKCS7, d.data, ASN1_OCTET_STRING, 0)),
	ADB_ENTRY(NID_pkcs7_signed, ASN1_EXP_OPT(PKCS7, d.sign, PKCS7_SIGNED, 0)),
	ADB_ENTRY(NID_pkcs7_enveloped, ASN1_EXP_OPT(PKCS7, d.enveloped, PKCS7_ENVELOPE, 0)),
	ADB_ENTRY(NID_pkcs7_signedAndEnveloped, ASN1_EXP_OPT(PKCS7, d.signed_and_enveloped, PKCS7_SIGN_ENVELOPE, 0)),
	ADB_ENTRY(NID_pkcs7_digest, ASN1_EXP_OPT(PKCS7, d.digest, PKCS7_DIGEST, 0)),
	ADB_ENTRY(NID_pkcs7_encrypted, ASN1_EXP_OPT(PKCS7, d.encrypted, PKCS7_ENCRYPT, 0))
} ASN1_ADB_END(PKCS7, 0, type, 0, &p7default_tt, NULL);

ASN1_SEQUENCE(PKCS7) = {
	ASN1_SIMPLE(PKCS7, type, ASN1_OBJECT),
	ASN1_ADB_OBJECT(PKCS7)
}ASN1_SEQUENCE_END(PKCS7);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7)

ASN1_SEQUENCE(PKCS7_SIGNED) = {
	ASN1_SIMPLE(PKCS7_SIGNED, version, ASN1_INTEGER),
	ASN1_SET_OF(PKCS7_SIGNED, md_algs, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_SIGNED, contents, PKCS7),
	ASN1_IMP_SEQUENCE_OF_OPT(PKCS7_SIGNED, cert, X509, 0),
	ASN1_IMP_SET_OF_OPT(PKCS7_SIGNED, crl, X509_CRL, 1),
	ASN1_SET_OF(PKCS7_SIGNED, signer_info, PKCS7_SIGNER_INFO)
} ASN1_SEQUENCE_END(PKCS7_SIGNED);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SIGNED)

ASN1_SEQUENCE(PKCS7_SIGNER_INFO) = {
	ASN1_SIMPLE(PKCS7_SIGNER_INFO, version, ASN1_INTEGER),
	ASN1_SIMPLE(PKCS7_SIGNER_INFO, issuer_and_serial, PKCS7_ISSUER_AND_SERIAL),
	ASN1_SIMPLE(PKCS7_SIGNER_INFO, digest_alg, X509_ALGOR),
	ASN1_IMP_SET_OF_OPT(PKCS7_SIGNER_INFO, auth_attr, X509_ATTRIBUTE, 0),
	ASN1_SIMPLE(PKCS7_SIGNER_INFO, digest_enc_alg, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_SIGNER_INFO, enc_digest, ASN1_OCTET_STRING),
	ASN1_IMP_SET_OF_OPT(PKCS7_SIGNER_INFO, unauth_attr, X509_ATTRIBUTE, 1)
} ASN1_SEQUENCE_END(PKCS7_SIGNER_INFO);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SIGNER_INFO)

ASN1_SEQUENCE(PKCS7_ISSUER_AND_SERIAL) = {
	ASN1_SIMPLE(PKCS7_ISSUER_AND_SERIAL, issuer, X509_NAME),
	ASN1_SIMPLE(PKCS7_ISSUER_AND_SERIAL, serial, ASN1_INTEGER)
} ASN1_SEQUENCE_END(PKCS7_ISSUER_AND_SERIAL);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_ISSUER_AND_SERIAL)

ASN1_SEQUENCE(PKCS7_ENVELOPE) = {
	ASN1_SIMPLE(PKCS7_ENVELOPE, version, ASN1_INTEGER),
	ASN1_SET_OF(PKCS7_ENVELOPE, recipientinfo, PKCS7_RECIP_INFO),
	ASN1_SIMPLE(PKCS7_ENVELOPE, enc_data, PKCS7_ENC_CONTENT)
} ASN1_SEQUENCE_END(PKCS7_ENVELOPE);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_ENVELOPE)

ASN1_SEQUENCE(PKCS7_RECIP_INFO) = {
	ASN1_SIMPLE(PKCS7_RECIP_INFO, version, ASN1_INTEGER),
	ASN1_SIMPLE(PKCS7_RECIP_INFO, issuer_and_serial, PKCS7_ISSUER_AND_SERIAL),
	ASN1_SIMPLE(PKCS7_RECIP_INFO, key_enc_algor, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_RECIP_INFO, enc_key, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(PKCS7_RECIP_INFO);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_RECIP_INFO)

ASN1_SEQUENCE(PKCS7_ENC_CONTENT) = {
	ASN1_SIMPLE(PKCS7_ENC_CONTENT, content_type, ASN1_OBJECT),
	ASN1_SIMPLE(PKCS7_ENC_CONTENT, algorithm, X509_ALGOR),
	ASN1_IMP_OPT(PKCS7_ENC_CONTENT, enc_data, ASN1_OCTET_STRING, 0)
} ASN1_SEQUENCE_END(PKCS7_ENC_CONTENT);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_ENC_CONTENT)

ASN1_SEQUENCE(PKCS7_SIGN_ENVELOPE) = {
	ASN1_SIMPLE(PKCS7_SIGN_ENVELOPE, version, ASN1_INTEGER),
	ASN1_SET_OF(PKCS7_SIGN_ENVELOPE, recipientinfo, PKCS7_RECIP_INFO),
	ASN1_SET_OF(PKCS7_SIGN_ENVELOPE, md_algs, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_SIGN_ENVELOPE, enc_data, PKCS7_ENC_CONTENT),
	ASN1_IMP_SET_OF_OPT(PKCS7_SIGN_ENVELOPE, cert, X509, 0),
	ASN1_IMP_SET_OF_OPT(PKCS7_SIGN_ENVELOPE, crl, X509_CRL, 1),
	ASN1_SET_OF(PKCS7_SIGN_ENVELOPE, signer_info, PKCS7_SIGNER_INFO)
} ASN1_SEQUENCE_END(PKCS7_SIGN_ENVELOPE);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_SIGN_ENVELOPE)

ASN1_SEQUENCE(PKCS7_ENCRYPT) = {
	ASN1_SIMPLE(PKCS7_ENCRYPT, version, ASN1_INTEGER),
	ASN1_SIMPLE(PKCS7_ENCRYPT, enc_data, PKCS7_ENC_CONTENT)
} ASN1_SEQUENCE_END(PKCS7_ENCRYPT);

IMPLEMENT_ASN1_FUNCTIONS(PKCS7_ENCRYPT)

ASN1_SEQUENCE(PKCS7_DIGEST) = {
	ASN1_SIMPLE(PKCS7_DIGEST, version, ASN1_INTEGER),
	ASN1_SIMPLE(PKCS7_DIGEST, md, X509_ALGOR),
	ASN1_SIMPLE(PKCS7_DIGEST, contents, PKCS7),
	ASN1_SIMPLE(PKCS7_DIGEST, digest, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(PKCS7_DIGEST);
