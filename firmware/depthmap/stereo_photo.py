#!/usr/bin/python

import cv,cv2
import threading

cv2.namedWindow("cam1")
cv2.namedWindow("cam2")

capture1 = cv2.VideoCapture(1)
capture2 = cv2.VideoCapture(2)

imgL = 0
imgR = 0

listFile = open('list.txt', 'w')

count = 0
n = 1
N_MAX = 20
TIMEOUT = 7

def makePhoto():
	global count
	global n
	count += 1
	print str(count)
	if (n <= N_MAX):
		threading.Timer(1.0, makePhoto).start()


while True:
	rval, imgL = capture1.read()
	rval, imgR = capture2.read()
	imgL = cv2.cvtColor(imgL,cv2.COLOR_BGR2GRAY)
	imgR = cv2.cvtColor(imgR,cv2.COLOR_BGR2GRAY)

	if (count >= TIMEOUT):
		print "Photo #" + str(n)
		name1 = "images/img"+str(n)+"left.png"
		name2 = "images/img"+str(n)+"right.png"
		cv2.imwrite(name1,imgL)
		cv2.imwrite(name2,imgR)
		listFile.write(name1 + "\n")
		listFile.write(name2 + "\n")
		count = 0
		n += 1

	cv2.imshow("cam1",imgL)
	cv2.imshow("cam2",imgR)

	key = cv2.waitKey(1) & 0xFF
	if key == ord('q'):
		break
	if key == ord('s'):
		threading.Timer(1.0, makePhoto).start()
		print("started")
	if key == ord('w'):
		tmp = capture1
		capture1 = capture2
		capture2 = tmp
		print("chaged")

capture1.release()
capture2.release()
cv2.destroyAllWindows()
