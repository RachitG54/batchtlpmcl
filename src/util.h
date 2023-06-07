/*
 * Adithya Bhat <bhat24@purdue.edu>
 * 2019
 * */

#include <gmp.h>
#include <stdint.h>
#include <sys/time.h>

#ifndef __LHP_UTIL__
#define __LHP_UTIL__

void generate_prime ( mpz_t p , uint64_t bit_count ) ;

/* Strong prime generation code
 *
 * Step 1: Generate p'
 * Step 2: Make sure p' is prime
 * Step 3: Get p = 2p'+1
 * Step 4: Ensure that p is prime
 * */
void generate_strong_prime ( mpz_t p , uint64_t bit_count ) ;

/*
 * 1. Generate a random element r of N bits
 * 2. Return if gcd ( r , N ) = 1
 * */
void random_group_element ( mpz_t res , mpz_t N ) ;

void totient_n ( mpz_t res , mpz_t p , mpz_t q ) ;

static void print_exp_info ( 
        struct timeval *start ,
        struct timeval *end , 
        char* exp_name 
    )
{
    struct timeval diff ;
    uint64_t time ; 
    timersub ( end , start , &diff ) ;
    time = diff.tv_sec * 1000000 + diff.tv_usec ;
    printf ( "Time taken for %s: [%lu] microseconds or [%.3f] seconds \n" ,
            exp_name , (unsigned long)time , time / 1000000.0 ) ;
}

#define SUCCESS(test_name) (printf("%s test: Passed\n", test_name))
#define FAILED(test_name) (printf("%s test: Failed\n", test_name))

#endif