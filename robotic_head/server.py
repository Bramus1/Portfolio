from flask import Flask, request, jsonify, send_file
from transformers import T5Tokenizer, T5ForConditionalGeneration
import torch
import os
from gtts import gTTS
import speech_recognition as sr

app = Flask(__name__)

# Initialize the AI model
tokenizer = T5Tokenizer.from_pretrained("google/flan-t5-small")
model = T5ForConditionalGeneration.from_pretrained("google/flan-t5-small")

# Initialize speech recognition
recognizer = sr.Recognizer()

# Absolute directory path
app_dir = os.path.dirname(os.path.realpath(__file__))

@app.route('/process_audio', methods=['POST'])
def process_audio():
    # Save the uploaded audio file
    if 'file' not in request.files:
        return jsonify({"error": "No file provided"}), 400

    file = request.files['file']
    file_path = os.path.join(app_dir, "received_audio.wav")
    file.save(file_path)

    # Perform speech-to-text on the received audio
    try:
        with sr.AudioFile(file_path) as source:
            audio = recognizer.record(source)
            recognized_text = recognizer.recognize_google(audio)
            print(f"Recognized text: {recognized_text}")
    except sr.UnknownValueError:
        return jsonify({"error": "Could not understand the audio"}), 400
    except sr.RequestError:
        return jsonify({"error": "Could not request results from Google Speech Recognition service"}), 500

    # Generate an AI response
    response_text = get_ai_response(recognized_text)

    # Convert text to speech (TTS)
    tts = gTTS(response_text)
    response_path = os.path.join(app_dir, "response.wav")
    tts.save(response_path)

    return jsonify({
        "message": "Audio received and processed.",
        "recognized_text": recognized_text,
        "response": response_text,
        "audio_file": "response.wav"
    })

@app.route('/response.wav', methods=['GET'])
def get_audio_response():
    try:
        return send_file(os.path.join(app_dir, "response.wav"), mimetype="audio/wav")
    except FileNotFoundError:
        return "File not found", 404

def get_ai_response(input_text):
    inputs = tokenizer(input_text, return_tensors="pt")
    outputs = model.generate(inputs.input_ids, max_new_tokens=50)
    response = tokenizer.decode(outputs[0], skip_special_tokens=True)
    print(f"AI response: {response}")
    return response

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
