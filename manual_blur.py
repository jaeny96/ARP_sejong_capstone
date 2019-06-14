# -*- coding: utf-8 -*-
import cv2
import sys
import numpy as np
import random
# from manualClass import ManualClass

coor_list = []
temp = []


def onChange(x):
    pass

class ManualClass():
    def __init__(self, frame, frameId, coor_list, temp):

        self.img1 = frame.copy()
        self.frameId = int(frameId)
        self.refPt = []
        self.cropping = False
        self.drawing = False
        temp = []
        cv2.namedWindow("Video")
        cv2.setMouseCallback('Video', self.mouse_rec)

        clone = self.img1.copy()

        for coor in coor_list:
            x, y, x2, y2 = coor
            rec = cv2.rectangle(self.img1, (x, y), (x2, y2), (0,255,0), 2)

        while True:
            cv2.imshow('Video', self.img1)
            for coor in coor_list:
                x, y, x2, y2 = coor
                rec = cv2.rectangle(self.img1, (x, y), (x2, y2), (0, 255, 0), 2)
            key = cv2.waitKey(0) & 0xFF

            if key == 27 or key == 32 or key == ord('q'):
                break

        self.img1 = clone.copy()

    # Mouse Event
    def mouse_rec(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            self.drawing = True
            self.refPt = [(x, y)]
            self.cropping = True

        elif event == cv2.EVENT_MOUSEMOVE:
            if self.drawing:
                copy = self.img1.copy()
                cv2.rectangle(copy, self.refPt[0], (x, y), (0, 255, 0), 2)
                cv2.imshow("Video", copy)

        elif event == cv2.EVENT_LBUTTONUP:
            self.drawing = False
            self.refPt.append((x, y))
            self.cropping = False

            if self.refPt[0] == self.refPt[1]: #remove RECT
                temp = []
                for coor in coor_list:
                    x1, y1, x2, y2 = coor
                    if x > x1 and x < x2 and y > y1 and y < y2:
                        temp.append(coor)
                for i in temp:
                    coor_list.remove(i)

                for coor in temp:
                    x, y, x2, y2 = coor
                    rec = cv2.rectangle(self.img1, (x, y), (x2, y2), (0, 0, 255), 2)
                cv2.imshow('Video', self.img1)
            else:
                cv2.rectangle(self.img1, self.refPt[0], self.refPt[1], (0,255,0), 2)
                coor_list.append(self.refPt[0] + self.refPt[1])
                cv2.imshow("Video", self.img1)



filename = sys.argv[1]
savename = sys.argv[2]
capture = cv2.VideoCapture(filename)

width = capture.get(cv2.CAP_PROP_FRAME_WIDTH)
height = capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
fps = capture.get(cv2.CAP_PROP_FPS)
total_frame = capture.get(cv2.CAP_PROP_FRAME_COUNT)
cc = cv2.VideoWriter_fourcc(*'DIVX')

out = cv2.VideoWriter(savename, cc, fps, (int(width), int(height)))

cv2.namedWindow("Video")
cv2.moveWindow('Video', 400,300)

cv2.createTrackbar('BLUR Value', 'Video', 1, 60, onChange)
cv2.setTrackbarPos('BLUR Value', 'Video', 30)
cv2.setTrackbarMin('BLUR Value', 'Video', 1)


first = True
pause = False

while (capture.isOpened()):
    frameId = capture.get(1)
    ret, frame = capture.read()

    B = cv2.getTrackbarPos('BLUR Value', 'Video')


    if ret:

        if not pause:
            cl = frame.copy()
            for coor in coor_list:
                x, y, x2, y2 = coor
                blurred = cv2.blur(frame[coor[1]:coor[3], coor[0]:coor[2]], (B, B))
                frame[coor[1]:coor[3], coor[0]:coor[2]] = blurred
            cv2.imshow('Video', frame)

            if first:
                key = 32
                first = False
            else:
                key = cv2.waitKey(1) & 0xFF

            if key == 32: #spacebar
                while True:
                    key = cv2.waitKey(1) or 0xFF
                    cv2.imshow('Video', frame)
                    if key == 32 or key == ord('q'): #spacebar or 'q'
                        break
                    if key == ord('w'):
                        manualClass = ManualClass(frame, frameId, coor_list, temp)
                        break

            frame = cl.copy()
            for i,coor in enumerate(coor_list):
                if frame is not None:
                    blurred = cv2.blur(frame[coor[1]:coor[3], coor[0]:coor[2]],(B,B))
                    frame[coor[1]:coor[3], coor[0]:coor[2]] = blurred
            out.write(frame)

            if key == ord('q') or key == 27:
                print('stop')
                pause = True
                # out.release()
                # break
        else:
            out.write(frame)

    else:
        capture.release()
        out.release()
        cv2.destroyAllWindows()
        break


