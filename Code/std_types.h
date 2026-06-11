/*
 * std_types.h
 *
 *  Created on: Jun 7, 2026
 */


#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/*******************************************************************************
 *                          Data Types Declarations                            *
 *******************************************************************************/

/* Unsigned 8-bit integer */
typedef unsigned char uint8;

/* Signed 8-bit integer */
typedef signed char sint8;

/* Unsigned 16-bit integer */
typedef unsigned short uint16;

/* Signed 16-bit integer */
typedef signed short sint16;

/* Unsigned 32-bit integer */
typedef unsigned long uint32;

/* Signed 32-bit integer */
typedef signed long sint32;

/* Unsigned 64-bit integer */
typedef unsigned long long uint64;

/* Signed 64-bit integer */
typedef signed long long sint64;

/* Single precision floating point */
typedef float float32;

/* Double precision floating point */
typedef double float64;

/* Boolean type definition */
typedef unsigned char boolean;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Boolean false and true values */
#ifndef FALSE
#define FALSE        (0u)
#endif

#ifndef TRUE
#define TRUE         (1u)
#endif

/* NULL pointer definition */
#define NULL_PTR  ((void*)0)

/* Logic level definitions for digital signals */
#define LOGIC_HIGH   (1u)
#define LOGIC_LOW    (0u)

#endif /* STD_TYPES_H_ */
