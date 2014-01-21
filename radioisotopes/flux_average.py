import numpy as np
import pylab as lab
import scipy.optimize as opt
# Some Constants

def main():
    x, y=np.loadtxt("110mAg_crosssection_grey.txt", usecols=(0,1), unpack=True)
    lab.loglog()
    lab.plot(x,y)
    lab.show()

if __name__=="__main__":
    main()
