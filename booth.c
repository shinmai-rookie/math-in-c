/* booth.c: Perform the Booth's algorithm step by step
 * Copyright (C) 2015  Jaime Mosquera
 *
 * Booth is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Booth is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Booth.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

/* The type of the multiplier and the multiplicand */
#define INT_TYPE        signed char
/* The type of the result; must be twice as big as INT_TYPE */
#define DOUBLE_INT_TYPE signed short int
/* With this format we read the multiplier and the multiplicand with scanf */
#define INT_TYPE_SCANF  "%hi"
/* Number of \t that are necessary to align "Multiplier", "Multiplicand" and "Result" */
/* We need  sizeof(DOUBLE_INT_TYPE)  \t (assuming \t=8 columns) */
#define INT_TYPE_MARGIN "\t\t"

/* We convert the number to 2-complement, in case the computer uses
 * 1-complement (or sign-magnitude, for that matter) */
#define CONVERT_TO_2_COMPLEMENT(n)     \
 {                                     \
     if ((n) < 0)                      \
         (n) = ~(-(n)) + 1;            \
 }

/* Return the  i-th  bit of  n  (which is a DOUBLE_INT_TYPE) */
#define GET_BIT(n, i)   (((n) & ((DOUBLE_INT_TYPE) 1 << (i))) && 1)

void print_binary(DOUBLE_INT_TYPE n)
{
    size_t i;
    for (i = sizeof(DOUBLE_INT_TYPE) * 8; i > 0; i--)
        printf("%d", GET_BIT(n, i - 1));
}

/* Multiply according to the Booth algorithm, showing the steps and the
 * result */
DOUBLE_INT_TYPE booth(INT_TYPE multiplier, INT_TYPE multiplicand)
{
    /* We convert multiplier and multiplicand to bigger integers in order not
     * to overflow when shifting leftward */
    DOUBLE_INT_TYPE result = 0, m = multiplier, n = multiplicand;
    m *= 2;
    printf("Multiplier:%sMultiplicand:%sResult:\n",
           INT_TYPE_MARGIN, INT_TYPE_MARGIN);
    while (m != 0)
    {
        /* If the number ends in [...]01 -> Add */
        if (GET_BIT(m, 0) && !GET_BIT(m, 1))
            result += n;
        /* If the number ends in [...]10 -> Subtract */
        else if (!GET_BIT(m, 0) && GET_BIT(m, 1))
            result -= n;

        print_binary(m);
        printf("\t");
        print_binary(n);
        printf("\t");
        print_binary(result);
        printf("\n");

        /* By dividing by two we avoid the padding in integers, a possible
         * non-arithmetic extension… */
        m /= 2;
        /* We shift  n  a bit leftward */
        n *= 2;
    }

    return result;
}

int main(int argc, char** argv)
{
    DOUBLE_INT_TYPE multiplier, multiplicand;

    printf("Enter the multiplier: ");
    fflush(stdout);
    scanf(INT_TYPE_SCANF, &multiplier);
    CONVERT_TO_2_COMPLEMENT(multiplier);

    printf("Enter the multiplicand: ");
    fflush(stdout);
    scanf(INT_TYPE_SCANF, &multiplicand);
    CONVERT_TO_2_COMPLEMENT(multiplicand);

    printf("Final result: %i", booth(multiplier, multiplicand));

    return 0;
}
