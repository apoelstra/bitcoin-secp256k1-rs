/***********************************************************************
 * Copyright (c) 2014 Pieter Wuille                                    *
 * Distributed under the MIT software license, see the accompanying    *
 * file COPYING or https://www.opensource.org/licenses/mit-license.php.*
 ***********************************************************************/

#ifndef SECP256K1_SCALAR_IMPL_H
#define SECP256K1_SCALAR_IMPL_H

#ifdef VERIFY
#include <string.h>
#endif

#include "scalar.h"
#include "util.h"

#if defined(EXHAUSTIVE_TEST_ORDER)
#include "scalar_low_impl.h"
#elif defined(SECP256K1_WIDEMUL_INT128)
#include "scalar_4x64_impl.h"
#elif defined(SECP256K1_WIDEMUL_INT64)
#include "scalar_8x32_impl.h"
#else
#error "Please select wide multiplication implementation"
#endif

static const rustsecp256k1_v0_9_0_scalar rustsecp256k1_v0_9_0_scalar_one = SECP256K1_SCALAR_CONST(0, 0, 0, 0, 0, 0, 0, 1);
static const rustsecp256k1_v0_9_0_scalar rustsecp256k1_v0_9_0_scalar_zero = SECP256K1_SCALAR_CONST(0, 0, 0, 0, 0, 0, 0, 0);

static int rustsecp256k1_v0_9_0_scalar_set_b32_seckey(rustsecp256k1_v0_9_0_scalar *r, const unsigned char *bin) {
    int overflow;
    rustsecp256k1_v0_9_0_scalar_set_b32(r, bin, &overflow);
    return (!overflow) & (!rustsecp256k1_v0_9_0_scalar_is_zero(r));
}

#if defined(EXHAUSTIVE_TEST_ORDER)
/* Begin of section generated by sage/gen_exhaustive_groups.sage. */
#  if EXHAUSTIVE_TEST_ORDER == 7
#    define EXHAUSTIVE_TEST_LAMBDA 2
#  elif EXHAUSTIVE_TEST_ORDER == 13
#    define EXHAUSTIVE_TEST_LAMBDA 9
#  elif EXHAUSTIVE_TEST_ORDER == 199
#    define EXHAUSTIVE_TEST_LAMBDA 92
#  else
#    error No known lambda for the specified exhaustive test group order.
#  endif
/* End of section generated by sage/gen_exhaustive_groups.sage. */

/**
 * Find r1 and r2 given k, such that r1 + r2 * lambda == k mod n; unlike in the
 * full case we don't bother making r1 and r2 be small, we just want them to be
 * nontrivial to get full test coverage for the exhaustive tests. We therefore
 * (arbitrarily) set r2 = k + 5 (mod n) and r1 = k - r2 * lambda (mod n).
 */
static void rustsecp256k1_v0_9_0_scalar_split_lambda(rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT r1, rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT r2, const rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT k) {
    VERIFY_CHECK(r1 != k);
    VERIFY_CHECK(r2 != k);
    VERIFY_CHECK(r1 != r2);
    *r2 = (*k + 5) % EXHAUSTIVE_TEST_ORDER;
    *r1 = (*k + (EXHAUSTIVE_TEST_ORDER - *r2) * EXHAUSTIVE_TEST_LAMBDA) % EXHAUSTIVE_TEST_ORDER;
}
#else
/**
 * The Secp256k1 curve has an endomorphism, where lambda * (x, y) = (beta * x, y), where
 * lambda is: */
static const rustsecp256k1_v0_9_0_scalar rustsecp256k1_v0_9_0_const_lambda = SECP256K1_SCALAR_CONST(
    0x5363AD4CUL, 0xC05C30E0UL, 0xA5261C02UL, 0x8812645AUL,
    0x122E22EAUL, 0x20816678UL, 0xDF02967CUL, 0x1B23BD72UL
);

#ifdef VERIFY
static void rustsecp256k1_v0_9_0_scalar_split_lambda_verify(const rustsecp256k1_v0_9_0_scalar *r1, const rustsecp256k1_v0_9_0_scalar *r2, const rustsecp256k1_v0_9_0_scalar *k);
#endif

