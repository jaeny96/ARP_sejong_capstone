import cv2
import sys
import os
import numpy as np
import imageio


def template(filename, tem, gifname,templatetxt):
    txt = open(templatetxt, 'w')

    print(filename, tem)
    #
    # print(tem)
    temp = []
    # tem = tem[1:]
    filename = filename[1:]
    cap= cv2.VideoCapture(filename)
    # cap_gray = cv.cvtColor(cap_rgb, cv.COLOR_BGR2GRAY)

    for i in tem:
        if(tem != ""):
            print(i)
            temimg = cv2.imread('/'+i,cv2.IMREAD_GRAYSCALE)
            temp.append(temimg)


    # # for i in template:
    # #     w, h = i.shape[::-1]
    #
    method = cv2.TM_CCOEFF_NORMED
    gif = imageio.mimread(gifname)
    nums = len(gif)
    imgs = [cv2.cvtColor(img, cv2.COLOR_RGB2BGR) for img in gif]
    i_g = 0
    total_frame = cap.get(cv2.CAP_PROP_FRAME_COUNT)
    str_loc = 70

    while True:
        frameId = cap.get(1)
        ret,frame = cap.read()
        if ret:
            cap_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            percentage = int((frameId+1)/total_frame*100)
            for i in temp:
                res = cv2.matchTemplate(cap_gray, i, method)
                w, h = i.shape[::-1]
                min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

                if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
                    top_left = min_loc
                else:
                    top_left = max_loc
                # bottom_right = (top_left[0] + w, top_left[1] + h)
                # cv2.rectangle(frame,top_left,bottom_right,255,2)
                # cv2.imshow('frame',frame)

                threshold = 0.8
                loc = np.where(res >= threshold)

                for pt in zip(*loc[::-1]):
                    cv2.rectangle(frame, pt, (pt[0] + w, pt[1] + h), (0, 0, 255), 2)
                    txt.write('{}_{}_{}_{}_{}\n'.format(int(frameId), pt[0], pt[1], pt[0] + w, pt[1] + h))
                    # cv2.imwrite('C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/templateimg/{}_{}_{}_{}_{}.jpeg'.format(int(frameId),pt[0],pt[1],pt[0]+w,pt[1]+h), frame)

#            cv2.imshow('Template Matching', frame)
            clone = imgs[i_g].copy()
            if percentage == 100:
                str_loc = 58
            cv2.putText(imgs[i_g], str(percentage) + '%', (str_loc, 110), cv2.FONT_HERSHEY_SCRIPT_SIMPLEX, 1, (76, 76, 76))
            cv2.imshow("Loading", imgs[i_g])
            imgs[i_g] = clone.copy()
            i_g = (i_g + 1) % nums
            
            if cv2.waitKey(10) & 0xFF == ord('q'):
                break
        else:
            txt.close()
            cap.release()
            cv2.destroyAllWindows()
            break
            # cv2.waitKey(0)

    # cap.release()
    # cap_gray.release()
    # cv2.destroyAllWindows()

filename = sys.argv[1]
tem = sys.argv[2]
gifname = sys.argv[3]
templatetxt = sys.argv[4]
# print(filename)
# print(tem)
# print(type(tem))
li = tem.split(',')
# print(li)
# print(type(li))

templ = ['C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/face1.jpeg', 'C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/face2.jpeg','C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/face3.jpeg','C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/face4.jpeg','C:/Users/JISU/PycharmProjects/capstone/cctv_j/cctv3/face5.jpeg']
# for i in tem:
#     i = i[1:]

template(filename,li,gifname,templatetxt)
