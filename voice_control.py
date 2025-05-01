import speech_recognition as sr
import pyautogui
import time

# Initialize recognizer class (for speech-to-text)
recognizer = sr.Recognizer()

# === Function to recognize speech ===
def listen_for_command():
    with sr.Microphone() as source:
        print("🎙️ Listening... Speak now!")
        recognizer.adjust_for_ambient_noise(source)  # Adjust for ambient noise
        audio = recognizer.listen(source)
        
        try:
            print("🔍 Recognizing speech...")
            command = recognizer.recognize_google(audio)  # Use Google's API for speech-to-text
            print(f"🗣️ You said: {command}")
            return command.lower()  # Return recognized command in lowercase
        except sr.UnknownValueError:
            print("⚠️ Sorry, I didn't understand that.")
        except sr.RequestError:
            print("⚠️ There was an issue with the speech recognition service.")
        return None

# === Main Loop ===
print("Speak a command (Ctrl+C to quit)")

try:
    while True:
        # Listen for a speech command
        command = listen_for_command()
        if command:
            # Use pyautogui to type the command into the active window (Arduino Serial Monitor)
            print(f"➡️ Typing command into Arduino IDE: {command}")
            pyautogui.typewrite(command + '\n')  # Simulate typing the command and press Enter
            time.sleep(1)  # Slight delay before next speech command

except KeyboardInterrupt:
    print("\n🛑 Exiting speech-to-text control.")