/*
 * Both lambda and beta are primitive cube roots of unity.  That is lamba^3 == 1 mod n and
 * beta^3 == 1 mod p, where n is the curve order and p is the field order.
 *
 * Furthermore, because (X^3 - 1) = (X - 1)(X^2 + X + 1), the primitive cube roots of unity are
 * roots of X^2 + X + 1.  Therefore lambda^2 + lamba == -1 mod n and beta^2 + beta == -1 mod p.
 * (The other primitive cube roots of unity are lambda^2 and beta^2 respectively.)
 *
 * Let l = -1/2 + i*sqrt(3)/2, the complex root of X^2 + X + 1. We can define a ring
 * homomorphism phi : Z[l] -> Z_n where phi(a + b*l) == a + b*lambda mod n. The kernel of phi
 * is a lattice over Z[l] (considering Z[l] as a Z-module). This lattice is generated by a
 * reduced basis {a1 + b1*l, a2 + b2*l} where
 *
 * - a1 =      {0x30,0x86,0xd2,0x21,0xa7,0xd4,0x6b,0xcd,0xe8,0x6c,0x90,0xe4,0x92,0x84,0xeb,0x15}
 * - b1 =     -{0xe4,0x43,0x7e,0xd6,0x01,0x0e,0x88,0x28,0x6f,0x54,0x7f,0xa9,0x0a,0xbf,0xe4,0xc3}
 * - a2 = {0x01,0x14,0xca,0x50,0xf7,0xa8,0xe2,0xf3,0xf6,0x57,0xc1,0x10,0x8d,0x9d,0x44,0xcf,0xd8}
 * - b2 =      {0x30,0x86,0xd2,0x21,0xa7,0xd4,0x6b,0xcd,0xe8,0x6c,0x90,0xe4,0x92,0x84,0xeb,0x15}
 *
 * "Guide to Elliptic Curve Cryptography" (Hankerson, Menezes, Vanstone) gives an algorithm
 * (algorithm 3.74) to find k1 and k2 given k, such that k1 + k2 * lambda == k mod n, and k1
 * and k2 are small in absolute value.
 *
 * The algorithm computes c1 = round(b2 * k / n) and c2 = round((-b1) * k / n), and gives
 * k1 = k - (c1*a1 + c2*a2) and k2 = -(c1*b1 + c2*b2). Instead, we use modular arithmetic, and
 * compute r2 = k2 mod n, and r1 = k1 mod n = (k - r2 * lambda) mod n, avoiding the need for
 * the constants a1 and a2.
 *
 * g1, g2 are precomputed constants used to replace division with a rounded multiplication
 * when decomposing the scalar for an endomorphism-based point multiplication.
 *
 * The possibility of using precomputed estimates is mentioned in "Guide to Elliptic Curve
 * Cryptography" (Hankerson, Menezes, Vanstone) in section 3.5.
 *
 * The derivation is described in the paper "Efficient Software Implementation of Public-Key
 * Cryptography on Sensor Networks Using the MSP430X Microcontroller" (Gouvea, Oliveira, Lopez),
 * Section 4.3 (here we use a somewhat higher-precision estimate):
 * d = a1*b2 - b1*a2
 * g1 = round(2^384 * b2/d)
 * g2 = round(2^384 * (-b1)/d)
 *
 * (Note that d is also equal to the curve order, n, here because [a1,b1] and [a2,b2]
 * can be found as outputs of the Extended Euclidean Algorithm on inputs n and lambda).
 *
 * The function below splits k into r1 and r2, such that
 * - r1 + lambda * r2 == k (mod n)
 * - either r1 < 2^128 or -r1 mod n < 2^128
 * - either r2 < 2^128 or -r2 mod n < 2^128
 *
 * See proof below.
 */
