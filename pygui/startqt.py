import sys
from PyQt5.QtWidgets import QApplication

# You need one (and only one) QApplication instance per application.
# Pass in sys.argv to allow command line arguments for your app.
# If you know you won't use command line arguments QApplication([]) is fine.
app = QApplication(sys.argv)

# Start the event loop.
app.exec_()

# Your application won't reach here until you exit and the event
# loop has stopped.