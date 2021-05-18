import time
import threading


class PositionSimulation:
	def __init__(self, positionList, periodInSeconds=1):
		
		# Connection parameters
		self.positionList = positionList
		self.periodInSeconds = periodInSeconds
		# Current position values
		self.Latitude = None
		self.Longitude = None
		self.Altitude = None
		# Background thread parameters
		self.backgroundThread = None
		self.backgroundThreadIsActive = False
		self.backgroundThreadShouldGoOn = False

	# ---- Methods to be called from the outside ---- #

	def start(self):
		if not self.backgroundThreadIsActive: 
			self.backgroundThreadShouldGoOn = True
			self.backgroundThread = threading.Thread(target=self.getPositionInBackgroundThread)
			self.backgroundThread.start()
			self.backgroundThreadIsActive = True
				
	def stop(self):
		if self.backgroundThreadIsActive:
			self.backgroundThreadShouldGoOn = False
			self.backgroundThread.join()
			self.backgroundThreadIsActive = False

	def getPosition(self):
		return self.Latitude, self.Longitude, self.Altitude



	# ---- Methods to be called from the inside ---- #

	def getPositionInBackgroundThread(self):
		while self.backgroundThreadShouldGoOn:
			# Get the current position values and append them at the end
			currentValues = self.positionList.pop(0)
			self.positionList.append(currentValues)
			# Update the current values
			self.Latitude = currentValues['Latitude']
			self.Longitude = currentValues['Longitude']
			self.Altitude = currentValues['Altitude']
			# Weit for next values
			time.sleep(self.periodInSeconds)