static void rustsecp256k1_v0_9_0_scalar_split_lambda(rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT r1, rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT r2, const rustsecp256k1_v0_9_0_scalar * SECP256K1_RESTRICT k) {
    rustsecp256k1_v0_9_0_scalar c1, c2;
    static const rustsecp256k1_v0_9_0_scalar minus_b1 = SECP256K1_SCALAR_CONST(
        0x00000000UL, 0x00000000UL, 0x00000000UL, 0x00000000UL,
        0xE4437ED6UL, 0x010E8828UL, 0x6F547FA9UL, 0x0ABFE4C3UL
    );
    static const rustsecp256k1_v0_9_0_scalar minus_b2 = SECP256K1_SCALAR_CONST(
        0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFEUL,
        0x8A280AC5UL, 0x0774346DUL, 0xD765CDA8UL, 0x3DB1562CUL
    );
    static const rustsecp256k1_v0_9_0_scalar g1 = SECP256K1_SCALAR_CONST(
        0x3086D221UL, 0xA7D46BCDUL, 0xE86C90E4UL, 0x9284EB15UL,
        0x3DAA8A14UL, 0x71E8CA7FUL, 0xE893209AUL, 0x45DBB031UL
    );
    static const rustsecp256k1_v0_9_0_scalar g2 = SECP256K1_SCALAR_CONST(
        0xE4437ED6UL, 0x010E8828UL, 0x6F547FA9UL, 0x0ABFE4C4UL,
        0x221208ACUL, 0x9DF506C6UL, 0x1571B4AEUL, 0x8AC47F71UL
    );
    VERIFY_CHECK(r1 != k);
    VERIFY_CHECK(r2 != k);
    VERIFY_CHECK(r1 != r2);
    /* these _var calls are constant time since the shift amount is constant */
    rustsecp256k1_v0_9_0_scalar_mul_shift_var(&c1, k, &g1, 384);
    rustsecp256k1_v0_9_0_scalar_mul_shift_var(&c2, k, &g2, 384);
    rustsecp256k1_v0_9_0_scalar_mul(&c1, &c1, &minus_b1);
    rustsecp256k1_v0_9_0_scalar_mul(&c2, &c2, &minus_b2);
    rustsecp256k1_v0_9_0_scalar_add(r2, &c1, &c2);
    rustsecp256k1_v0_9_0_scalar_mul(r1, r2, &rustsecp256k1_v0_9_0_const_lambda);
    rustsecp256k1_v0_9_0_scalar_negate(r1, r1);
    rustsecp256k1_v0_9_0_scalar_add(r1, r1, k);

#ifdef VERIFY
    rustsecp256k1_v0_9_0_scalar_split_lambda_verify(r1, r2, k);
#endif
}

#ifdef VERIFY
/*
 * Proof for rustsecp256k1_v0_9_0_scalar_split_lambda's bounds.
 *
 * Let
 *  - epsilon1 = 2^256 * |g1/2^384 - b2/d|
 *  - epsilon2 = 2^256 * |g2/2^384 - (-b1)/d|
 *  - c1 = round(k*g1/2^384)
 *  - c2 = round(k*g2/2^384)
 *
 * Lemma 1: |c1 - k*b2/d| < 2^-1 + epsilon1
 *
 *    |c1 - k*b2/d|
 *  =
 *    |c1 - k*g1/2^384 + k*g1/2^384 - k*b2/d|
 * <=   {triangle inequality}
 *    |c1 - k*g1/2^384| + |k*g1/2^384 - k*b2/d|
 *  =
 *    |c1 - k*g1/2^384| + k*|g1/2^384 - b2/d|
 * <    {rounding in c1 and 0 <= k < 2^256}
 *    2^-1 + 2^256 * |g1/2^384 - b2/d|
 *  =   {definition of epsilon1}
 *    2^-1 + epsilon1
 *
 * Lemma 2: |c2 - k*(-b1)/d| < 2^-1 + epsilon2
 *
 *    |c2 - k*(-b1)/d|
 *  =
 *    |c2 - k*g2/2^384 + k*g2/2^384 - k*(-b1)/d|
 * <=   {triangle inequality}
 *    |c2 - k*g2/2^384| + |k*g2/2^384 - k*(-b1)/d|
 *  =
 *    |c2 - k*g2/2^384| + k*|g2/2^384 - (-b1)/d|
 * <    {rounding in c2 and 0 <= k < 2^256}
 *    2^-1 + 2^256 * |g2/2^384 - (-b1)/d|
 *  =   {definition of epsilon2}
 *    2^-1 + epsilon2
 *
 * Let
 *  - k1 = k - c1*a1 - c2*a2
 *  - k2 = - c1*b1 - c2*b2
 *
 * Lemma 3: |k1| < (a1 + a2 + 1)/2 < 2^128
 *
 *    |k1|
 *  =   {definition of k1}
 *    |k - c1*a1 - c2*a2|
 *  =   {(a1*b2 - b1*a2)/n = 1}
 *    |k*(a1*b2 - b1*a2)/n - c1*a1 - c2*a2|
 *  =
 *    |a1*(k*b2/n - c1) + a2*(k*(-b1)/n - c2)|
 * <=   {triangle inequality}
 *    a1*|k*b2/n - c1| + a2*|k*(-b1)/n - c2|
 * <    {Lemma 1 and Lemma 2}
 *    a1*(2^-1 + epslion1) + a2*(2^-1 + epsilon2)
 * <    {rounding up to an integer}
 *    (a1 + a2 + 1)/2
 * <    {rounding up to a power of 2}
 *    2^128
 *
 * Lemma 4: |k2| < (-b1 + b2)/2 + 1 < 2^128
 *
 *    |k2|
 *  =   {definition of k2}
 *    |- c1*a1 - c2*a2|
 *  =   {(b1*b2 - b1*b2)/n = 0}
 *    |k*(b1*b2 - b1*b2)/n - c1*b1 - c2*b2|
 *  =
 *    |b1*(k*b2/n - c1) + b2*(k*(-b1)/n - c2)|
 * <=   {triangle inequality}
 *    (-b1)*|k*b2/n - c1| + b2*|k*(-b1)/n - c2|
 * <    {Lemma 1 and Lemma 2}
 *    (-b1)*(2^-1 + epslion1) + b2*(2^-1 + epsilon2)
 * <    {rounding up to an integer}
 *    (-b1 + b2)/2 + 1
 * <    {rounding up to a power of 2}
 *    2^128
 *
 * Let
 *  - r2 = k2 mod n
 *  - r1 = k - r2*lambda mod n.
 *
 * Notice that r1 is defined such that r1 + r2 * lambda == k (mod n).
 *
 * Lemma 5: r1 == k1 mod n.
 *
 *    r1
 * ==   {definition of r1 and r2}
 *    k - k2*lambda
 * ==   {definition of k2}
 *    k - (- c1*b1 - c2*b2)*lambda
 * ==
 *    k + c1*b1*lambda + c2*b2*lambda
 * ==  {a1 + b1*lambda == 0 mod n and a2 + b2*lambda == 0 mod n}
 *    k - c1*a1 - c2*a2
 * ==  {definition of k1}
 *    k1
 *
 * From Lemma 3, Lemma 4, Lemma 5 and the definition of r2, we can conclude that
 *
 *  - either r1 < 2^128 or -r1 mod n < 2^128
 *  - either r2 < 2^128 or -r2 mod n < 2^128.
 *
 * Q.E.D.
 */
