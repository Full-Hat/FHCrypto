#include <math.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

typedef struct point {
  mpz_t x, y;
} point;


typedef struct elliptic_curve {
  mpz_t a, b, p;
  point *base;
} elliptic_curve;

typedef struct elgam_ctx {
  mpz_t dom_par_q, dom_par_p, dom_par_g;
  mpz_t priv_x, pub_h, eph_k;
} elgam_ctx;

typedef struct ciphertext {
  mpz_t c1, c2;
} ciphertext;

typedef struct cipherec {
	point *c1, *c2;
} cipherec;

typedef struct elgam_ec_ctx {
  mpz_t priv_key, eph_k;
  point *pub_key;
  elliptic_curve *ec;
} elgam_ec_ctx;

void init_point(point **);
void destroy_point(point *);

point* ecc_scalar_mul(elliptic_curve *, mpz_t, point *);
point* ecc_addition(elliptic_curve *, point *, point *);
point* ecc_doubling(elliptic_curve *, point *);

void init_point(point **p) 
{
	*p = (point*) malloc(sizeof(point));
	mpz_init((*p)->x); //set value to 0
	mpz_init((*p)->y);
}


void destroy_point(point *p)
{
	if (p) {
		mpz_clears(p->x, p->y, NULL);
		free(p);
		p = NULL;
	}
}

point* ecc_scalar_mul(elliptic_curve *ec, mpz_t m, point *p) {
	if (mpz_cmp_ui(m, 1) == 0) { // if equal retrun the number itself.
		return p;
	} else if (mpz_even_p(m) == 0) { // return true/1 if even
		mpz_sub_ui(m, m, 1); // m = m-1
		return ecc_addition(ec, p, ecc_scalar_mul(ec,m,p));
	} else {
		mpz_div_ui(m, m, 2); // m = m/2
		return ecc_scalar_mul(ec, m, ecc_doubling(ec, p));
	}
}

point* ecc_doubling(elliptic_curve *ec, point *p)
{
	// https://crypto.stackexchange.com/questions/64456/problem-on-elliptic-curve-point-doubling
	point *r = malloc(sizeof(point));
	mpz_init(r->x);
	mpz_init(r->y);
	mpz_mod(p->x, p->x, ec->p);
	mpz_mod(p->y, p->y, ec->p);
	mpz_t temp, slope;
	mpz_init(temp);
	mpz_init_set_ui(slope, 0);

	// temp = 2*y1
	mpz_mul_ui(temp, p->y, 2);
	// temp = temp^-1 mod p
	mpz_invert(temp, temp, ec->p);
	// slope = x1*x1 = x1^2
	mpz_mul(slope, p->x, p->x);
	// slope = slope * 3
	mpz_mul_ui(slope, slope, 3);

	// slope = slope + a
	mpz_add(slope, slope, ec->a);

	// slope = slope * temp (numinator * denuminator)
	mpz_mul(slope, slope, temp);
	// slope = slope mod p
	mpz_mod(slope, slope, ec->p);

	// x3 = slope * slope
	mpz_mul(r->x, slope, slope);
	mpz_sub(r->x, r->x, p->x);
	mpz_sub(r->x, r->x, p->x);
	mpz_mod(r->x, r->x, ec->p);
	mpz_sub(temp, p->x, r->x);
	mpz_mul(r->y, slope, temp);
	mpz_sub(r->y, r->y, p->y);
	mpz_mod(r->y, r->y, ec->p);

	//return r;
	mpz_clears(temp, slope, NULL);
	return r;
}

