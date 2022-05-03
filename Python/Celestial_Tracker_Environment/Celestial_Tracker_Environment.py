# -*- coding: utf-8 -*-
"""
Created on Sun May  1 13:04:03 2022

@author: austi
"""

import ctypes

dllPath = 'C:\\Users\\Austin\\Documents\\GitHub\\Celestial-Tracker\\Visual_Studio\\C_Types_Hello_World\\x64\\Release\\C_Types_Hello_World.dll'
lib = ctypes.cdll.LoadLibrary(dllPath)

lib.initialize()

g = lib.update()
print(g)

g = lib.update()
print(g)

print()

a = (ctypes.c_int32 * 2)(*[8,2])
h = lib.sum2(a)
print(h)