#define PY_SSIZE_T_CLEAN
#include <python3.12/Python.h>
#include <stdio.h>
#include <fcntl.h>

static PyObject *inferencepy;
static PyObject *nnpy;

/**
 * The external function calls must be deterministic: for any argument values, the function should produce the same result at any time when called with the same values.
 * For example:
 * 
 *  Side-effect-free (or free) functions, which do not depend on static variables, are deterministic.
 *  Memoization pattern is not side-effect-free but results in a deterministic behavior, hence is also allowed.
 *
*/
double nn(double v, double eps_x, double eps_v)
{
    PyObject *val[3];
    val[0] = PyFloat_FromDouble(v);
    val[1] = PyFloat_FromDouble(eps_x);
    val[2] = PyFloat_FromDouble(eps_v);
    if(!(val[0] && val[1] && val[2]))
        abort();

    PyObject *args = PyTuple_Pack(3, val[0], val[1], val[2]);
    if(!args)
        abort();

    PyObject *ret = PyObject_CallObject(nnpy, args);
    if(!ret)
        abort();

    double ret2 = PyFloat_AsDouble(ret);

    Py_DECREF(ret);
    Py_DECREF(val[0]);
    Py_DECREF(val[1]);
    Py_DECREF(val[2]);
    Py_DECREF(args);
    return ret2;
}


void __attribute__ ((constructor)) enter()
{
    //int fd = open("/tmp/AAAAA.log", O_RDWR | O_CREAT | O_TRUNC, 0644);
    //dup2(fd, STDIN_FILENO);
    //dup2(fd, STDOUT_FILENO);
    //dup2(fd, STDERR_FILENO);
    //chdir("/home/alessio/Documenti/verificann/smc");

    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    status = PyConfig_SetBytesString(&config, &config.program_name, "/home/alessio/virtualenvs/torchverify/bin/python");
    if (PyStatus_Exception(status))
    {
        //Py_ExitStatusException(status);
        abort();
    }

    PyConfig_SetBytesString(&config, &config.pythonpath_env, "/home/alessio/Documenti/verificann/smc");

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status))
    {
        //Py_ExitStatusException(status);
        abort();
    }

    PyConfig_Clear(&config);

    inferencepy = PyImport_ImportModule("inference");
    if (! inferencepy)
    {
        PyErr_Print();
        abort();
    }

    nnpy = PyObject_GetAttrString(inferencepy, "nn");
    if (!(nnpy && PyCallable_Check(nnpy)))
        abort();
}

void __attribute__ ((destructor)) leave()
{
    Py_DECREF(nnpy);
    Py_DECREF(inferencepy);

    // @FIXME causes SIGABRT on UPPAAL
    //if (Py_FinalizeEx() < 0)
    //    abort();
}
