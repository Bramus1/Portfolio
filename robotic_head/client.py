import pygame
import pyaudio
import wave
import requests
import json
import time
from pydub import AudioSegment
import speech_recognition as sr
import RPi.GPIO as GPIO
import threading

# Initialize pygame for playing audio
pygame.mixer.init()

# Servo setup
SERVO_PIN = 18  # GPIO pin for the servo
GPIO.setmode(GPIO.BCM)
GPIO.setup(SERVO_PIN, GPIO.OUT)
servo = GPIO.PWM(SERVO_PIN, 50)  # 50Hz PWM frequency
servo.start(0)  # Start with the servo at 0%

# Duty cycle values for the mouth movement
MOUTH_CLOSED = 2.5  # Fully retracted position
MOUTH_OPEN = 7.5  # Fully extended position (adjust based on your calibration)

# Global variable to manage servo movement
servo_moving = False

# Function to control servo for piston-based mouth movement
def move_servo_mouth():
    """Simulate 3 controlled piston movements."""
    print("Starting servo movement...")
    movements = 0
    while servo_moving and movements < 3:  # Perform exactly 3 movements
        # Open mouth
        servo.ChangeDutyCycle(MOUTH_OPEN)
        time.sleep(0.2)  # Hold the open position for 200ms
        
        # Close mouth
        servo.ChangeDutyCycle(MOUTH_CLOSED)
        time.sleep(0.2)  # Hold the closed position for 200ms
        
        movements += 1
    servo.ChangeDutyCycle(0)  # Stop the servo
    print("Servo movement complete.")

# Record audio and return the recognized text
def record_audio():
    chunk = 1024  # Record in chunks
    sample_format = pyaudio.paInt16  # 16-bit resolution
    channels = 1  # Mono
    rate = 44100  # 44.1kHz
    record_seconds = 5  # Duration of recording
    output_file = "output.wav"  # Name of the file to save

    # Initialize PyAudio object
    p = pyaudio.PyAudio()

    # Open a stream to record audio
    stream = p.open(format=sample_format, channels=channels, rate=rate,
                    input=True, frames_per_buffer=chunk)

    print("Recording...")

    frames = []

    # Record data for the specified time
    for _ in range(0, int(rate / chunk * record_seconds)):
        data = stream.read(chunk)
        frames.append(data)

    # Stop and close the stream
    stream.stop_stream()
    stream.close()
    p.terminate()

    # Save the recorded audio as a WAV file
    wf = wave.open(output_file, 'wb')
    wf.setnchannels(channels)
    wf.setsampwidth(p.get_sample_size(sample_format))
    wf.setframerate(rate)
    wf.writeframes(b''.join(frames))
    wf.close()

    # Transcribe the recorded audio locally for the exit check
    recognizer = sr.Recognizer()
    with sr.AudioFile(output_file) as source:
        audio = recognizer.record(source)
        try:
            text = recognizer.recognize_google(audio).lower()
            print(f"Recognized locally: {text}")
            return text, output_file
        except sr.UnknownValueError:
            print("Could not understand the command.")
            return None, output_file
        except sr.RequestError as e:
            print(f"Speech recognition error: {e}")
            return None, output_file

def send_audio_to_server(audio_file):
    url = 'http://192.168.1.129:5000/process_audio'
    files = {'file': open(audio_file, 'rb')}
    response = requests.post(url, files=files)

    if response.status_code == 200:
        server_response = json.loads(response.content)
        print("Server response:", server_response)
        download_and_play_audio(server_response['audio_file'])
        return server_response.get('text', '')  # Return the recognized text to check for exit condition
    else:
        print("Failed to process audio on the server.")
        return None

def download_and_play_audio(audio_file_name):
    global servo_moving
    url = f'http://192.168.1.129:5000/{audio_file_name}'
    response = requests.get(url)

    if response.status_code == 200:
        with open(audio_file_name, 'wb') as f:
            f.write(response.content)

        # Ensure the response audio file is in valid WAV format
        sound = AudioSegment.from_file(audio_file_name)
        wav_audio = audio_file_name.replace('.wav', '_converted.wav')
        sound.export(wav_audio, format="wav")

        # Play the properly formatted WAV file using pygame
        print("Playing response...")
        pygame.mixer.music.load(wav_audio)
        pygame.mixer.music.play()

        # Trigger servo movement during playback in a separate thread
        servo_moving = True
        threading.Thread(target=move_servo_mouth).start()

        # Wait until playback is finished
        while pygame.mixer.music.get_busy():
            time.sleep(0.1)

        servo_moving = False  # Stop servo after playback
    else:
        print(f"Failed to download: {response.status_code}")

if __name__ == "__main__":
    try:
        while True:
            recognized_text, audio_file = record_audio()
            if recognized_text:
                if "exit" in recognized_text:
                    print("Exit keyword detected. Terminating the program.")
                    break
                print("Sending audio to server...")
                send_audio_to_server(audio_file)
            print("Waiting for the next command...")
    except KeyboardInterrupt:
        print("Program interrupted.")
    finally:
        print("Cleaning up GPIO.")
        servo.stop()
        GPIO.cleanup()
