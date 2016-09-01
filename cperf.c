
#include <Python.h>
#include <time.h>



#include "wrapper.h"

#include <sys/time.h>

#define N 10000

#define gettime(T, V) do {                  \
    gettimeofday(&(T), NULL);               \
        V = (T).tv_sec + (T).tv_usec / 1e6; \
    }                                       \
    while(0)

static real V_vap(arglist *al){
    real R_gas = 8.314472;

    // Ideal gas molar volume
    real T, P, V;
    T = al->ra[al->at[0]];
    P = al->ra[al->at[1]];
    V = R_gas*T/P;
    if(al->derivs){
        al->derivs[0] = R_gas/P;
        al->derivs[1] = -R_gas*T/(P*P);
        if(al->hes){
            al->hes[0] = 0;
            al->hes[1] = -R_gas/(P*P); 
            al->hes[2] = 2*R_gas*T/(P*P*P);
        }
    }
    return V;
}


int main()
{

    arglist al;


    real value;

    double tm[N], t0, t1;
    double max_t, min_t, mean_t;
    int i;
    struct timeval tv;

    int at[] = {0, 1};
    real ra[] = {5.123, 1.323};
    real derivs[] = {0, 0, 0};
    real hes[] = {0, 0, 0};

    al.n = 3;
    al.at = at;
    al.ra = ra;
    al.derivs = derivs;
    al.hes = hes;



    for (i=0; i < N; i++) {

      gettime(tv, t0);
      value = V_vap(&al);
      gettime(tv, t1);
      tm[i] = t1 - t0;
      // printf("Result of call: %f\n", value);
    

    }

    mean_t = max_t = 0;
    min_t = 1e9;
    for (i=0; i < N; i++) {
        double dt = tm[i];
        if (dt > max_t) max_t = dt;
        if (dt < min_t) min_t = dt;
        mean_t += dt;
    }
    mean_t = mean_t / N;
    printf("%d timings: min=%lfus max=%lfus mean=%lfus\n",
        N, min_t*1e6, max_t*1e6, mean_t*1e6);

    return 0;
}