point* ecc_addition(elliptic_curve *ec, point *p, point *q) // ec->p is prime
{
	point *r = malloc(sizeof(point));
	mpz_init(r->x);
	mpz_init(r->y);
	mpz_mod(p->x, p->x, ec->p); //1st = 2nd % 3rd
	mpz_mod(p->y, p->y, ec->p);
	mpz_mod(q->x, q->x, ec->p);
	mpz_mod(q->y, q->y, ec->p);
	mpz_t temp,slope;
	mpz_init(temp);
	mpz_init_set_ui(slope, 0);

	//if (mpz_cmp(p->x, q->x) == 0 && mpz_cmp(p->y, q->y) == 0)
	//  return ecc_doubling(ec, p);

	// temp = x1-x2
	mpz_sub(temp, p->x, q->x);
	// temp = temp mod p
	mpz_mod(temp, temp, ec->p);
	// temp^-1 mod p
	mpz_invert(temp, temp, ec->p);
	// slope = y1-y2
	mpz_sub(slope, p->y, q->y);
	// slope = slope * temp
	mpz_mul(slope, slope, temp);
	// slope = slope mod p
	mpz_mod(slope, slope, ec->p);

	// x3 = slope * slope = alpha^2
	mpz_mul(r->x, slope, slope);

	// x3 = x3 - x1
	mpz_sub(r->x, r->x, p->x);
	// x3 = x3 - x2
	mpz_sub(r->x, r->x, q->x);
	// x3 = x3 mod p
	mpz_mod(r->x, r->x, ec->p);

	// temp = x1 - x3
	mpz_sub(temp, p->x, r->x);
	// y3 = slope * temp
	mpz_mul(r->y, slope, temp);
	// y3 = y3 - y1
	mpz_sub(r->y, r->y, p->y);
	// y3 = y3 mod p
	mpz_mod(r->y, r->y, ec->p);
	//return r;
	mpz_clears(temp, slope, NULL);
	return r;
}

/*
  Sets r to a random GMP integer with the specified number
  of bits.
*/
void get_random_n_bits(mpz_t r, size_t bits)
{
	size_t size = (size_t) ceilf(bits/8);
	char *buffer = (char*) malloc(sizeof(char)*size);
	int prg = open("/dev/random", O_RDONLY);
	read(prg, buffer, size);
	close(prg);
	mpz_import (r, size, 1, sizeof(char), 0, 0, buffer);
	free(buffer);
}


/*
  Sets r to a random GMP *prime* integer, smaller than max.
*/
void get_random_n_prime(mpz_t r, mpz_t max) 
{
	do {
		get_random_n_bits(r, mpz_sizeinbase(max, 2));
		mpz_nextprime(r, r);
	} while (mpz_cmp(r, max) >= 0);
}


/*
  Sets r to a random GMP integer smaller than max.
*/
void get_random_n(mpz_t r, mpz_t max) 
{
	do {
		get_random_n_bits(r, mpz_sizeinbase(max, 2));
	} while (mpz_cmp(r, max) >= 0);
}


/*
 Init structure. Set domain parameters p, q and g
 */
void init_elgam(elgam_ctx **ectx, size_t bits) 
{
	*ectx = (elgam_ctx*) malloc(sizeof(elgam_ctx));
	// 1. find large prime p for domain parameter
	mpz_t p, g, x, h;
	mpz_init((*ectx)->dom_par_p);
	mpz_init((*ectx)->dom_par_g);
	mpz_init((*ectx)->priv_x);
	mpz_init((*ectx)->pub_h);
	mpz_init((*ectx)->eph_k);
	get_random_n_bits((*ectx)->dom_par_p, bits);
	mpz_nextprime((*ectx)->dom_par_p, (*ectx)->dom_par_p);
	gmp_printf("\n\np = %Zd\n", (*ectx)->dom_par_p);

	get_random_n_prime((*ectx)->dom_par_g, (*ectx)->dom_par_p);
	gmp_printf("g = %Zd\n", (*ectx)->dom_par_g);

	get_random_n((*ectx)->priv_x, (*ectx)->dom_par_p);
	gmp_printf("x = %Zd\n", (*ectx)->priv_x);
	/* h = g^x (mod n) */
	mpz_powm_sec((*ectx)->pub_h, (*ectx)->dom_par_g, (*ectx)->priv_x, (*ectx)->dom_par_p);
	gmp_printf("h = %Zd\n\n", (*ectx)->pub_h);
}


void destroy_elgam(elgam_ctx *ectx) 
{
	if (ectx) {
		mpz_clears(ectx->dom_par_p, ectx->dom_par_g, ectx->dom_par_q, NULL);
		mpz_clears(ectx->priv_x, ectx->pub_h, ectx->eph_k, NULL);
		free(ectx);
		ectx = NULL;
	}
}


