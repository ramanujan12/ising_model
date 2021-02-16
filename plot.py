#
# python script to plot the things and stuff
#
# author  : jannis schürmann
# caution : for some reason compiling and running with python is much slower than to execute
#           the 3 commands in a single bash line (y tho?)
#
# import libs
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf
import os
from os import listdir
from os.path import isfile, join

# plot the files to the same graph
def plot_thermalization(file_name):
    data = np.loadtxt(file_name)
    data_tr = data.transpose()
    x = data_tr[0]
    for col in data_tr[1:]:
        plt.plot(x, col, label = file_name)
    plt.title(file_name)
    plt.xlabel("t in steps")
    plt.ylabel("<M>/V or <E>/V")
                
# main function to read in data
if __name__ == '__main__':

    # plot thermalization
    path_t = "out_t/"
    files_t = [path_t + f for f in listdir(path_t) if isfile(join(path_t, f))]

    pdf_therm = matplotlib.backends.backend_pdf.PdfPages("therm.pdf")
    for file_name in files_t:
        plot_thermalization(file_name)
        pdf_therm.savefig(plt.gcf())
        plt.clf()
    pdf_therm.close()

    # plot exercise c
    data = np.loadtxt("exc.dat")
    data_tr = data.transpose()
    x = data_tr[0]
    for col in data_tr[1:]:
        plt.plot(x, col, label = file_name)
    # plt.title('exercise c')
    plt.xlabel('k_B*T [a.u.]')
    plt.ylabel('<M>/V')
    plt.savefig('exc.pdf')
    plt.clf()
