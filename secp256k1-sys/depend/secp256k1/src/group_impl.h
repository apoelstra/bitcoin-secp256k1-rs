/***********************************************************************
 * Copyright (c) 2013, 2014 Pieter Wuille                              *
 * Distributed under the MIT software license, see the accompanying    *
 * file COPYING or https://www.opensource.org/licenses/mit-license.php.*
 ***********************************************************************/

#ifndef SECP256K1_GROUP_IMPL_H
#define SECP256K1_GROUP_IMPL_H

#include "field.h"
#include "group.h"
#include "util.h"

/* Begin of section generated by sage/gen_exhaustive_groups.sage. */
#define SECP256K1_G_ORDER_7 SECP256K1_GE_CONST(\
    0x66625d13, 0x317ffe44, 0x63d32cff, 0x1ca02b9b,\
    0xe5c6d070, 0x50b4b05e, 0x81cc30db, 0xf5166f0a,\
    0x1e60e897, 0xa7c00c7c, 0x2df53eb6, 0x98274ff4,\
    0x64252f42, 0x8ca44e17, 0x3b25418c, 0xff4ab0cf\
)
#define SECP256K1_G_ORDER_13 SECP256K1_GE_CONST(\
    0xa2482ff8, 0x4bf34edf, 0xa51262fd, 0xe57921db,\
    0xe0dd2cb7, 0xa5914790, 0xbc71631f, 0xc09704fb,\
    0x942536cb, 0xa3e49492, 0x3a701cc3, 0xee3e443f,\
    0xdf182aa9, 0x15b8aa6a, 0x166d3b19, 0xba84b045\
)
#define SECP256K1_G_ORDER_199 SECP256K1_GE_CONST(\
    0x7fb07b5c, 0xd07c3bda, 0x553902e2, 0x7a87ea2c,\
    0x35108a7f, 0x051f41e5, 0xb76abad5, 0x1f2703ad,\
    0x0a251539, 0x5b4c4438, 0x952a634f, 0xac10dd4d,\
    0x6d6f4745, 0x98990c27, 0x3a4f3116, 0xd32ff969\
)
/** Generator for secp256k1, value 'g' defined in
 *  "Standards for Efficient Cryptography" (SEC2) 2.7.1.
 */
#define SECP256K1_G SECP256K1_GE_CONST(\
    0x79be667e, 0xf9dcbbac, 0x55a06295, 0xce870b07,\
    0x029bfcdb, 0x2dce28d9, 0x59f2815b, 0x16f81798,\
    0x483ada77, 0x26a3c465, 0x5da4fbfc, 0x0e1108a8,\
    0xfd17b448, 0xa6855419, 0x9c47d08f, 0xfb10d4b8\
)
/* These exhaustive group test orders and generators are chosen such that:
 * - The field size is equal to that of secp256k1, so field code is the same.
 * - The curve equation is of the form y^2=x^3+B for some small constant B.
 * - The subgroup has a generator 2*P, where P.x is as small as possible.
 * - The subgroup has size less than 1000 to permit exhaustive testing.
 * - The subgroup admits an endomorphism of the form lambda*(x,y) == (beta*x,y).
 */
#if defined(EXHAUSTIVE_TEST_ORDER)
#  if EXHAUSTIVE_TEST_ORDER == 7

static const rustsecp256k1_v0_8_1_ge rustsecp256k1_v0_8_1_ge_const_g = SECP256K1_G_ORDER_7;
#define SECP256K1_B 6

#  elif EXHAUSTIVE_TEST_ORDER == 13

static const rustsecp256k1_v0_8_1_ge rustsecp256k1_v0_8_1_ge_const_g = SECP256K1_G_ORDER_13;
#define SECP256K1_B 2

#  elif EXHAUSTIVE_TEST_ORDER == 199

static const rustsecp256k1_v0_8_1_ge rustsecp256k1_v0_8_1_ge_const_g = SECP256K1_G_ORDER_199;
#define SECP256K1_B 4

#  else
#    error No known generator for the specified exhaustive test group order.
#  endif
#else

static const rustsecp256k1_v0_8_1_ge rustsecp256k1_v0_8_1_ge_const_g = SECP256K1_G;
#define SECP256K1_B 7

#endif
/* End of section generated by sage/gen_exhaustive_groups.sage. */

static void rustsecp256k1_v0_8_1_ge_verify(const rustsecp256k1_v0_8_1_ge *a) {
#ifdef VERIFY
    rustsecp256k1_v0_8_1_fe_verify(&a->x);
    rustsecp256k1_v0_8_1_fe_verify(&a->y);
    VERIFY_CHECK(a->infinity == 0 || a->infinity == 1);
#endif
    (void)a;
}

static void rustsecp256k1_v0_8_1_gej_verify(const rustsecp256k1_v0_8_1_gej *a) {
#ifdef VERIFY
    rustsecp256k1_v0_8_1_fe_verify(&a->x);
    rustsecp256k1_v0_8_1_fe_verify(&a->y);
    rustsecp256k1_v0_8_1_fe_verify(&a->z);
    VERIFY_CHECK(a->infinity == 0 || a->infinity == 1);
#endif
    (void)a;
}

/* Set r to the affine coordinates of Jacobian point (a.x, a.y, 1/zi). */
static void rustsecp256k1_v0_8_1_ge_set_gej_zinv(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_fe *zi) {
    rustsecp256k1_v0_8_1_fe zi2;
    rustsecp256k1_v0_8_1_fe zi3;
    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_fe_verify(zi);
    VERIFY_CHECK(!a->infinity);
    rustsecp256k1_v0_8_1_fe_sqr(&zi2, zi);
    rustsecp256k1_v0_8_1_fe_mul(&zi3, &zi2, zi);
    rustsecp256k1_v0_8_1_fe_mul(&r->x, &a->x, &zi2);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &a->y, &zi3);
    r->infinity = a->infinity;
    rustsecp256k1_v0_8_1_ge_verify(r);
}