void destroy_ciphertxt(ciphertext *ct) 
{
	if (ct) {
		mpz_clears(ct->c1, ct->c2, NULL);
		free(ct);
		ct = NULL;
	}
}

ciphertext* encrypt1(mpz_t m, elgam_ctx *ectx)
{
	ectx->eph_k;
	get_random_n(ectx->eph_k, ectx->dom_par_p);
	ciphertext *ct = malloc(sizeof(ciphertext));
	mpz_init(ct->c1);
	mpz_init(ct->c2);
	mpz_powm_sec(ct->c1, ectx->dom_par_g, ectx->eph_k, ectx->dom_par_p);
	mpz_powm_sec(ct->c2, ectx->pub_h, ectx->eph_k, ectx->dom_par_p);
	mpz_mul(ct->c2, m, ct->c2);
	mpz_mod(ct->c2, ct->c2, ectx->dom_par_p);
	gmp_printf("c1 = %Zd\n", ct->c1);
	gmp_printf("c2 = %Zd\n\n", ct->c2);
	return ct;
}


void decrypt(mpz_t msg, ciphertext *ct, elgam_ctx *ectx) 
{
	mpz_powm_sec(ct->c1, ct->c1, ectx->priv_x, ectx->dom_par_p);
	mpz_invert(ct->c1, ct->c1, ectx->dom_par_p);
	mpz_mul(msg, ct->c2, ct->c1);
	mpz_mod(msg, msg, ectx->dom_par_p);
}

/* setup elliptic curve, public and private key
 Using the brainpoolP160r1 - EC domain parameters
 http://www.ecc-brainpool.org/download/Domain-parameters.pdf
 */
void init_elgam_ec(elgam_ec_ctx **eec_ctx)
{
    *eec_ctx = (elgam_ec_ctx*) malloc(sizeof(elgam_ec_ctx));
    elliptic_curve *ecc = malloc(sizeof(elliptic_curve));
    (*eec_ctx)->ec = ecc;

	//Set elliptic curve.
    mpz_set_str(ecc->a, "340E7BE2A280EB74E2BE61BADA745D97E8F7C300", 16); 
    mpz_set_str(ecc->b, "1E589A8595423412134FAA2DBDEC95C8D8675E58", 16); 
	mpz_set_str(ecc->p, "E95E4A5F737059DC60DFC7AD95B3D8139515620F", 16); 

	mpz_init((*eec_ctx)->priv_key);
	init_point(&(ecc->base));
	init_point(&((*eec_ctx)->pub_key));

	// Set the base point.
	mpz_set_str(ecc->base->x, "BED5AF16EA3F6A4F62938C4631EB5AF7BDBCDBC3", 16); 
	mpz_set_str(ecc->base->y, "1667CB477A1A8EC338F94741669C976316DA6321", 16); 
	gmp_printf("\np = %Zd\n", ecc->p);

	// Choose a random private key.
	get_random_n((*eec_ctx)->priv_key, ecc->p);
	gmp_printf("x = %Zd\n", (*eec_ctx)->priv_key);

	mpz_t tmp;
	mpz_init_set(tmp, (*eec_ctx)->priv_key); // private key is stored in tmp
	(*eec_ctx)->pub_key = ecc_scalar_mul((*eec_ctx)->ec, tmp, ecc->base);  //eec_ctx is e2. e2 = d * e1
	mpz_clears(tmp, NULL);
	gmp_printf("Base point P = (%Zd,%Zd)\n", ecc->base->x, ecc->base->y);
	gmp_printf("Public key xP =  (%Zd,%Zd)\n\n", ((*eec_ctx)->pub_key)->x, ((*eec_ctx)->pub_key)->y);
}


void test_init_elgam_ec(elgam_ec_ctx **eec_ctx)
{
    *eec_ctx = (elgam_ec_ctx*) malloc(sizeof(elgam_ec_ctx));
    elliptic_curve *ecc = malloc(sizeof(elliptic_curve));
    (*eec_ctx)->ec = ecc;

	mpz_init_set_ui(ecc->a, 1);
	mpz_init_set_ui(ecc->b, 3);
	mpz_init_set_ui(ecc->p, 23);

	mpz_init((*eec_ctx)->priv_key);
	init_point(&(ecc->base));
	init_point(&((*eec_ctx)->pub_key));

	//mpz_init_set_ui(ecc->base->x, 21);
	//mpz_init_set_ui(ecc->base->y, 1);
}


