import cv2

from ultralytics import YOLO

# Load the YOLOv8 model
model = YOLO("best2.pt")

def webcam_detection():
    cap = cv2.VideoCapture(0)

    # Loop through the video frames
    while cap.isOpened():
        # Read a frame from the video
        success, frame = cap.read()

        if success:
            # Run YOLOv8 inference on the frame
            results = model(frame, conf=0.5)
            #results = model.predict(vid_path, stream=True, show=True, conf=0.8)

            # Visualize the results on the frame
            annotated_frame = results[0].plot()

            yield annotated_frame

def vid_detection(vid_path):
    cap = cv2.VideoCapture(vid_path)

    # Loop through the video frames
    while cap.isOpened():
        # Read a frame from the video
        success, frame = cap.read()

        if success:
            # Run YOLOv8 inference on the frame
            results = model(frame, conf=0.5)
            #results = model.predict(vid_path, stream=True, show=True, conf=0.8)

            # Visualize the results on the frame
            annotated_frame = results[0].plot()

            yield annotated_frame

def img_detection(img_path):
    img = cv2.imread(img_path)
    results = model(img, conf=0.5)
    #results = model.predict(img_path, stream=True, show=True, conf=0.8)
    annotated_frame = results[0].plot()
    yield annotated_frame
