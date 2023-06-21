import requests
import time
import serial

import requests
    # 這邊加上picurl
def lineNotify(token, msg, picurl):
    # line伺服器位址
    url = "https://notify-api.line.me/api/notify"
    # token認證用
    headers = {
        "Authorization": "Bearer " + "OWbgpbxq30zMuNAzDoaf9HeSiTP0t3hEodgeWs0hR19"
    }
    
    Buzzer() # beep
    
    # 宣告一個物件，裡面存放要傳送的訊息
    payload = { 'message': msg,}
    files = {'imageFile': open(picurl, 'rb')}
    # 將headers和data傳送至url，也就是將token和訊息傳送至line伺服器
    r = requests.post(url, headers=headers,  paramas=payload, files=files)
    # status_code為requests除錯用，回傳錯誤代碼，不用理會
    print(r.status_code)
    return r.status_code

# use rpi to send message to arduino with USB line and the buzzer will bee when someone faints
def Buzzer():
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1.0) # ttyUSB0 is the port showing in the tool of Arduino IDE
    ser.reset_input_buffer()
    # print("Serial OK")
    ser.write("H\n".encode('utf-8')) # send "H" to Arduino
    if KeyboardInterrupt:
        ser.close()
        
# test the code without rpi but with your laptop
"""
a = time.time()
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
Warning(0,now)
b = time.time()
print(b-a)
"""
