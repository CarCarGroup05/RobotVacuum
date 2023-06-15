import cv2
import mediapipe as mp
import math
import os
import time
from Line_notify import Warning
import requests
import urllib

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
        #get time
        url = "http://time.artjoey.com/js/basetime.php"
        res = requests.get(url)
        data = res.content.decode('ascii')
        ms = int(data.split('=')[1][:-1])
        twsec = ms / 1000 + (60 * 60 * 8)
        daysec = twsec % (60 * 60 * 24) 
        HH = int(daysec / 60 / 60)
        MM = int(daysec / 60) % 60
        SS = int(daysec % 60)
        now = f"{HH}{MM}{SS}"
        Warning(0,now)
    
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
            
            Ltmp = 0
            for i in L_test:
                if abs(i) < 50:
                    Ltmp += 1
                    print("???????????")
                    if Ltmp >= 2:
                        self.warning()
            Rtmp = 0
            for i in R_test:
                if abs(i) < 50:
                    Rtmp += 1
                    print("???????????")
                    if Rtmp >= 2:
                        self.warning()
                    
            if draw:
                self.mpDraw.draw_landmarks(img, self.results.pose_landmarks,
                                           self.mpPose.POSE_CONNECTIONS)
        if draw:
            return img
        else:
            return 0

def main():
    while True:
        path = "/home/usr/RobotVacuum/photos"
        pictures = os.listdir(path)
        IMAGE = cv2.imread("/home/usr/RobotVacuum/photos/"+pictures[-1])
        img = IMAGE
        detector = PoseDetector()
        img = detector.faint_detect(img, bboxWithHands=False)
        while True:
            cv2.imshow("Image", img)
            time.sleep(5)
            break
        print(1)    
        if cv2.waitKey(1) == 27:
            break
        
    return 0
       


if __name__ == "__main__":
    main()
    #cap.release()
    cv2.destroyAllWindows()
