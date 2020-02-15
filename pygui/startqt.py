# Martin Fitzpatrick on learnpyqt:
# The following minimal example sets up a Matplotlib canvas FigureCanvasQTAgg 
# which creates the Figure and adds a single set of axes to it.
# This canvas object is also a QWidget and so can be embedded straight 
# into an application as any other Qt widget.

import sys
import random
import matplotlib
matplotlib.use('Qt5Agg')

from PyQt5 import QtCore, QtWidgets

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure

class MplCanvas(FigureCanvasQTAgg):

	def __init__(self, parent=None, width=5, height=4, dpi=100):
		fig = Figure(figsize=(width, height), dpi=dpi)
		self.axes = fig.add_subplot(111)
		super(MplCanvas, self).__init__(fig)

class MainWindow(QtWidgets.QMainWindow):

	def __init__(self, *args, **kwargs):
		super(MainWindow, self).__init__(*args, **kwargs)

		# Create the maptlotlib FigureCanvas object,
		self.canvas = MplCanvas(self, width=5, height=4, dpi=100)
		self.setCentralWidget(self.canvas)
		
		n_data = 50
		self.xdata = list(range(n_data))
		self.ydata = [random.randint(0,10) for i in range(n_data)]

		# store a reference to the plotted line
		self._plot_ref = None
		self.update_plot()
		self.show()

		# setup a timer to trigger the redraw by calling update_plot
		self.timer = QtCore.QTimer()
		self.timer.setInterval(100)
		self.timer.timeout.connect(self.update_plot)
		self.timer.start()

	def update_plot(self): 
		
		# remove first element, append one in the back
		self.ydata = self.ydata[1:] + [random.randint(0,10)]

		if self._plot_ref is None:
			# do a normal plot, first invoke
			# get first axes by [0] or use tuple
			plot_refs = self.canvas.axes.plot(self.xdata, self.ydata, 'r')
			self._plot_ref = plot_refs[0]
		else: 
			self._plot_ref.set_ydata(self.ydata)

		# trigger canvas to update and redraw
		self.canvas.draw()

# You need one (and only one) QApplication instance per application.
# Pass in sys.argv to allow command line arguments for your app.
# If you know you won't use command line arguments QApplication([]) is fine.
app = QtWidgets.QApplication(sys.argv)

w = MainWindow()
# window.show() # Windows are hidden by default

# Start the event loop.
app.exec_()

# Your application won't reach here until you exit and the event
# loop has stopped.