void destroy_elgam_ec(elgam_ec_ctx *eec_ctx) 
{
	if (eec_ctx) {
 		mpz_clears(eec_ctx->priv_key, eec_ctx->eph_k, NULL);
		mpz_clears(eec_ctx->ec->a, eec_ctx->ec->b, eec_ctx->ec->p, NULL);
		destroy_point(eec_ctx->ec->base);
		destroy_point(eec_ctx->pub_key);
		if (eec_ctx->ec) {
			free(eec_ctx->ec);
			eec_ctx->ec = NULL;
		}
		free(eec_ctx);
		eec_ctx = NULL;
	}
}

void destroy_cipherec(cipherec *c)
{
	if (c) {
		destroy_point(c->c1);
		destroy_point(c->c2);
		free(c);
		c = NULL;
	}
}

void encrypt_ec(point **res1, point **res2, elgam_ec_ctx *eec, point *pm)
{
	gmp_printf("Encrypted: (%Zd,%Zd)\n", pm->x, pm->y);  

	mpz_init(eec->eph_k);
	get_random_n(eec->eph_k, eec->ec->p); //eph_k is random r.
	gmp_printf("\nEphemeral key = %Zd\n", eec->eph_k);

	cipherec *cipher = malloc(sizeof(cipherec));
	init_point(&cipher->c1);
	init_point(&cipher->c2);
	mpz_t tmp;
	mpz_init_set(tmp, eec->eph_k); //tmp is set to r
	cipher->c1 = ecc_scalar_mul(eec->ec, tmp, eec->ec->base); // base is e1. ec is elliptic curve
	mpz_clears(tmp, NULL);

	mpz_init_set(tmp, eec->eph_k);
	cipher->c2 = ecc_scalar_mul(eec->ec, tmp, eec->pub_key);
	mpz_clears(tmp, NULL);
	gmp_printf("Cipher C1: (%Zd,%Zd)\n", cipher->c1->x, cipher->c1->y);
	cipher->c2 = ecc_addition(eec->ec, cipher->c2, pm);
	gmp_printf("Cipher C2 with msg: (%Zd,%Zd)\n", cipher->c2->x, cipher->c2->y);
	*res1 = cipher->c1;
	*res2 = cipher->c2;
	//destroy_cipherec(cipher);
	//mpz_clears(eec->eph_k, NULL);
}


void decrypt_ec(point **res, elgam_ec_ctx *eec, point *c1, point *c2)
{
	point *d1, *d2;
	init_point(&d1);
	init_point(&d2);
	mpz_t tmp;
	mpz_init_set(tmp, eec->priv_key);
	d1 = ecc_scalar_mul(eec->ec, tmp, c1); // d1 = d * c1. d is tmp/private key.

	mpz_clears(tmp, NULL);
	gmp_printf("D1=(%Zd,%Zd)\n", d1->x, d1->y);
	mpz_neg(d1->y, d1->y); // x = -x
	d2 = ecc_addition(eec->ec, c2, d1); // P = c2 + (-d1) . d2 is P/Plain text.
	gmp_printf("Decrypted: (%Zd,%Zd)\n", d2->x, d2->y);
	*res = d2;
	//destroy_point(d1);
}

int compute_sqrt_mod_q(mpz_t x, mpz_t q) {
    // Check if x is a quadratic residue mod q
    return mpz_legendre(x, q) == 1;
}

void init_point_by_x(mpz_t x, point **p, elliptic_curve *curve)
{
	mpz_t real_x;
	mpz_init(real_x);
	mpz_set(real_x, x);

	// x <<= 8
	mpz_mul_2exp(x, x, 8);

	double end = pow(2, 15);
	for (int i = 0; i < end; ++i)
	{
		// x = x(x*x + a) + b mod q
		mpz_mul(x, x, x);
		mpz_add(x, x, curve->a);
		mpz_mul(x, x, x);
		mpz_add(x, x, curve->b);
		mpz_mod(x, x, curve->p);
		// check if x is quadratic residue and if true calculate y = sqrt(x) nod q and return (y, x), else repeate
		int res;
		if ((res = compute_sqrt_mod_q(x, curve->p)))
		{
			mpz_t y;
			mpz_init(y);
			mpz_sqrt(y, x);
			mpz_mod(y, y, curve->p);
			mpz_set((*p)->y, y);
			mpz_set((*p)->x, real_x);
			return;
		}
		// add 1 to the x
		mpz_add_ui(x, x, 1);
	}

	printf("Error, can't calculate y");
}

