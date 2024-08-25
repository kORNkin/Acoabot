import streamlit as st
import cv2
from ultralytics import YOLO
import numpy as np

# Load the YOLOv8 model
model = YOLO('yolov8n.pt')  # Adjust the path to your .pt file if necessary

# Function to process and display the live camera feed with YOLOv8 predictions
def process_frame():
    camera = cv2.VideoCapture(0)  # Use 0 for the default webcam

    while camera.isOpened():
        success, frame = camera.read()
        if not success:
            st.error("Failed to capture video", icon="🚨")
            break

        # Perform YOLOv8 predictions on the frame
        results = model(frame)

        # Draw bounding boxes and labels on the frame
        for result in results:
            boxes = result.boxes.xyxy.cpu().numpy()
            confidences = result.boxes.conf.cpu().numpy()
            class_ids = result.boxes.cls.cpu().numpy()

            for box, conf, cls in zip(boxes, confidences, class_ids):
                x1, y1, x2, y2 = map(int, box)
                label = f'{model.names[int(cls)]}: {conf:.2f}'
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        # Convert the frame to RGB format (required by Streamlit)
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Display the frame in the Streamlit app
        st.image(frame, channels="RGB")

    camera.release()

# Streamlit interface
st.title("YOLOv8 Live Camera Feed")

# Button to start the camera
if st.button('Start Camera'):
    process_frame()
