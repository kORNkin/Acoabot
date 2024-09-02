from ultralytics import YOLO
import requests
import time

#Line notify
def line(txt):
    url = 'https://notify-api.line.me/api/notify'
    token = 'FXkSbQJioSr5nZhb0HcZCIDz0zQdrltgmFcYgQ0o2YN'
    headers = {'content-type':'application/x-www-form-urlencoded','Authorization':'Bearer '+token}

    r = requests.post(url, headers=headers , data = {'message':txt})
    print(r.text)

#Predictation
model = YOLO("best.pt")
results = model.predict(source="0", stream=True, show=True, conf=0.8) # source already setup
names = model.names

for r in results:
    for c in r.boxes.cls:
        print(names[int(c)])
        n = names[int(c)]
        if n.find('BLACKPOD') != -1  or n.find('FROSTYPOD') != -1 or n.find('MIRID') != -1:
            line(n)
time.sleep(2)