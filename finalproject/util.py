'''
Utility functions for plinko
'''

from math import pi

def deg2rad(deg):
    return pi * deg / 180.0

def rad2deg(rad):
    return rad * 180.0 / pi

def p2w_x(w, WindowH, WindowW):
    '''
    Converts from pixel to world, given a constant worldH of 2
    '''
    WorldW = 2.0 * WindowW / WindowH
    return WorldW / WindowW * w - WorldW / 2

def p2w_y(h, WindowH, WindowW):
    '''
    Coverts from pixel to world, given a contant worldH of 2
    '''
    WorldH = 2.0
    return -1 * (WorldH / WindowH * h - WorldH / 2)
