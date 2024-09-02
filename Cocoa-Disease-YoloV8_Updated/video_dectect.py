import cv2
import torch
import numpy as np
import math
from ultralytics import YOLO

def vid_detection(path_x):

    cap = cv2.VideoCapture(path_x)

    frame_width = int(cap.get(3))

    frame_height = int(cap.get(4))

    device = torch.device("cuda:0") if torch.cuda.is_available() else torch.device("cpu")

    model = YOLO("best.pt").to(device)

    count = 0
    classNames = ["Frostypod", "Mirid", "Blackpod", "Healthy"]

    out = cv2.VideoWriter('output.avi', cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'), 10, (frame_width, frame_height))

    while True:
        ret, frame = cap.read()
        count += 1
        if ret:
            result = list(model.predict(frame, conf=0.35))[0]
            bbox_xyxys = result.prediction.bboxes_xyxy.tolist()
            confidences = result.prediction.confidence
            labels = result.prediction.labels.tolist()
            for(bbox_xyxy, confidence, cls) in zip(bbox_xyxys, confidences, labels):
                bbox = np.array(bbox_xyxy)
                x1, y1, x2, y2 = bbox[0], bbox[1], bbox[2], bbox[3]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                classname = int(cls)
                class_name = classNames[classname]
                conf = math.celi((confidence * 100)) / 100
                label = f'{class_name}{conf}'
                print("Frame N", count, "", x1, y1, x2, y2)
                cv2.rectangle(frame, (x1,y1), (x2,y2), (0, 144, 30), 3)
                t_size = cv2.getTextSize(label, 0, fontScale = 1, thickness = 2)[0]
                c2 = x1 + t_size[0], y1 - t_size[1] - 3
                cv2.rectangle(frame, (x1,y1), c2, [0, 255, 255], -1, cv2.LINE_AA)
                cv2.putText(frame, label, (x1, y1-2), 0, 1, [255, 255, 255], thickness = 1, lineType = cv2.LINE_AA)
                resize_frame = cv2.resize(frame, (0,0), fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
            yield frame#
            # out.write(frame)
        else:
            break

def img_detection(path_x):

    img = cv2.imread(path_x)

    device = torch.device("cuda:0") if torch.cuda.is_available() else torch.device("cpu")

    model = YOLO("best.pt").to(device)

    classNames = ["Frostypod", "Mirid", "Blackpod", "Healthy"]

    result = list(model.predict(img, conf=0.35))[0]
    bbox_xyxys = result.prediction.bboxes_xyxy.tolist()
    confidences = result.prediction.confidence
    labels = result.prediction.labels.tolist()
    for(bbox_xyxy, confidence, cls) in zip(bbox_xyxys, confidences, labels):
        bbox = np.array(bbox_xyxy)
        x1, y1, x2, y2 = bbox[0], bbox[1], bbox[2], bbox[3]
        x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
        classname = int(cls)
        class_name = classNames[classname]
        conf = math.celi((confidence * 100)) / 100
        label = f'{class_name}{conf}'
        cv2.rectangle(img, (x1,y1), (x2,y2), (0, 144, 30), 3)
        t_size = cv2.getTextSize(label, 0, fontScale = 1, thickness = 2)[0]
        c2 = x1 + t_size[0], y1 - t_size[1] - 3
        cv2.rectangle(img, (x1,y1), c2, [0, 255, 255], -1, cv2.LINE_AA)
        cv2.putText(img, label, (x1, y1-2), 0, 1, [255, 255, 255], thickness = 1, lineType = cv2.LINE_AA)
        resize_frame = cv2.resize(img, (0,0), fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
    yield img
    # out.write(frame)