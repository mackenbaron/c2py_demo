
typedef double real;

typedef struct arglist arglist;

struct arglist {
        int n;                  /* number of args */
        int *at;                /* argument types -- see DISCUSSION below */
        real *ra;               /* pure real args (IN, OUT, and INOUT) */
        real *derivs;           /* for partial derivatives (if nonzero) */
        real *hes;              /* for second partials (if nonzero) */
};