void encrypt_point(point **res1, point **res2, point *in, elgam_ec_ctx *eec)
{
	// Elgamal encryption.
	// c1 = kP (rand k * base point)
	// c2 = xkP + Pm (public key xP * rand k) + point on curve (secret message)
	gmp_printf("\nEnter Plain text in the form of point P(x,y) Usage:x,y\n");
	printf("\nEncrypting!!!!!!!!!!!!!!!!!\n");
	encrypt_ec(res1, res2, eec, in);
}

void decrypt_point(point **res, point *in1, point *in2, elgam_ec_ctx *eec)
{
	// Elgamal Decryption.
	// c1 * x = c1' = xkP
	// Pm = c1' - c2 = xkP - xkP + Pm
	gmp_printf("\nEnter cipher text C1, C2 in the form of point P(x,y)\n");
	printf("\nDecrypting!!!!!!!!!!!!!!!!!\n");
	decrypt_ec(res, eec, in1, in2);
}

char* mpz_to_str(mpz_t num) {
    // Convert the mpz_t number to a string in base 10
    char* str = mpz_get_str(NULL, 10, num);

    return str;
}

#include <stdio.h>
#include <gmp.h>

// Include your ElGamal-EC library here

void encrypt_decrypt_text(elgam_ec_ctx *eec) {
    char input[100] = {0};
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);

    mpz_t x;
    mpz_t y;
    point *p1, *res1, *res2, *res;
    init_point(&p1);
    init_point(&res1);
    init_point(&res2);
    init_point(&res);

    printf("Original: ");
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        printf("%c", input[i]);
    }
    printf("\n");

    printf("Encrypted: ");
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        mpz_init_set_ui(x, input[i]);
        mpz_init(y);

        init_point_by_x(x, &p1, eec->ec);
        encrypt_point(&res1, &res2, p1, eec);
        gmp_printf("(%Zd,%Zd), (%Zd,%Zd) ", res1->x, res1->y, res2->x, res2->y);

        mpz_clear(x);
        mpz_clear(y);
    }
    printf("\n");

    printf("Decrypted: ");
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        mpz_init_set_ui(x, input[i]);
        mpz_init(y);

        init_point_by_x(x, &p1, eec->ec);
        encrypt_point(&res1, &res2, p1, eec);
        decrypt_point(&res, res1, res2, eec);
        int value = mpz_get_si(res->x);
        printf("%c", value);

        mpz_clear(x);
        mpz_clear(y);
    }
    printf("\n");
}

int main() {
    elgam_ec_ctx *eec;
    init_elgam_ec(&eec);

    encrypt_decrypt_text(eec);

    return 0;
}

// int main()
// {
// 	// ElGamal-EC
// 	elgam_ec_ctx *eec;
// 	init_elgam_ec(&eec);
// 	char input[100] = {0};

// 	mpz_t x;
// 	mpz_t y;
// 	mpz_init_set_ui(x, 's');
// 	mpz_init(y);

// 	point *p1;
// 	init_point(&p1);
// 	init_point_by_x(x, &p1, eec->ec);
// 	gmp_printf("X %Zd Y %Zd", x, y);
// 	point *res1, *res2;
// 	point *res;
// 	init_point(&res1);
// 	init_point(&res2);
// 	init_point(&res);
// 	encrypt_point(&res1, &res2, p1, eec);
// 	decrypt_point(&res, res1, res2, eec);
// 	gmp_printf("D1=(%Zd,%Zd)\n", res->x, res->y);
// 	int value = mpz_get_si(res->x);
// 	printf("%c", value);

// 	destroy_elgam_ec(eec);
// 	return 0;
// }
