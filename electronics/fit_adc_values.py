import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from numpy import float64
import pandas as pd


class Translator:
    def __init__(self, xdata, ydata):
        self.xdata = xdata
        self.ydata = ydata
        self._popt, pcov = curve_fit(self.func, self.xdata, self.ydata)

    def freq2digits(self, freq):
        return self.counter_func(freq, *self._popt)

    @staticmethod
    def func(x, m, t):
        return m * x + t

    @staticmethod
    def counter_func(y, m, t):
        return (y-t)/m

    @property
    def popt(self):
        return self._popt

    def print_func(self):
        return f"y = f(x) = {self._popt[0]}*x + {self._popt[1]}"

    def plot_data_against_approximation(self):
        plt.plot(self.xdata, self.func(float64(self.xdata), *self._popt), 'r', label='approximation')
        plt.plot(self.xdata, self.ydata, 'b-', label='data')
        plt.legend()


if __name__ == "__main__":
    df = pd.read_csv("adc.raw")
    xdata = df["adc_raw"].to_list()
    ydata = df["voltage"].to_list()
    t = Translator(xdata, ydata)
    print("popt = " + t.popt)
    t.plot_data_against_approximation()