/* Set r to the affine coordinates of Jacobian point (a.x, a.y, 1/zi). */
static void rustsecp256k1_v0_8_1_ge_set_ge_zinv(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_ge *a, const rustsecp256k1_v0_8_1_fe *zi) {
    rustsecp256k1_v0_8_1_fe zi2;
    rustsecp256k1_v0_8_1_fe zi3;
    rustsecp256k1_v0_8_1_ge_verify(a);
    rustsecp256k1_v0_8_1_fe_verify(zi);
    VERIFY_CHECK(!a->infinity);
    rustsecp256k1_v0_8_1_fe_sqr(&zi2, zi);
    rustsecp256k1_v0_8_1_fe_mul(&zi3, &zi2, zi);
    rustsecp256k1_v0_8_1_fe_mul(&r->x, &a->x, &zi2);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &a->y, &zi3);
    r->infinity = a->infinity;
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_set_xy(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_fe *x, const rustsecp256k1_v0_8_1_fe *y) {
    rustsecp256k1_v0_8_1_fe_verify(x);
    rustsecp256k1_v0_8_1_fe_verify(y);
    r->infinity = 0;
    r->x = *x;
    r->y = *y;
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static int rustsecp256k1_v0_8_1_ge_is_infinity(const rustsecp256k1_v0_8_1_ge *a) {
    rustsecp256k1_v0_8_1_ge_verify(a);
    return a->infinity;
}

static void rustsecp256k1_v0_8_1_ge_neg(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_ge *a) {
    rustsecp256k1_v0_8_1_ge_verify(a);
    *r = *a;
    rustsecp256k1_v0_8_1_fe_normalize_weak(&r->y);
    rustsecp256k1_v0_8_1_fe_negate(&r->y, &r->y, 1);
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_set_gej(rustsecp256k1_v0_8_1_ge *r, rustsecp256k1_v0_8_1_gej *a) {
    rustsecp256k1_v0_8_1_fe z2, z3;
    rustsecp256k1_v0_8_1_gej_verify(a);
    r->infinity = a->infinity;
    rustsecp256k1_v0_8_1_fe_inv(&a->z, &a->z);
    rustsecp256k1_v0_8_1_fe_sqr(&z2, &a->z);
    rustsecp256k1_v0_8_1_fe_mul(&z3, &a->z, &z2);
    rustsecp256k1_v0_8_1_fe_mul(&a->x, &a->x, &z2);
    rustsecp256k1_v0_8_1_fe_mul(&a->y, &a->y, &z3);
    rustsecp256k1_v0_8_1_fe_set_int(&a->z, 1);
    r->x = a->x;
    r->y = a->y;
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_set_gej_var(rustsecp256k1_v0_8_1_ge *r, rustsecp256k1_v0_8_1_gej *a) {
    rustsecp256k1_v0_8_1_fe z2, z3;
    rustsecp256k1_v0_8_1_gej_verify(a);
    if (rustsecp256k1_v0_8_1_gej_is_infinity(a)) {
        rustsecp256k1_v0_8_1_ge_set_infinity(r);
        return;
    }
    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_inv_var(&a->z, &a->z);
    rustsecp256k1_v0_8_1_fe_sqr(&z2, &a->z);
    rustsecp256k1_v0_8_1_fe_mul(&z3, &a->z, &z2);
    rustsecp256k1_v0_8_1_fe_mul(&a->x, &a->x, &z2);
    rustsecp256k1_v0_8_1_fe_mul(&a->y, &a->y, &z3);
    rustsecp256k1_v0_8_1_fe_set_int(&a->z, 1);
    rustsecp256k1_v0_8_1_ge_set_xy(r, &a->x, &a->y);
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_set_all_gej_var(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_gej *a, size_t len) {
    rustsecp256k1_v0_8_1_fe u;
    size_t i;
    size_t last_i = SIZE_MAX;

    for (i = 0; i < len; i++) {
        rustsecp256k1_v0_8_1_gej_verify(&a[i]);
        if (a[i].infinity) {
            rustsecp256k1_v0_8_1_ge_set_infinity(&r[i]);
        } else {
            /* Use destination's x coordinates as scratch space */
            if (last_i == SIZE_MAX) {
                r[i].x = a[i].z;
            } else {
                rustsecp256k1_v0_8_1_fe_mul(&r[i].x, &r[last_i].x, &a[i].z);
            }
            last_i = i;
        }
    }
    if (last_i == SIZE_MAX) {
        return;
    }
    rustsecp256k1_v0_8_1_fe_inv_var(&u, &r[last_i].x);

    i = last_i;
    while (i > 0) {
        i--;
        if (!a[i].infinity) {
            rustsecp256k1_v0_8_1_fe_mul(&r[last_i].x, &r[i].x, &u);
            rustsecp256k1_v0_8_1_fe_mul(&u, &u, &a[last_i].z);
            last_i = i;
        }
    }
    VERIFY_CHECK(!a[last_i].infinity);
    r[last_i].x = u;

    for (i = 0; i < len; i++) {
        if (!a[i].infinity) {
            rustsecp256k1_v0_8_1_ge_set_gej_zinv(&r[i], &a[i], &r[i].x);
        }
        rustsecp256k1_v0_8_1_ge_verify(&r[i]);
    }
}

static void rustsecp256k1_v0_8_1_ge_table_set_globalz(size_t len, rustsecp256k1_v0_8_1_ge *a, const rustsecp256k1_v0_8_1_fe *zr) {
    size_t i = len - 1;
    rustsecp256k1_v0_8_1_fe zs;

    if (len > 0) {
        /* Verify inputs a[len-1] and zr[len-1]. */
        rustsecp256k1_v0_8_1_ge_verify(&a[i]);
        rustsecp256k1_v0_8_1_fe_verify(&zr[i]);
        /* Ensure all y values are in weak normal form for fast negation of points */
        rustsecp256k1_v0_8_1_fe_normalize_weak(&a[i].y);
        zs = zr[i];

        /* Work our way backwards, using the z-ratios to scale the x/y values. */
        while (i > 0) {
            /* Verify all inputs a[i] and zr[i]. */
            rustsecp256k1_v0_8_1_fe_verify(&zr[i]);
            rustsecp256k1_v0_8_1_ge_verify(&a[i]);
            if (i != len - 1) {
                rustsecp256k1_v0_8_1_fe_mul(&zs, &zs, &zr[i]);
            }
            i--;
            rustsecp256k1_v0_8_1_ge_set_ge_zinv(&a[i], &a[i], &zs);
            /* Verify the output a[i]. */
            rustsecp256k1_v0_8_1_ge_verify(&a[i]);
        }
    }
}

static void rustsecp256k1_v0_8_1_gej_set_infinity(rustsecp256k1_v0_8_1_gej *r) {
    r->infinity = 1;
    rustsecp256k1_v0_8_1_fe_clear(&r->x);
    rustsecp256k1_v0_8_1_fe_clear(&r->y);
    rustsecp256k1_v0_8_1_fe_clear(&r->z);
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_set_infinity(rustsecp256k1_v0_8_1_ge *r) {
    r->infinity = 1;
    rustsecp256k1_v0_8_1_fe_clear(&r->x);
    rustsecp256k1_v0_8_1_fe_clear(&r->y);
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static void rustsecp256k1_v0_8_1_gej_clear(rustsecp256k1_v0_8_1_gej *r) {
    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_clear(&r->x);
    rustsecp256k1_v0_8_1_fe_clear(&r->y);
    rustsecp256k1_v0_8_1_fe_clear(&r->z);
}

static void rustsecp256k1_v0_8_1_ge_clear(rustsecp256k1_v0_8_1_ge *r) {
    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_clear(&r->x);
    rustsecp256k1_v0_8_1_fe_clear(&r->y);
}

static int rustsecp256k1_v0_8_1_ge_set_xo_var(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_fe *x, int odd) {
    rustsecp256k1_v0_8_1_fe x2, x3;
    int ret;
    rustsecp256k1_v0_8_1_fe_verify(x);
    r->x = *x;
    rustsecp256k1_v0_8_1_fe_sqr(&x2, x);
    rustsecp256k1_v0_8_1_fe_mul(&x3, x, &x2);
    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_add_int(&x3, SECP256K1_B);
    ret = rustsecp256k1_v0_8_1_fe_sqrt(&r->y, &x3);
    rustsecp256k1_v0_8_1_fe_normalize_var(&r->y);
    if (rustsecp256k1_v0_8_1_fe_is_odd(&r->y) != odd) {
        rustsecp256k1_v0_8_1_fe_negate(&r->y, &r->y, 1);
    }
    rustsecp256k1_v0_8_1_ge_verify(r);
    return ret;
}

static void rustsecp256k1_v0_8_1_gej_set_ge(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_ge *a) {
   rustsecp256k1_v0_8_1_ge_verify(a);
   r->infinity = a->infinity;
   r->x = a->x;
   r->y = a->y;
   rustsecp256k1_v0_8_1_fe_set_int(&r->z, 1);
   rustsecp256k1_v0_8_1_gej_verify(r);
}

static int rustsecp256k1_v0_8_1_gej_eq_var(const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_gej *b) {
    rustsecp256k1_v0_8_1_gej tmp;
    rustsecp256k1_v0_8_1_gej_verify(b);
    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_gej_neg(&tmp, a);
    rustsecp256k1_v0_8_1_gej_add_var(&tmp, &tmp, b, NULL);
    return rustsecp256k1_v0_8_1_gej_is_infinity(&tmp);
}

static int rustsecp256k1_v0_8_1_gej_eq_x_var(const rustsecp256k1_v0_8_1_fe *x, const rustsecp256k1_v0_8_1_gej *a) {
    rustsecp256k1_v0_8_1_fe r, r2;
    rustsecp256k1_v0_8_1_fe_verify(x);
    rustsecp256k1_v0_8_1_gej_verify(a);
    VERIFY_CHECK(!a->infinity);
    rustsecp256k1_v0_8_1_fe_sqr(&r, &a->z); rustsecp256k1_v0_8_1_fe_mul(&r, &r, x);
    r2 = a->x; rustsecp256k1_v0_8_1_fe_normalize_weak(&r2);
    return rustsecp256k1_v0_8_1_fe_equal_var(&r, &r2);
}

static void rustsecp256k1_v0_8_1_gej_neg(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a) {
    rustsecp256k1_v0_8_1_gej_verify(a);
    r->infinity = a->infinity;
    r->x = a->x;
    r->y = a->y;
    r->z = a->z;
    rustsecp256k1_v0_8_1_fe_normalize_weak(&r->y);
    rustsecp256k1_v0_8_1_fe_negate(&r->y, &r->y, 1);
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static int rustsecp256k1_v0_8_1_gej_is_infinity(const rustsecp256k1_v0_8_1_gej *a) {
    rustsecp256k1_v0_8_1_gej_verify(a);
    return a->infinity;
}

static int rustsecp256k1_v0_8_1_ge_is_valid_var(const rustsecp256k1_v0_8_1_ge *a) {
    rustsecp256k1_v0_8_1_fe y2, x3;
    rustsecp256k1_v0_8_1_ge_verify(a);
    if (a->infinity) {
        return 0;
    }
    /* y^2 = x^3 + 7 */
    rustsecp256k1_v0_8_1_fe_sqr(&y2, &a->y);
    rustsecp256k1_v0_8_1_fe_sqr(&x3, &a->x); rustsecp256k1_v0_8_1_fe_mul(&x3, &x3, &a->x);
    rustsecp256k1_v0_8_1_fe_add_int(&x3, SECP256K1_B);
    rustsecp256k1_v0_8_1_fe_normalize_weak(&x3);
    return rustsecp256k1_v0_8_1_fe_equal_var(&y2, &x3);
}

static SECP256K1_INLINE void rustsecp256k1_v0_8_1_gej_double(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a) {
    /* Operations: 3 mul, 4 sqr, 8 add/half/mul_int/negate */
    rustsecp256k1_v0_8_1_fe l, s, t;

    rustsecp256k1_v0_8_1_gej_verify(a);
    r->infinity = a->infinity;

    /* Formula used:
     * L = (3/2) * X1^2
     * S = Y1^2
     * T = -X1*S
     * X3 = L^2 + 2*T
     * Y3 = -(L*(X3 + T) + S^2)
     * Z3 = Y1*Z1
     */

    rustsecp256k1_v0_8_1_fe_mul(&r->z, &a->z, &a->y); /* Z3 = Y1*Z1 (1) */
    rustsecp256k1_v0_8_1_fe_sqr(&s, &a->y);           /* S = Y1^2 (1) */
    rustsecp256k1_v0_8_1_fe_sqr(&l, &a->x);           /* L = X1^2 (1) */
    rustsecp256k1_v0_8_1_fe_mul_int(&l, 3);           /* L = 3*X1^2 (3) */
    rustsecp256k1_v0_8_1_fe_half(&l);                 /* L = 3/2*X1^2 (2) */
    rustsecp256k1_v0_8_1_fe_negate(&t, &s, 1);        /* T = -S (2) */
    rustsecp256k1_v0_8_1_fe_mul(&t, &t, &a->x);       /* T = -X1*S (1) */
    rustsecp256k1_v0_8_1_fe_sqr(&r->x, &l);           /* X3 = L^2 (1) */
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);           /* X3 = L^2 + T (2) */
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);           /* X3 = L^2 + 2*T (3) */
    rustsecp256k1_v0_8_1_fe_sqr(&s, &s);              /* S' = S^2 (1) */
    rustsecp256k1_v0_8_1_fe_add(&t, &r->x);           /* T' = X3 + T (4) */
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &t, &l);       /* Y3 = L*(X3 + T) (1) */
    rustsecp256k1_v0_8_1_fe_add(&r->y, &s);           /* Y3 = L*(X3 + T) + S^2 (2) */
    rustsecp256k1_v0_8_1_fe_negate(&r->y, &r->y, 2);  /* Y3 = -(L*(X3 + T) + S^2) (3) */
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_gej_double_var(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, rustsecp256k1_v0_8_1_fe *rzr) {
    /** For secp256k1, 2Q is infinity if and only if Q is infinity. This is because if 2Q = infinity,
     *  Q must equal -Q, or that Q.y == -(Q.y), or Q.y is 0. For a point on y^2 = x^3 + 7 to have
     *  y=0, x^3 must be -7 mod p. However, -7 has no cube root mod p.
     *
     *  Having said this, if this function receives a point on a sextic twist, e.g. by
     *  a fault attack, it is possible for y to be 0. This happens for y^2 = x^3 + 6,
     *  since -6 does have a cube root mod p. For this point, this function will not set
     *  the infinity flag even though the point doubles to infinity, and the result
     *  point will be gibberish (z = 0 but infinity = 0).
     */
    rustsecp256k1_v0_8_1_gej_verify(a);
    if (a->infinity) {
        rustsecp256k1_v0_8_1_gej_set_infinity(r);
        if (rzr != NULL) {
            rustsecp256k1_v0_8_1_fe_set_int(rzr, 1);
        }
        return;
    }

    if (rzr != NULL) {
        *rzr = a->y;
        rustsecp256k1_v0_8_1_fe_normalize_weak(rzr);
    }

    rustsecp256k1_v0_8_1_gej_double(r, a);
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_gej_add_var(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_gej *b, rustsecp256k1_v0_8_1_fe *rzr) {
    /* 12 mul, 4 sqr, 11 add/negate/normalizes_to_zero (ignoring special cases) */
    rustsecp256k1_v0_8_1_fe z22, z12, u1, u2, s1, s2, h, i, h2, h3, t;

    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_gej_verify(b);
    if (a->infinity) {
        VERIFY_CHECK(rzr == NULL);
        *r = *b;
        return;
    }
    if (b->infinity) {
        if (rzr != NULL) {
            rustsecp256k1_v0_8_1_fe_set_int(rzr, 1);
        }
        *r = *a;
        return;
    }

    rustsecp256k1_v0_8_1_fe_sqr(&z22, &b->z);
    rustsecp256k1_v0_8_1_fe_sqr(&z12, &a->z);
    rustsecp256k1_v0_8_1_fe_mul(&u1, &a->x, &z22);
    rustsecp256k1_v0_8_1_fe_mul(&u2, &b->x, &z12);
    rustsecp256k1_v0_8_1_fe_mul(&s1, &a->y, &z22); rustsecp256k1_v0_8_1_fe_mul(&s1, &s1, &b->z);
    rustsecp256k1_v0_8_1_fe_mul(&s2, &b->y, &z12); rustsecp256k1_v0_8_1_fe_mul(&s2, &s2, &a->z);
    rustsecp256k1_v0_8_1_fe_negate(&h, &u1, 1); rustsecp256k1_v0_8_1_fe_add(&h, &u2);
    rustsecp256k1_v0_8_1_fe_negate(&i, &s2, 1); rustsecp256k1_v0_8_1_fe_add(&i, &s1);
    if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&h)) {
        if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&i)) {
            rustsecp256k1_v0_8_1_gej_double_var(r, a, rzr);
        } else {
            if (rzr != NULL) {
                rustsecp256k1_v0_8_1_fe_set_int(rzr, 0);
            }
            rustsecp256k1_v0_8_1_gej_set_infinity(r);
        }
        return;
    }

    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_mul(&t, &h, &b->z);
    if (rzr != NULL) {
        *rzr = t;
    }
    rustsecp256k1_v0_8_1_fe_mul(&r->z, &a->z, &t);

    rustsecp256k1_v0_8_1_fe_sqr(&h2, &h);
    rustsecp256k1_v0_8_1_fe_negate(&h2, &h2, 1);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h2, &h);
    rustsecp256k1_v0_8_1_fe_mul(&t, &u1, &h2);

    rustsecp256k1_v0_8_1_fe_sqr(&r->x, &i);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &h3);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);

    rustsecp256k1_v0_8_1_fe_add(&t, &r->x);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &t, &i);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h3, &s1);
    rustsecp256k1_v0_8_1_fe_add(&r->y, &h3);
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_gej_add_ge_var(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_ge *b, rustsecp256k1_v0_8_1_fe *rzr) {
    /* 8 mul, 3 sqr, 13 add/negate/normalize_weak/normalizes_to_zero (ignoring special cases) */
    rustsecp256k1_v0_8_1_fe z12, u1, u2, s1, s2, h, i, h2, h3, t;
    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_ge_verify(b);
    if (a->infinity) {
        VERIFY_CHECK(rzr == NULL);
        rustsecp256k1_v0_8_1_gej_set_ge(r, b);
        return;
    }
    if (b->infinity) {
        if (rzr != NULL) {
            rustsecp256k1_v0_8_1_fe_set_int(rzr, 1);
        }
        *r = *a;
        return;
    }

    rustsecp256k1_v0_8_1_fe_sqr(&z12, &a->z);
    u1 = a->x; rustsecp256k1_v0_8_1_fe_normalize_weak(&u1);
    rustsecp256k1_v0_8_1_fe_mul(&u2, &b->x, &z12);
    s1 = a->y; rustsecp256k1_v0_8_1_fe_normalize_weak(&s1);
    rustsecp256k1_v0_8_1_fe_mul(&s2, &b->y, &z12); rustsecp256k1_v0_8_1_fe_mul(&s2, &s2, &a->z);
    rustsecp256k1_v0_8_1_fe_negate(&h, &u1, 1); rustsecp256k1_v0_8_1_fe_add(&h, &u2);
    rustsecp256k1_v0_8_1_fe_negate(&i, &s2, 1); rustsecp256k1_v0_8_1_fe_add(&i, &s1);
    if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&h)) {
        if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&i)) {
            rustsecp256k1_v0_8_1_gej_double_var(r, a, rzr);
        } else {
            if (rzr != NULL) {
                rustsecp256k1_v0_8_1_fe_set_int(rzr, 0);
            }
            rustsecp256k1_v0_8_1_gej_set_infinity(r);
        }
        return;
    }

    r->infinity = 0;
    if (rzr != NULL) {
        *rzr = h;
    }
    rustsecp256k1_v0_8_1_fe_mul(&r->z, &a->z, &h);

    rustsecp256k1_v0_8_1_fe_sqr(&h2, &h);
    rustsecp256k1_v0_8_1_fe_negate(&h2, &h2, 1);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h2, &h);
    rustsecp256k1_v0_8_1_fe_mul(&t, &u1, &h2);

    rustsecp256k1_v0_8_1_fe_sqr(&r->x, &i);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &h3);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);

    rustsecp256k1_v0_8_1_fe_add(&t, &r->x);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &t, &i);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h3, &s1);
    rustsecp256k1_v0_8_1_fe_add(&r->y, &h3);
    rustsecp256k1_v0_8_1_gej_verify(r);
    if (rzr != NULL) rustsecp256k1_v0_8_1_fe_verify(rzr);
}

