import os
import cv2
import sys
import copy

# crop_dir = '../darkflow/Crop_images/'
# save_dir = '../darkflow/video1-masked.mp4'


def masking(crop_dir, origin, save_dir, value,templatetxt):


    file_list = list(filter(lambda x: x.find('.jpeg') != -1, os.listdir(crop_dir)))

    with open(templatetxt, 'r') as t:
        text = t.readlines()

    capture = cv2.VideoCapture(origin)
    cc = cv2.VideoWriter_fourcc(*'DIVX')
    width = capture.get(cv2.CAP_PROP_FRAME_WIDTH)
    height = capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
    fps = capture.get(cv2.CAP_PROP_FPS)

    out = cv2.VideoWriter(save_dir, cc, fps, (int(width), int(height)))

    coordict = {}
    coordict2 = {}

    for f in file_list:
        filename = (os.path.splitext(f)[0]).split('_')
        frameId, xmin, ymin, xmax, ymax = map(int, filename)

        if frameId not in coordict.keys():
            coordict[frameId] = [filename[1:]]
        else:
            coordict[frameId].append(filename[1:])

    for t in text:
        t = t[:-1]
        templatename = t.split('_')
        tframeId, txmin, tymin, txmax, tymax = map(int, templatename)

        if tframeId not in coordict2.keys():
            coordict2[tframeId] = [templatename[1:]]
        else:
            coordict2[tframeId].append(templatename[1:])

    while (capture.isOpened()):
        frameId = int(capture.get(1))
        tframeId = int(capture.get(1))
        ret, frame = capture.read()
        frame2 = copy.copy(frame)
        if ret:

            blurred = cv2.blur(frame, (int(value), int(value)))
            if frameId in coordict.keys():
                for coor in coordict[frameId]:
                    xmin, ymin, xmax, ymax = map(int, coor)

                    frame[ymin:ymax, xmin:xmax] = blurred[ymin:ymax, xmin:xmax]

                    if tframeId in coordict2.keys():
                        for coor2 in coordict2[tframeId]:
                            txmin, tymin, txmax, tymax = map(int, coor2)

                            frame[tymin:tymax, txmin:txmax] = frame2[tymin:tymax, txmin:txmax]

            cv2.imshow('Masking', frame)

            out.write(frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        else:
            capture.release()
            out.release()
            cv2.destroyAllWindows()
            break

    while (capture.isOpened()):
        frameId = int(capture.get(1))
        ret, frame1,frame2 = capture.read()

        if ret:
            blurred = cv2.blur(frame, (int(value), int(value)))
            if frameId in coordict.keys():
                for coor in coordict[frameId]:
                    xmin, ymin, xmax, ymax = map(int, coor)

                    frame1[ymin:ymax, xmin:xmax] = blurred[ymin:ymax, xmin:xmax]

                    if tframeId in coordict2.keys():
                        for coor2 in coordict2[tframeId]:
                            txmin, tymin, txmax, tymax = map(int, coor2)

                            frame1[tymin:tymax, txmin:txmax] = frame2[tymin:tymax, txmin:txmax]
            cv2.imshow('Masking', frame1)

            out.write(frame1)

            if cv2.waitKey(1) & 0xFF == ord('q') or cv2.waitKey(1) & 0xFF == 27:
                break

        else:
            capture.release()
            out.release()
            cv2.destroyAllWindows()
            break

crop_dir = sys.argv[1]
origin = sys.argv[2]
save_dir = sys.argv[3]
value = sys.argv[4]
templatetxt = sys.argv[5]
# print(crop_dir)
# print(origin)
# print(save_dir)
# print(value)
#
# crop_dir = "C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/Crop_images"
# origin = "C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/video_2.mp4"
# save_dir = "C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/video-masked.mp4"
# value = 30

masking(crop_dir, origin, save_dir,value,templatetxt)
