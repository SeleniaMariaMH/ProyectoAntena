import serial
import threading
import time

class GPS:
	def __init__(self, portName, baudRate, timeOut):
		# verbose
		self.verbose = False
		# Connection parameters
		self.portName = portName
		self.baudRate = baudRate
		self.timeOut = timeOut
		self.gpsSerial = None
		# Background thread parameters
		self.backgroundThread = None
		self.backgroundThreadIsActive = False
		self.backgroundThreadShouldGoOn = False
		# Location parameters
		self.Latitude = None
		self.Longitude = None
		self.Latitude_RMC = None
		self.Longitude_RMC = None
		self.Latitude_GGA = None
		self.Longitude_GGA = None
		self.Latitude_GLL= None
		self.Longitude_GLL = None
		self.NSatelites = None


	# xxxxxxxxxx Methods triggered from the caller xxxxxxxxxx #

	def connect(self):
		self.gpsSerial = serial.Serial(self.portName, self.baudRate, timeout=self.timeOut)
		time.sleep(3)

	def startMeasuring(self):
		if not self.backgroundThreadIsActive: 
			self.backgroundThreadShouldGoOn = True
			self.backgroundThread = threading.Thread(target=self.measureInBackgroundThread, args=(1,))
			self.backgroundThread.start()
			self.backgroundThreadIsActive = True
				
	def stopMeasuring(self):
		if self.backgroundThreadIsActive:
			self.backgroundThreadShouldGoOn = False
			self.backgroundThread.join()
			self.backgroundThreadIsActive = False

	def getPosition(self):
		self.updateLatLon()
		return self.Latitude, self.Longitude




	# xxxxxxxxxx Internal methods xxxxxxxxxx #

	def measureInBackgroundThread(self, args):
		starting = True
		fullLine = ''
		while self.backgroundThreadShouldGoOn:
			if starting:
				print('waiting for initial $')
				while self.gpsSerial.read().decode('utf-8') != '$':
					# print(self.gpsSerial.read().decode('utf-8'))
					pass
				starting = False
			reading = self.gpsSerial.read().decode('utf-8') # Read the entire string
			if reading != '$':
				fullLine = fullLine + reading
			else:
				# print(fullLine)
				self.decodeLine(fullLine)
				fullLine = ''


	def decodeLine(self, line):
		if self.validateChecksum(line):
			if line[0:5] == 'GPRMC':
				self.decodeRMC(line)
			if line[0:5] == 'GPGGA':
				self.decodeGGA(line)
			if line[0:5] == 'GPGLL':
				self.decodeGLL(line)


	def validateChecksum(self, entireLine):
		checkString = entireLine.partition("*")
		checksum = 0
		for c in checkString[0]:
			checksum ^= ord(c)
		try: # Just to make sure
			inputChecksum = int(checkString[2].rstrip(), 16);
		except:
			if self.verbose:
				print('Error in string: {}'.format(entireLine))
			return False
		if checksum != inputChecksum:
			if self.verbose:
				print('Error in checksum')
			return False
		# Return true if checksum is ok
		return True



	# ---------- Decode Latatitude and Longitude Strings --------- #

	def getLatitude(self, latitudeValueString, latitudeOrientationString):
		latitude = float(latitudeValueString[:2]) + float(latitudeValueString[2:])*1.0/60.0
		if latitudeOrientationString == 'N':
			return latitude
		elif latitudeOrientationString == 'S':
			return (-1) * latitude
		else:
			if self.verbose:
				print('Latitude direction differs: {}'.format(latitudeOrientationString))
			return None


	def getLongitude(self, longitudeValueString, longitudeOrientationString):
		longitude = float(longitudeValueString[:3]) + float(longitudeValueString[3:])*1.0/60.0
		if longitudeOrientationString == 'E':
			return longitude
		elif longitudeOrientationString == 'W':
			return (-1) * longitude
		else:
			if self.verbose:
				print('Longitude direction differs: {}'.format(longitudeOrientationString))
			return None


	def updateLatLon(self):
		# Latitude
		# No one is None
		if self.Latitude_RMC is not None and self.Latitude_GGA is not None and self.Latitude_GLL is not None:
			self.Latitude = (self.Latitude_RMC + self.Latitude_GGA + self.Latitude_GLL) / 3
		else:
			# Just one is None
			if self.Latitude_RMC is not None and self.Latitude_GGA is not None:
				self.Latitude = (self.Latitude_RMC + self.Latitude_GGA) / 2
			elif self.Latitude_GGA is not None and self.Latitude_GLL is not None:
				self.Latitude = (self.Latitude_GGA + self.Latitude_GLL) / 2
			elif self.Latitude_RMC is not None and self.Latitude_GLL is not None:
				self.Latitude = (self.Latitude_RMC+ self.Latitude_GLL) / 2
			else:
				# Two of them are None
				if self.Latitude_RMC is not None:
					self.Latitude = self.Latitude_RMC
				elif self.Latitude_GGA is not None:
					self.Latitude = self.Latitude_GGA
				elif self.Latitude_GLL is not None:
					self.Latitude = self.Latitude_GLL
				else:
					# All of them are None
					self.Latitude = None
		# Longitude
		# No one is None
		if self.Longitude_RMC is not None and self.Longitude_GGA is not None and self.Longitude_GLL is not None:
			self.Longitude = (self.Longitude_RMC + self.Longitude_GGA + self.Longitude_GLL) / 3
		else:
			# Just one is None
			if self.Longitude_RMC is not None and self.Longitude_GGA is not None:
				self.Longitude = (self.Longitude_RMC + self.Longitude_GGA) / 2
			elif self.Longitude_GGA is not None and self.Longitude_GLL is not None:
				self.Longitude = (self.Longitude_GGA + self.Longitude_GLL) / 2
			elif self.Longitude_RMC is not None and self.Longitude_GLL is not None:
				self.Longitude = (self.Longitude_RMC+ self.Longitude_GLL) / 2
			else:
				# Two of them are None
				if self.Longitude_RMC is not None:
					self.Longitude = self.Longitude_RMC
				elif self.Longitude_GGA is not None:
					self.Longitude = self.Longitude_GGA
				elif self.Longitude_GLL is not None:
					self.Longitude = self.Longitude_GLL
				else:
					# All of them are None
					self.Longitude = None



	# ---------- RMC Decoding methods ---------- #

	def decodeRMC(self, entireLine):
		subLines = entireLine.split(",")
		latitudeValueString = subLines[3]
		latitudeOrientationString = subLines[4]
		longitudeValueString = subLines[5]
		longitudeOrientationString = subLines[6]
		if latitudeValueString != '' and latitudeOrientationString != '' and longitudeValueString != '' and longitudeOrientationString != '':
			self.Latitude_RMC = self.getLatitude(latitudeValueString, latitudeOrientationString)
			self.Longitude_RMC = self.getLongitude(longitudeValueString, longitudeOrientationString)
			if self.verbose:
				print('RMC (Lat, Long) = ({}, {})'.format(self.Latitude, self.Longitude))
		else:
			self.Latitude_RMC = None
			self.Longitude_RMC = None
			if self.verbose:
				print('RMC no coordinates')



	# ---------- GGA Decoding methods ---------- #

	# TODO: Decode altitude here
	def decodeGGA(self, entireLine):
		subLines = entireLine.split(",")
		if self.verbose:
			print("GGA Altitude: ", subLines[9], subLines[10],sep="")
		latitudeValueString = subLines[2]
		latitudeOrientationString = subLines[3]
		longitudeValueString = subLines[4]
		longitudeOrientationString = subLines[5]
		if latitudeValueString != '' and latitudeOrientationString != '' and longitudeValueString != '' and longitudeOrientationString != '':
			self.Latitude_GGA = self.getLatitude(latitudeValueString, latitudeOrientationString)
			self.Longitude_GGA = self.getLongitude(longitudeValueString, longitudeOrientationString)
			if self.verbose:
				print('GGA (Lat, Long) = ({}, {})'.format(self.Latitude, self.Longitude))
		else:
			self.Latitude_GGA = None
			self.Longitude_GGA = None
			if self.verbose:
				print('GGA no coordinates')




	# ---------- GLL Decoding methods ---------- #

	def decodeGLL(self, entireLine):
		subLines = entireLine.split(",")
		latitudeValueString = subLines[1]
		latitudeOrientationString = subLines[2]
		longitudeValueString = subLines[3]
		longitudeOrientationString = subLines[4]
		if latitudeValueString != '' and latitudeOrientationString != '' and longitudeValueString != '' and longitudeOrientationString != '':
			self.Latitude_GLL = self.getLatitude(latitudeValueString, latitudeOrientationString)
			self.Longitude_GLL = self.getLongitude(longitudeValueString, longitudeOrientationString)
			if self.verbose:
				print('GLL (Lat, Long) = ({}, {})'.format(self.Latitude, self.Longitude))
		else:
			self.Latitude_GLL = None
			self.Longitude_GLL = None
			if self.verbose:
				print('GLL no coordinates')


			