static void rustsecp256k1_v0_8_1_gej_add_zinv_var(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_ge *b, const rustsecp256k1_v0_8_1_fe *bzinv) {
    /* 9 mul, 3 sqr, 13 add/negate/normalize_weak/normalizes_to_zero (ignoring special cases) */
    rustsecp256k1_v0_8_1_fe az, z12, u1, u2, s1, s2, h, i, h2, h3, t;

    rustsecp256k1_v0_8_1_ge_verify(b);
    rustsecp256k1_v0_8_1_fe_verify(bzinv);
    if (a->infinity) {
        rustsecp256k1_v0_8_1_fe bzinv2, bzinv3;
        r->infinity = b->infinity;
        rustsecp256k1_v0_8_1_fe_sqr(&bzinv2, bzinv);
        rustsecp256k1_v0_8_1_fe_mul(&bzinv3, &bzinv2, bzinv);
        rustsecp256k1_v0_8_1_fe_mul(&r->x, &b->x, &bzinv2);
        rustsecp256k1_v0_8_1_fe_mul(&r->y, &b->y, &bzinv3);
        rustsecp256k1_v0_8_1_fe_set_int(&r->z, 1);
        return;
    }
    if (b->infinity) {
        *r = *a;
        return;
    }

    /** We need to calculate (rx,ry,rz) = (ax,ay,az) + (bx,by,1/bzinv). Due to
     *  secp256k1's isomorphism we can multiply the Z coordinates on both sides
     *  by bzinv, and get: (rx,ry,rz*bzinv) = (ax,ay,az*bzinv) + (bx,by,1).
     *  This means that (rx,ry,rz) can be calculated as
     *  (ax,ay,az*bzinv) + (bx,by,1), when not applying the bzinv factor to rz.
     *  The variable az below holds the modified Z coordinate for a, which is used
     *  for the computation of rx and ry, but not for rz.
     */
    rustsecp256k1_v0_8_1_fe_mul(&az, &a->z, bzinv);

    rustsecp256k1_v0_8_1_fe_sqr(&z12, &az);
    u1 = a->x; rustsecp256k1_v0_8_1_fe_normalize_weak(&u1);
    rustsecp256k1_v0_8_1_fe_mul(&u2, &b->x, &z12);
    s1 = a->y; rustsecp256k1_v0_8_1_fe_normalize_weak(&s1);
    rustsecp256k1_v0_8_1_fe_mul(&s2, &b->y, &z12); rustsecp256k1_v0_8_1_fe_mul(&s2, &s2, &az);
    rustsecp256k1_v0_8_1_fe_negate(&h, &u1, 1); rustsecp256k1_v0_8_1_fe_add(&h, &u2);
    rustsecp256k1_v0_8_1_fe_negate(&i, &s2, 1); rustsecp256k1_v0_8_1_fe_add(&i, &s1);
    if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&h)) {
        if (rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(&i)) {
            rustsecp256k1_v0_8_1_gej_double_var(r, a, NULL);
        } else {
            rustsecp256k1_v0_8_1_gej_set_infinity(r);
        }
        return;
    }

    r->infinity = 0;
    rustsecp256k1_v0_8_1_fe_mul(&r->z, &a->z, &h);

    rustsecp256k1_v0_8_1_fe_sqr(&h2, &h);
    rustsecp256k1_v0_8_1_fe_negate(&h2, &h2, 1);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h2, &h);
    rustsecp256k1_v0_8_1_fe_mul(&t, &u1, &h2);

    rustsecp256k1_v0_8_1_fe_sqr(&r->x, &i);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &h3);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);
    rustsecp256k1_v0_8_1_fe_add(&r->x, &t);

    rustsecp256k1_v0_8_1_fe_add(&t, &r->x);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &t, &i);
    rustsecp256k1_v0_8_1_fe_mul(&h3, &h3, &s1);
    rustsecp256k1_v0_8_1_fe_add(&r->y, &h3);
    rustsecp256k1_v0_8_1_gej_verify(r);
}


