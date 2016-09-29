/* Proposal for automatically generating wrapper functions
 * for IDAES model property functions
 */


// This is the core - automatically generate a wrapperstub that calls the python wrapper
#define FUNCGEN(funcname) real funcname(arglist *al) {  \
    return py_wrapper(al, funcname); \
}

// This is what invokes the underlying python
real py_wrapper(arglist *al; funcname) {
  // This is the shortened version of the code.

  if (!Py_IsInitialized())
    {
      Py_Initialize();
    }

  // Do some stuff
  pName = PyString_FromString("phys_prop");
  pModule = PyImport_Import(pName);

  // Use the funcname passed in
  pFunc = PyObject_GetAttrString(pModule, funcname);

  //  Create objects, load data 
  // Call python etc.

  pValue = PyObject_CallObject(pFunc, pArgs);

  // DECR ref counts
  // 

  return V;

}

// All the above code comes from us.
// So a user just needs to do the following to define a new function:

FUNCGEN(h_vap)
addfunc("h_vap", (rfunc)h_vap, t, -1, NULL);



