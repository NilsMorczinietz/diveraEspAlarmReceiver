import asyncio
import websockets
import json
from win10toast_click import ToastNotifier
import logging
from tkinter import Tk, Label, Button
from threading import Thread
from my_secrets import websocket_url

# Logging einrichten
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
    handlers=[logging.FileHandler("websocket_notifier.log"), logging.StreamHandler()]
)

def show_notification(title, text):
    try:
        # Benachrichtigung anzeigen
        notifier = ToastNotifier()
        notifier.show_toast(
            title=title,
            msg=text,
            duration=30,  # Dauer in Sekunden
            threaded=True,
            icon_path=''
        )
        # Fenster mit Benachrichtigung anzeigen
        Thread(target=show_popup, args=(title, text)).start()
    except Exception as e:
        logging.error(f"Fehler beim Anzeigen der Benachrichtigung: {e}")

def show_popup(title, text):
    def close_window():
        root.destroy()

    # Neues Tkinter-Fenster erstellen
    root = Tk()
    root.title(title)
    root.geometry("400x200")
    root.configure(bg="red")

    # Fenster immer im Vordergrund
    root.attributes('-topmost', True)

    # Titel und Text als Label hinzufügen
    label_title = Label(root, text=title, font=("Helvetica", 16), bg="red", fg="white")
    label_title.pack(pady=10)

    label_text = Label(root, text=text, font=("Helvetica", 12), bg="red", fg="white", wraplength=380)
    label_text.pack(pady=10)

    # Button zum Schließen des Fensters
    close_button = Button(root, text="Schließen", command=close_window, bg="white", fg="black")
    close_button.pack(pady=10)

    # Fenster anzeigen
    root.mainloop()

async def connect_to_server():
    uri = websocket_url

    while True:
        try:
            logging.info("Verbinde mit dem WebSocket-Server...")
            async with websockets.connect(uri) as websocket:
                logging.info("Erfolgreich mit dem Server verbunden.")
                while True:
                    try:
                        # Empfangene Nachricht abrufen
                        message = await websocket.recv()

                        # Nachricht als JSON laden
                        data = json.loads(message)
                        title = data.get("title", "Neue Nachricht")
                        text = data.get("text", "Keine weiteren Details.")

                        # Benachrichtigung anzeigen
                        show_notification(title, text)
                    except json.JSONDecodeError:
                        logging.warning("Fehler beim Dekodieren der Nachricht: Ungültiges JSON.")
                    except websockets.exceptions.ConnectionClosedError:
                        logging.warning("Die Verbindung wurde geschlossen. Versuche erneut zu verbinden...")
                        break
                    except Exception as e:
                        logging.error(f"Unerwarteter Fehler beim Empfang von Nachrichten: {e}")

        except (websockets.exceptions.ConnectionClosedError, websockets.exceptions.ConnectionClosedOK):
            logging.warning("Verbindung zum Server abgebrochen. Versuche erneut zu verbinden...")
        except Exception as e:
            logging.error(f"Fehler beim Verbindungsaufbau: {e}")

        logging.info("Warte 5 Sekunden vor erneutem Verbindungsversuch...")
        await asyncio.sleep(5)

if __name__ == "__main__":
    try:
        asyncio.run(connect_to_server())
    except KeyboardInterrupt:
        logging.info("Programm wurde vom Benutzer beendet.")