# -*- coding: utf-8 -*-
"""
Created on Sun May  1 13:04:03 2022

@author: austi
"""

import ctypes

lib = ctypes.cdll.LoadLibrary("C_Types_Hello_World.dll")

lib.initialize()

g = lib.update()
print(g)

g = lib.update()
print(g)

g = lib.update()
print(g)