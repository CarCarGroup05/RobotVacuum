import cv2
import mediapipe as mp
import os
import time
from Line_notify import Warning
import requests
import sys
import argparse
import gc
counter = 0

class PoseDetector:
    """
    Estimates Pose points of a human body using the mediapipe library.
    """

    def __init__(self, mode=False, smooth=True,
                 detectionCon=0.5, trackCon=0.5):
        """
        :param mode: In static mode, detection is done on each image: slower
        :param smooth: Smoothness Flag
        :param detectionCon: Minimum Detection Confidence Threshold
        :param trackCon: Minimum Tracking Confidence Threshold
        """

        self.mode = mode
        self.smooth = smooth
        self.detectionCon = detectionCon
        self.trackCon = trackCon

        self.mpDraw = mp.solutions.drawing_utils
        self.mpPose = mp.solutions.pose
        self.pose = self.mpPose.Pose(static_image_mode=self.mode,
                                     smooth_landmarks=self.smooth,
                                     min_detection_confidence=self.detectionCon,
                                     min_tracking_confidence=self.trackCon)
        self.lmList = []
        self.t = 0

    def warning(self):
        url = "http://time.artjoey.com/js/basetime.php"
        res = requests.get(url)
        data = res.content.decode('ascii')
        ms = int(data.split('=')[1][:-1])
        twsec = ms / 1000 + (60 * 60 * 8)
        daysec = twsec % (60 * 60 * 24) 
        HH = int(daysec / 60 / 60)
        MM = int(daysec / 60) % 60
        SS = int(daysec % 60)
        now = f"{HH}:{MM}:{SS}"
        img = "image{}.jpeg".format(counter) 
        Warning(0,now,img)
    
    def faint_detect(self, img, draw=True, bboxWithHands=False):

        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.pose.process(imgRGB)
        joint_list = []
        
        if self.results.pose_landmarks:
            for id, lm in enumerate(self.results.pose_landmarks.landmark):
                h, w, c = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                joint_list.append([cx, cy])
            shoulder_L_y = joint_list[11][1]
            shoulder_R_y = joint_list[12][1]
            hip_L_y = joint_list[23][1]
            hip_R_y = joint_list[24][1]          
            knee_L_y = joint_list[25][1]
            knee_R_y = joint_list[26][1]
            heel_L_y = joint_list[27][1]
            heel_R_y = joint_list[28][1]
            print(shoulder_L_y,hip_L_y,knee_L_y,heel_L_y,"///",shoulder_R_y,hip_R_y,knee_R_y,heel_R_y)
            L_test = [shoulder_L_y - hip_L_y, hip_L_y - knee_L_y, knee_L_y - heel_L_y]
            R_test = [shoulder_R_y - hip_R_y, hip_R_y - knee_R_y, knee_R_y - heel_R_y]
            
            Ltmp = 0 # left hand side test
            for i in L_test:
                if abs(i) < 50:
                    Ltmp += 1
                    print("HELP!") 
                    if Ltmp == 2:
                        self.warning() # if any two sets of the y-difference oftwo key points, text messages to line group
            if Ltmp < 2:
                Rtmp = 0 # right hand side test
                for i in R_test:
                    if abs(i) < 50:
                        Rtmp += 1
                        print("HELP!")
                        if Rtmp == 2:
                            self.warning()
                    
            if draw:
                self.mpDraw.draw_landmarks(img, self.results.pose_landmarks,
                                           self.mpPose.POSE_CONNECTIONS)
        if draw:
            return img
        else:
            return 0
def run(camera_id: int):
    
    global counter
   # path = "/home/pi/RobotVacuum/photos"
    
    # Start capturing video input from the camera
    cap = cv2.VideoCapture(camera_id)
    
    while cap.isOpened():
        success, image = cap.read()
        if not success:
           sys.exit('ERROR: Unable to read from webcam. Please verify your webcam settings.')
        image_name = "/home/pi/RobotVacuum/python/image{}.jpeg".format(counter) 
        cv2.imwrite(image_name, image)

        img = cv2.imread(image_name)
        detector = PoseDetector()
        img = detector.faint_detect(img, bboxWithHands=False)
        
        while True:
            cv2.imshow("Image", img)
            cv2.waitKey(5000)
            cv2.destroyAllWindows()
            os.remove("/home/pi/RobotVacuum/python/image{}.jpeg".format(counter))
            gc.collect()
            counter += 1
            if counter > 39:
                counter = 0
            break
        print("\n")
        if cv2.waitKey(1) == 27:
            break
    cap.release()
    cv2.destroyAllWindows()

def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--cameraId', help='Id of camera.', required=False, default=-1)
    args = parser.parse_args()
    run(int(args.cameraId))

if __name__ == "__main__":
    main()
