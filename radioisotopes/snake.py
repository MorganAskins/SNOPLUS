import numpy as np
import pylab as lab

def main():
    x1 = np.array([1,2,3,4,5,6,7,8,9])
    y1=[]
    for i in range(len(x1)):
        y1.append(x1**2)
    y1=np.array(y1)

    x2 = np.array([2,5,6,8,9,12])
    y2=[]
    for i in range(len(x2)):
        y2.append(1/(3*x2**3))
    y2=np.array(y2)

    snake=func(x1, y1, x2, y2)
    lab.plot(snake[0], snake[1])
    lab.show()

def func(x1, y1, x2, y2):
    # Determine the product of two arrays
    output=[[],[]]

    for v1 in range(len(x1)):
        if (x1[v1]<x2
    
    
