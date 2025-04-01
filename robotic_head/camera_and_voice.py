import cv2
import numpy as np
import subprocess
import os
import time
import pyttsx3
import RPi.GPIO as GPIO
import threading

# Define the temporary file for saving frames
temp_file = "/tmp/frame.jpg"

# Frame skipping and confidence threshold
frame_skip = 3
frame_counter = 0
confidence_threshold = 0.3

# Servo setup
SERVO_PIN = 18  # GPIO pin for the servo
GPIO.setmode(GPIO.BCM)
GPIO.setup(SERVO_PIN, GPIO.OUT)
servo = GPIO.PWM(SERVO_PIN, 50)  # 50Hz PWM frequency
servo.start(0)  # Start with the servo at 0%

# Initialize pyttsx3 for speech synthesis
engine = pyttsx3.init()
engine.setProperty('rate', 150)  # Speed of speech
engine.setProperty('volume', 1.0)  # Volume (0.0 to 1.0)

# Servo movement control during speech
servo_moving = False


def servo_movement_thread():
    """Control the servo to simulate mouth movement."""
    global servo_moving
    movements = 0
    while servo_moving and movements < 5:  # Perform 5 little movements
        servo.ChangeDutyCycle(7.5)  # Neutral position
        time.sleep(0.1)
        servo.ChangeDutyCycle(5)  # Slight open position
        time.sleep(0.1)
        servo.ChangeDutyCycle(7.5)  # Back to neutral
        time.sleep(0.1)
        movements += 1
    servo.ChangeDutyCycle(0)  # Stop servo after movements


def on_start(name):
    """Callback when speech starts."""
    global servo_moving
    print(f"Speech started: {name}")
    servo_moving = True
    threading.Thread(target=servo_movement_thread).start()


def on_end(name, completed):
    """Callback when speech ends."""
    global servo_moving
    print(f"Speech ended: {name}")
    servo_moving = False
    servo.ChangeDutyCycle(0)  # Ensure servo stops


# Bind the callbacks
engine.connect("started-utterance", on_start)
engine.connect("finished-utterance", on_end)


# Function to capture a frame using libcamera-still
def capture_frame():
    process = subprocess.Popen(
        ['libcamera-still', '--width', '640', '--height', '480', '-o', temp_file, '--immediate', '--nopreview']
    )
    process.wait()


# Function for text-to-speech with synchronized servo movement
def speak(text):
    print(f"Speaking: {text}")
    engine.say(text)
    engine.runAndWait()


# Load the pre-trained MobileNet SSD model
net = cv2.dnn.readNetFromCaffe("deploy.prototxt", "mobilenet_iter_73000.caffemodel")

# Classes for MobileNet SSD
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
           "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
           "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
           "sofa", "train", "tvmonitor"]

# Main detection loop
try:
    while True:
        start_time = time.time()

        # Capture a frame
        capture_frame()
        frame_counter += 1

        # Skip frames for speed
        if frame_counter % frame_skip != 0:
            continue

        print(f"Frame capture took {time.time() - start_time:.2f} seconds")

        # Read the frame
        frame = cv2.imread(temp_file)
        if frame is None:
            print("Error reading frame")
            continue

        # Preprocess for model input
        blob = cv2.dnn.blobFromImage(cv2.resize(frame, (224, 224)), 0.007843, (224, 224), 127.5)
        net.setInput(blob)
        detections = net.forward()

        # Process detections
        for i in range(detections.shape[2]):
            confidence = detections[0, 0, i, 2]
            if confidence > confidence_threshold:
                idx = int(detections[0, 0, i, 1])
                label = CLASSES[idx]
                print(f"I find a {label} with confidence of {confidence * 100:.2f}%")
                speak(f"I have found a {label} with a confidence of {confidence * 100:.2f} percent")

        time.sleep(2)

except KeyboardInterrupt:
    print("Terminating camera process.")
finally:
    if os.path.exists(temp_file):
        os.remove(temp_file)
    servo.stop()
    GPIO.cleanup()
    print("Cleaned up resources. Program terminated.")
