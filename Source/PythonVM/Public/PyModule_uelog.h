#pragma once

#include "CoreMinimal.h"
#include <Python.h>
#include "PythonVM.h"

static PyObject* PrintUELogNormal(PyObject* self, PyObject* args)
{
    const char* command;
    //UE_LOG(LogPythonVM, Display, TEXT("PrintUELogNormal"));
    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    if (command)
    {
        AsyncTask(ENamedThreads::GameThread, [=]() {UE_LOG(LogPythonVM, Display, TEXT("PythonLevel:%s"), UTF8_TO_TCHAR(command)); });
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* PrintUELogError(PyObject* self, PyObject* args)
{
    const char* command;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    if (command)
    {
        AsyncTask(ENamedThreads::GameThread, [=]() {UE_LOG(LogPythonVM, Error, TEXT("PythonLevel:%s"), UTF8_TO_TCHAR(command)); });
        //printf("c++: %s", command);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef uelogmethods[] = {
    {"print",  PrintUELogNormal, METH_VARARGS, "print log in unreal engine."},
    {"error",  PrintUELogError, METH_VARARGS, "print error log in unreal engine."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef uelogmodule = {
    PyModuleDef_HEAD_INIT,
    "uelog",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    uelogmethods
};

PyMODINIT_FUNC
PyInit_uelog(void)
{
    return PyModule_Create(&uelogmodule);
}