static void rustsecp256k1_v0_9_0_scalar_split_lambda_verify(const rustsecp256k1_v0_9_0_scalar *r1, const rustsecp256k1_v0_9_0_scalar *r2, const rustsecp256k1_v0_9_0_scalar *k) {
    rustsecp256k1_v0_9_0_scalar s;
    unsigned char buf1[32];
    unsigned char buf2[32];

    /* (a1 + a2 + 1)/2 is 0xa2a8918ca85bafe22016d0b917e4dd77 */
    static const unsigned char k1_bound[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xa2, 0xa8, 0x91, 0x8c, 0xa8, 0x5b, 0xaf, 0xe2, 0x20, 0x16, 0xd0, 0xb9, 0x17, 0xe4, 0xdd, 0x77
    };

    /* (-b1 + b2)/2 + 1 is 0x8a65287bd47179fb2be08846cea267ed */
    static const unsigned char k2_bound[32] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x8a, 0x65, 0x28, 0x7b, 0xd4, 0x71, 0x79, 0xfb, 0x2b, 0xe0, 0x88, 0x46, 0xce, 0xa2, 0x67, 0xed
    };

    rustsecp256k1_v0_9_0_scalar_mul(&s, &rustsecp256k1_v0_9_0_const_lambda, r2);
    rustsecp256k1_v0_9_0_scalar_add(&s, &s, r1);
    VERIFY_CHECK(rustsecp256k1_v0_9_0_scalar_eq(&s, k));

    rustsecp256k1_v0_9_0_scalar_negate(&s, r1);
    rustsecp256k1_v0_9_0_scalar_get_b32(buf1, r1);
    rustsecp256k1_v0_9_0_scalar_get_b32(buf2, &s);
    VERIFY_CHECK(rustsecp256k1_v0_9_0_memcmp_var(buf1, k1_bound, 32) < 0 || rustsecp256k1_v0_9_0_memcmp_var(buf2, k1_bound, 32) < 0);

    rustsecp256k1_v0_9_0_scalar_negate(&s, r2);
    rustsecp256k1_v0_9_0_scalar_get_b32(buf1, r2);
    rustsecp256k1_v0_9_0_scalar_get_b32(buf2, &s);
    VERIFY_CHECK(rustsecp256k1_v0_9_0_memcmp_var(buf1, k2_bound, 32) < 0 || rustsecp256k1_v0_9_0_memcmp_var(buf2, k2_bound, 32) < 0);
}
#endif /* VERIFY */
#endif /* !defined(EXHAUSTIVE_TEST_ORDER) */

#endif /* SECP256K1_SCALAR_IMPL_H */
