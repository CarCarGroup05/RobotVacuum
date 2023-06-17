import requests
import time
def Warning(self,time,picurl):
    headers = {
        "Authorization": "Bearer " + "OWbgpbxq30zMuNAzDoaf9HeSiTP0t3hEodgeWs0hR19",
       # "Content-Type": "application/x-www-form-urlencoded"
    }
 
    params = {"message": "Your RobotVacuum detected someone fainted at" + time + ", please confirm his/her safety as soon as possible! "}
    files = {"imageFile": open(picurl, "rb")}
    s = requests.post("https://notify-api.line.me/api/notify", headers=headers, params=params)
    print(s.status_code)
    r = requests.post("https://notify-api.line.me/api/notify", headers=headers, params="picture:", files=files)
   # print(r.status_code)  #200
    return 0
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
