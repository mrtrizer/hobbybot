#!/usr/bin/python
import numpy as np
import cv
import cv2
from matplotlib import pyplot as plt

cv2.namedWindow("cam1")
cv2.namedWindow("cam2")
cv2.namedWindow("gray")
cv2.namedWindow("Face Detection")

haarFace = cv.Load('haarcascade_frontalface_default.xml')
haarEyes = cv.Load('haarcascade_eye.xml')

capture1 = cv2.VideoCapture(1)
capture2 = cv2.VideoCapture(2)

SADWin = 31
ndispar = 11

while True:
	rval, imgL = capture1.read()
	rval, imgR = capture2.read()

	

	imgL = cv2.cvtColor(imgL,cv2.COLOR_BGR2GRAY)
	imgR = cv2.cvtColor(imgR,cv2.COLOR_BGR2GRAY)

	faceImg = imgL

	# running the classifiers
	storage = cv.CreateMemStorage()
	detectedFace = cv.HaarDetectObjects(cv.fromarray(faceImg), haarFace, storage)

	# draw a green rectangle where the face is detected
	if detectedFace:
		for face in detectedFace:
			cv.Rectangle(cv.fromarray(faceImg),(face[0][0],face[0][1]),(face[0][0]+face[0][2],face[0][1]+face[0][3]),cv.RGB(155, 255, 25),2)

	cv2.imshow("Face Detection", faceImg) 

	cv2.imshow("cam1",imgL)
	cv2.imshow("cam2",imgR)
	key = cv2.waitKey(1) & 0xFF
	if key == ord('q'):
		break	
	if key == ord('+'):
		SADWin += 1
		print "SADWin: " + str(SADWin)
	if key == ord('-'):
		SADWin -= 1
		print "SADWin: " + str(SADWin)
	if key == ord('w'):
		tmp = capture1
		capture1 = capture2
		capture2 = tmp
		print("chaged")
	stereo = cv2.StereoBM(cv2.STEREO_BM_BASIC_PRESET,ndisparities=0, SADWindowSize=SADWin)
	disparity = stereo.compute(imgL,imgR,disptype=cv2.CV_32F)
	norm_coeff = 255 / disparity.max()
	cv2.imshow("gray",disparity * norm_coeff / 255)
capture1.release()
capture2.release()
cv2.destroyAllWindows()