static void rustsecp256k1_v0_8_1_gej_add_ge(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, const rustsecp256k1_v0_8_1_ge *b) {
    /* Operations: 7 mul, 5 sqr, 24 add/cmov/half/mul_int/negate/normalize_weak/normalizes_to_zero */
    rustsecp256k1_v0_8_1_fe zz, u1, u2, s1, s2, t, tt, m, n, q, rr;
    rustsecp256k1_v0_8_1_fe m_alt, rr_alt;
    int degenerate;
    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_ge_verify(b);
    VERIFY_CHECK(!b->infinity);
    VERIFY_CHECK(a->infinity == 0 || a->infinity == 1);

    /*  In:
     *    Eric Brier and Marc Joye, Weierstrass Elliptic Curves and Side-Channel Attacks.
     *    In D. Naccache and P. Paillier, Eds., Public Key Cryptography, vol. 2274 of Lecture Notes in Computer Science, pages 335-345. Springer-Verlag, 2002.
     *  we find as solution for a unified addition/doubling formula:
     *    lambda = ((x1 + x2)^2 - x1 * x2 + a) / (y1 + y2), with a = 0 for secp256k1's curve equation.
     *    x3 = lambda^2 - (x1 + x2)
     *    2*y3 = lambda * (x1 + x2 - 2 * x3) - (y1 + y2).
     *
     *  Substituting x_i = Xi / Zi^2 and yi = Yi / Zi^3, for i=1,2,3, gives:
     *    U1 = X1*Z2^2, U2 = X2*Z1^2
     *    S1 = Y1*Z2^3, S2 = Y2*Z1^3
     *    Z = Z1*Z2
     *    T = U1+U2
     *    M = S1+S2
     *    Q = -T*M^2
     *    R = T^2-U1*U2
     *    X3 = R^2+Q
     *    Y3 = -(R*(2*X3+Q)+M^4)/2
     *    Z3 = M*Z
     *  (Note that the paper uses xi = Xi / Zi and yi = Yi / Zi instead.)
     *
     *  This formula has the benefit of being the same for both addition
     *  of distinct points and doubling. However, it breaks down in the
     *  case that either point is infinity, or that y1 = -y2. We handle
     *  these cases in the following ways:
     *
     *    - If b is infinity we simply bail by means of a VERIFY_CHECK.
     *
     *    - If a is infinity, we detect this, and at the end of the
     *      computation replace the result (which will be meaningless,
     *      but we compute to be constant-time) with b.x : b.y : 1.
     *
     *    - If a = -b, we have y1 = -y2, which is a degenerate case.
     *      But here the answer is infinity, so we simply set the
     *      infinity flag of the result, overriding the computed values
     *      without even needing to cmov.
     *
     *    - If y1 = -y2 but x1 != x2, which does occur thanks to certain
     *      properties of our curve (specifically, 1 has nontrivial cube
     *      roots in our field, and the curve equation has no x coefficient)
     *      then the answer is not infinity but also not given by the above
     *      equation. In this case, we cmov in place an alternate expression
     *      for lambda. Specifically (y1 - y2)/(x1 - x2). Where both these
     *      expressions for lambda are defined, they are equal, and can be
     *      obtained from each other by multiplication by (y1 + y2)/(y1 + y2)
     *      then substitution of x^3 + 7 for y^2 (using the curve equation).
     *      For all pairs of nonzero points (a, b) at least one is defined,
     *      so this covers everything.
     */

    rustsecp256k1_v0_8_1_fe_sqr(&zz, &a->z);                       /* z = Z1^2 */
    u1 = a->x; rustsecp256k1_v0_8_1_fe_normalize_weak(&u1);        /* u1 = U1 = X1*Z2^2 (1) */
    rustsecp256k1_v0_8_1_fe_mul(&u2, &b->x, &zz);                  /* u2 = U2 = X2*Z1^2 (1) */
    s1 = a->y; rustsecp256k1_v0_8_1_fe_normalize_weak(&s1);        /* s1 = S1 = Y1*Z2^3 (1) */
    rustsecp256k1_v0_8_1_fe_mul(&s2, &b->y, &zz);                  /* s2 = Y2*Z1^2 (1) */
    rustsecp256k1_v0_8_1_fe_mul(&s2, &s2, &a->z);                  /* s2 = S2 = Y2*Z1^3 (1) */
    t = u1; rustsecp256k1_v0_8_1_fe_add(&t, &u2);                  /* t = T = U1+U2 (2) */
    m = s1; rustsecp256k1_v0_8_1_fe_add(&m, &s2);                  /* m = M = S1+S2 (2) */
    rustsecp256k1_v0_8_1_fe_sqr(&rr, &t);                          /* rr = T^2 (1) */
    rustsecp256k1_v0_8_1_fe_negate(&m_alt, &u2, 1);                /* Malt = -X2*Z1^2 */
    rustsecp256k1_v0_8_1_fe_mul(&tt, &u1, &m_alt);                 /* tt = -U1*U2 (2) */
    rustsecp256k1_v0_8_1_fe_add(&rr, &tt);                         /* rr = R = T^2-U1*U2 (3) */
    /* If lambda = R/M = R/0 we have a problem (except in the "trivial"
     * case that Z = z1z2 = 0, and this is special-cased later on). */
    degenerate = rustsecp256k1_v0_8_1_fe_normalizes_to_zero(&m);
    /* This only occurs when y1 == -y2 and x1^3 == x2^3, but x1 != x2.
     * This means either x1 == beta*x2 or beta*x1 == x2, where beta is
     * a nontrivial cube root of one. In either case, an alternate
     * non-indeterminate expression for lambda is (y1 - y2)/(x1 - x2),
     * so we set R/M equal to this. */
    rr_alt = s1;
    rustsecp256k1_v0_8_1_fe_mul_int(&rr_alt, 2);       /* rr = Y1*Z2^3 - Y2*Z1^3 (2) */
    rustsecp256k1_v0_8_1_fe_add(&m_alt, &u1);          /* Malt = X1*Z2^2 - X2*Z1^2 */

    rustsecp256k1_v0_8_1_fe_cmov(&rr_alt, &rr, !degenerate);
    rustsecp256k1_v0_8_1_fe_cmov(&m_alt, &m, !degenerate);
    /* Now Ralt / Malt = lambda and is guaranteed not to be Ralt / 0.
     * From here on out Ralt and Malt represent the numerator
     * and denominator of lambda; R and M represent the explicit
     * expressions x1^2 + x2^2 + x1x2 and y1 + y2. */
    rustsecp256k1_v0_8_1_fe_sqr(&n, &m_alt);                       /* n = Malt^2 (1) */
    rustsecp256k1_v0_8_1_fe_negate(&q, &t, 2);                     /* q = -T (3) */
    rustsecp256k1_v0_8_1_fe_mul(&q, &q, &n);                       /* q = Q = -T*Malt^2 (1) */
    /* These two lines use the observation that either M == Malt or M == 0,
     * so M^3 * Malt is either Malt^4 (which is computed by squaring), or
     * zero (which is "computed" by cmov). So the cost is one squaring
     * versus two multiplications. */
    rustsecp256k1_v0_8_1_fe_sqr(&n, &n);
    rustsecp256k1_v0_8_1_fe_cmov(&n, &m, degenerate);              /* n = M^3 * Malt (2) */
    rustsecp256k1_v0_8_1_fe_sqr(&t, &rr_alt);                      /* t = Ralt^2 (1) */
    rustsecp256k1_v0_8_1_fe_mul(&r->z, &a->z, &m_alt);             /* r->z = Z3 = Malt*Z (1) */
    rustsecp256k1_v0_8_1_fe_add(&t, &q);                           /* t = Ralt^2 + Q (2) */
    r->x = t;                                           /* r->x = X3 = Ralt^2 + Q (2) */
    rustsecp256k1_v0_8_1_fe_mul_int(&t, 2);                        /* t = 2*X3 (4) */
    rustsecp256k1_v0_8_1_fe_add(&t, &q);                           /* t = 2*X3 + Q (5) */
    rustsecp256k1_v0_8_1_fe_mul(&t, &t, &rr_alt);                  /* t = Ralt*(2*X3 + Q) (1) */
    rustsecp256k1_v0_8_1_fe_add(&t, &n);                           /* t = Ralt*(2*X3 + Q) + M^3*Malt (3) */
    rustsecp256k1_v0_8_1_fe_negate(&r->y, &t, 3);                  /* r->y = -(Ralt*(2*X3 + Q) + M^3*Malt) (4) */
    rustsecp256k1_v0_8_1_fe_half(&r->y);                           /* r->y = Y3 = -(Ralt*(2*X3 + Q) + M^3*Malt)/2 (3) */

    /* In case a->infinity == 1, replace r with (b->x, b->y, 1). */
    rustsecp256k1_v0_8_1_fe_cmov(&r->x, &b->x, a->infinity);
    rustsecp256k1_v0_8_1_fe_cmov(&r->y, &b->y, a->infinity);
    rustsecp256k1_v0_8_1_fe_cmov(&r->z, &rustsecp256k1_v0_8_1_fe_one, a->infinity);

    /* Set r->infinity if r->z is 0.
     *
     * If a->infinity is set, then r->infinity = (r->z == 0) = (1 == 0) = false,
     * which is correct because the function assumes that b is not infinity.
     *
     * Now assume !a->infinity. This implies Z = Z1 != 0.
     *
     * Case y1 = -y2:
     * In this case we could have a = -b, namely if x1 = x2.
     * We have degenerate = true, r->z = (x1 - x2) * Z.
     * Then r->infinity = ((x1 - x2)Z == 0) = (x1 == x2) = (a == -b).
     *
     * Case y1 != -y2:
     * In this case, we can't have a = -b.
     * We have degenerate = false, r->z = (y1 + y2) * Z.
     * Then r->infinity = ((y1 + y2)Z == 0) = (y1 == -y2) = false. */
    r->infinity = rustsecp256k1_v0_8_1_fe_normalizes_to_zero(&r->z);
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_gej_rescale(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_fe *s) {
    /* Operations: 4 mul, 1 sqr */
    rustsecp256k1_v0_8_1_fe zz;
    rustsecp256k1_v0_8_1_gej_verify(r);
    rustsecp256k1_v0_8_1_fe_verify(s);
#ifdef VERIFY
    VERIFY_CHECK(!rustsecp256k1_v0_8_1_fe_normalizes_to_zero_var(s));
#endif
    rustsecp256k1_v0_8_1_fe_sqr(&zz, s);
    rustsecp256k1_v0_8_1_fe_mul(&r->x, &r->x, &zz);                /* r->x *= s^2 */
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &r->y, &zz);
    rustsecp256k1_v0_8_1_fe_mul(&r->y, &r->y, s);                  /* r->y *= s^3 */
    rustsecp256k1_v0_8_1_fe_mul(&r->z, &r->z, s);                  /* r->z *= s   */
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static void rustsecp256k1_v0_8_1_ge_to_storage(rustsecp256k1_v0_8_1_ge_storage *r, const rustsecp256k1_v0_8_1_ge *a) {
    rustsecp256k1_v0_8_1_fe x, y;
    rustsecp256k1_v0_8_1_ge_verify(a);
    VERIFY_CHECK(!a->infinity);
    x = a->x;
    rustsecp256k1_v0_8_1_fe_normalize(&x);
    y = a->y;
    rustsecp256k1_v0_8_1_fe_normalize(&y);
    rustsecp256k1_v0_8_1_fe_to_storage(&r->x, &x);
    rustsecp256k1_v0_8_1_fe_to_storage(&r->y, &y);
}

static void rustsecp256k1_v0_8_1_ge_from_storage(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_ge_storage *a) {
    rustsecp256k1_v0_8_1_fe_from_storage(&r->x, &a->x);
    rustsecp256k1_v0_8_1_fe_from_storage(&r->y, &a->y);
    r->infinity = 0;
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static SECP256K1_INLINE void rustsecp256k1_v0_8_1_gej_cmov(rustsecp256k1_v0_8_1_gej *r, const rustsecp256k1_v0_8_1_gej *a, int flag) {
    rustsecp256k1_v0_8_1_gej_verify(r);
    rustsecp256k1_v0_8_1_gej_verify(a);
    rustsecp256k1_v0_8_1_fe_cmov(&r->x, &a->x, flag);
    rustsecp256k1_v0_8_1_fe_cmov(&r->y, &a->y, flag);
    rustsecp256k1_v0_8_1_fe_cmov(&r->z, &a->z, flag);

    r->infinity ^= (r->infinity ^ a->infinity) & flag;
    rustsecp256k1_v0_8_1_gej_verify(r);
}

static SECP256K1_INLINE void rustsecp256k1_v0_8_1_ge_storage_cmov(rustsecp256k1_v0_8_1_ge_storage *r, const rustsecp256k1_v0_8_1_ge_storage *a, int flag) {
    rustsecp256k1_v0_8_1_fe_storage_cmov(&r->x, &a->x, flag);
    rustsecp256k1_v0_8_1_fe_storage_cmov(&r->y, &a->y, flag);
}

static void rustsecp256k1_v0_8_1_ge_mul_lambda(rustsecp256k1_v0_8_1_ge *r, const rustsecp256k1_v0_8_1_ge *a) {
    *r = *a;
    rustsecp256k1_v0_8_1_ge_verify(a);
    rustsecp256k1_v0_8_1_fe_mul(&r->x, &r->x, &rustsecp256k1_v0_8_1_const_beta);
    rustsecp256k1_v0_8_1_ge_verify(r);
}

static int rustsecp256k1_v0_8_1_ge_is_in_correct_subgroup(const rustsecp256k1_v0_8_1_ge* ge) {
#ifdef EXHAUSTIVE_TEST_ORDER
    rustsecp256k1_v0_8_1_gej out;
    int i;

    rustsecp256k1_v0_8_1_ge_verify(ge);
    /* A very simple EC multiplication ladder that avoids a dependency on ecmult. */
    rustsecp256k1_v0_8_1_gej_set_infinity(&out);
    for (i = 0; i < 32; ++i) {
        rustsecp256k1_v0_8_1_gej_double_var(&out, &out, NULL);
        if ((((uint32_t)EXHAUSTIVE_TEST_ORDER) >> (31 - i)) & 1) {
            rustsecp256k1_v0_8_1_gej_add_ge_var(&out, &out, ge, NULL);
        }
    }
    return rustsecp256k1_v0_8_1_gej_is_infinity(&out);
#else
    (void)ge;
    /* The real secp256k1 group has cofactor 1, so the subgroup is the entire curve. */
    return 1;
#endif
}

#endif /* SECP256K1_GROUP_IMPL_H */
