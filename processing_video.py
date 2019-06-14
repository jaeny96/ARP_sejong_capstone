import cv2
from darkflow.net.build import TFNet
import shutil
import sys
import os
import imageio


def check_directory(dir):
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.mkdir(dir)

def processing(filename, cropdir, modeldir, gifname):
    option = {
        'model': modeldir,
        'load': 43125,
        'threshold': 0.05,
    }
    
    tfnet = TFNet(option)
    capture = cv2.VideoCapture(filename)

#gifname = "/Users/byeonjiyeon/Downloads/Spinner.gif"
    gif = imageio.mimread(gifname)
    
    nums = len(gif)
    imgs = [cv2.cvtColor(img, cv2.COLOR_RGB2BGR) for img in gif]
    i_g = 0
    
    total_frame = capture.get(cv2.CAP_PROP_FRAME_COUNT)
    str_loc = 70
    
    while (capture.isOpened()):
        frameId = capture.get(1) #sec
        
        ret, frame = capture.read()
        
        if ret:
            results = tfnet.return_predict(frame)
            #            print('{}%'.format(int((frameId + 1) / total_frame * 100)))
            percentage = int((frameId+1)/total_frame*100)
            
            for result in results:
                
                x_min = result['topleft']['x']
                y_min = result['topleft']['y']
                x_max = result['bottomright']['x']
                y_max = result['bottomright']['y']
                
                
                # 0.Crop Images
                crop_img = frame[y_min:y_max,x_min:x_max]
                cv2.imwrite(cropdir+'{}_{}_{}_{}_{}.jpeg'.format(int(frameId), x_min, y_min, x_max, y_max), crop_img)
        
            #cv2.imshow('Video', frame)
            clone = imgs[i_g].copy()
            if percentage == 100:
                str_loc = 58
            cv2.putText(imgs[i_g], str(percentage) + '%', (str_loc, 110), cv2.FONT_HERSHEY_SCRIPT_SIMPLEX, 1, (76, 76, 76))
            cv2.imshow("Loading", imgs[i_g])
            imgs[i_g] = clone.copy()
            i_g = (i_g + 1) % nums

            print('{}%'.format(int((frameId + 1) / total_frame * 100)))
            
            if cv2.waitKey(1) & 0xFF == ord('q') or cv2.waitKey(1) & 0xFF == 27:
                break
        else:
            capture.release()
            cv2.destroyAllWindows()
            break





filename = sys.argv[1]
cropdir = sys.argv[2]
modeldir = sys.argv[3]
gifname = sys.argv[4]
print(os.path)
print(os.getcwd())
print(sys.path)
print(filename)
print(cropdir)

check_directory(cropdir)
processing(filename, cropdir, modeldir, gifname)
