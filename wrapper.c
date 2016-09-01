
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



PyObject *make_int_list(int array[], size_t size) {
    PyObject *l = PyList_New(size);
    for (size_t i = 0; i != size; ++i) {
        PyList_SET_ITEM(l, i, PyInt_FromLong(array[i]));
    }
    return l;
}


PyObject *make_real_list(real array[], size_t size) {
    PyObject *l = PyList_New(size);
    for (size_t i = 0; i != size; ++i) {
        PyList_SET_ITEM(l, i, PyFloat_FromDouble(array[i]));
    }
    return l;
}

void pargs_dec_ref(PyObject *tup) {
    // Py_DECREF(PyTuple_GetItem(tup, 0));
    Py_DECREF(PyTuple_GetItem(tup, 1));
    Py_DECREF(PyTuple_GetItem(tup, 2));
    Py_DECREF(PyTuple_GetItem(tup, 3));
    Py_DECREF(PyTuple_GetItem(tup, 4));

}


int
main(int argc, char *argv[])
{

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    arglist al;


    double tm[N], t0, t1;
    double max_t, min_t, mean_t;
    int i;
    struct timeval tv;

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }


    int at[] = {0, 1};
    real ra[] = {5.123, 1.323};
    real derivs[] = {0, 0, 0};
    real hes[] = {0, 0, 0};


    al.n = 3;
    al.at = at;
    al.ra = ra;
    al.derivs = derivs;
    al.hes = hes;




    /* begin repeat loop */

    for (i=0; i < N; i++) {

    gettime(tv, t0);
    Py_Initialize();

    pName = PyString_FromString(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {


            pArgs = PyTuple_New(5);
            PyTuple_SetItem(pArgs, 0, PyInt_FromLong(al.n));
            PyTuple_SetItem(pArgs, 1, make_int_list(al.at, 2));
            PyTuple_SetItem(pArgs, 2, make_real_list(al.ra, 2));
            PyTuple_SetItem(pArgs, 3, make_real_list(al.derivs, al.n));
            PyTuple_SetItem(pArgs, 4, make_real_list(al.hes, al.n));
                



            pValue = PyObject_CallObject(pFunc, pArgs);


            if (pValue != NULL) {
                // printf("Result of call: %ld\n", PyInt_AsLong(pValue));


                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                pargs_dec_ref(pArgs);
                Py_XDECREF(pArgs);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }

            /* decref arrays in object */
            pargs_dec_ref(pArgs);
            Py_XDECREF(pArgs);

        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }

    gettime(tv, t1);
    tm[i] = t1 - t0;
    } /* repeat loop */

    Py_Finalize();


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
