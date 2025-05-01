import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
import serial.tools.list_ports
import speech_recognition as sr
import serial
import threading
import time

ser_robot = None  # Serial connection for robot
recognizer = sr.Recognizer()

# === Speech Recognition Thread ===
def listen_and_send(output_box, barcode_entry):
    global ser_robot
    with sr.Microphone() as source:
        output_box.insert(tk.END, "\n🎤 Listening...\n")
        output_box.see(tk.END)
        recognizer.adjust_for_ambient_noise(source)
        audio = recognizer.listen(source)

    try:
        command = recognizer.recognize_google(audio)
        output_box.insert(tk.END, f"🗣️ You said: {command}\n")
        output_box.see(tk.END)
        if ser_robot and ser_robot.is_open:
            ser_robot.write((command + '\n').encode())  # Send command to robot
            output_box.insert(tk.END, "✅ Sent command to robot.\n")
            output_box.see(tk.END)
        else:
            output_box.insert(tk.END, "⚠️ Serial not connected.\n")

        # Move focus to barcode entry field after sending voice command
        barcode_entry.focus()

    except sr.UnknownValueError:
        output_box.insert(tk.END, "⚠️ Could not understand audio.\n")
    except sr.RequestError:
        output_box.insert(tk.END, "⚠️ Error with speech recognition service.\n")

# === Barcode Scanner Input ===
def barcode_input(event, output_box, barcode_entry):
    barcode = barcode_entry.get()  # Get text from the entry field
    output_box.insert(tk.END, f"📦 Barcode Scanned: {barcode}\n")
    output_box.see(tk.END)
    if ser_robot and ser_robot.is_open:
        ser_robot.write((barcode + '\n').encode())  # Send barcode data to robot
        output_box.insert(tk.END, "✅ Sent barcode to robot.\n")
        output_box.see(tk.END)
    barcode_entry.delete(0, tk.END)  # Clear entry after sending barcode

# === GUI Setup ===
def create_gui():
    def start_listening():
        threading.Thread(target=listen_and_send, args=(output_box, barcode_entry), daemon=True).start()

    def connect_serial():
        global ser_robot
        robot_port = port_combo_robot.get()

        try:
            # Connect to robot serial port
            ser_robot = serial.Serial(robot_port, 9600, timeout=1)
            time.sleep(2)  # wait for robot initialization
            output_box.insert(tk.END, f"🔌 Connected to Robot on {robot_port}\n")
        except Exception as e:
            messagebox.showerror("Connection Failed", f"Could not open serial port:\n{str(e)}")
            output_box.insert(tk.END, f"⚠️ Error: {str(e)}\n")
            output_box.see(tk.END)

    root = tk.Tk()
    root.title("Robot Control - Voice & Barcode")
    root.geometry("500x350")

    # Port selection for robot
    ttk.Label(root, text="Robot Serial Port:").pack(pady=5)
    port_combo_robot = ttk.Combobox(root, values=[port.device for port in serial.tools.list_ports.comports()])
    port_combo_robot.pack()
    if port_combo_robot['values']:
        port_combo_robot.current(0)

    connect_btn = ttk.Button(root, text="Connect", command=connect_serial)
    connect_btn.pack(pady=5)

    # Start listening button for voice recognition
    listen_btn = ttk.Button(root, text="Start Listening", command=start_listening)
    listen_btn.pack(pady=5)

    # Barcode scanner input field
    ttk.Label(root, text="Barcode Scanner Input:").pack(pady=5)
    barcode_entry = ttk.Entry(root)
    barcode_entry.pack(pady=5)
    barcode_entry.bind("<Return>", lambda event: barcode_input(event, output_box, barcode_entry))  # Trigger on Enter key

    # Output log
    output_box = scrolledtext.ScrolledText(root, wrap=tk.WORD, height=10)
    output_box.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

    root.mainloop()

if __name__ == "__main__":
    try:
        create_gui()
    except Exception as e:
        print(f"Error during GUI setup: {str(e)}")
