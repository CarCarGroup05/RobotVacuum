# PYTHON dir
## Final version
* faint_detection.py:<br>
It is the main code for detecting someone fainting away with the Google mediapipe module. We used pi cam to capture images every five seconds and judge a person's joints(shoulder, waist, kneel, and heel) from the LHS to the RHS. Comparing the y-index of any two of the four joints, it will send a warning message with a picture to the Line group of the person's family so that their family members can be notified instantly and maybe save the person's life.<br>
Use `python faint_detection.py` to run the code in terminal, and maybe you should change the path.
* Line_notified.py:<br>
This is the code for sending messages with pictures via Line notify and "H" to Arduino with Serial. If Arduino gets the message "H", the buzzer will beep nonstop.
## Other verison
Before finishing the final version, we made 5 test versions.
## Outcome
<picture>
    <img alt="line", src="https://hackmd.io/_uploads/rkqPJll_h.png", width="200">
</picture>