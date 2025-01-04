# system_tray.py
import threading
from pystray import Icon, MenuItem, Menu
from PIL import Image
import logging

class TrayIcon:
    def __init__(self, connected=False):
        self.icon = None
        self.connected = connected

    # Funktion zum Laden des benutzerdefinierten Icons
    def load_icon(self, connected):
        # Lade das Bild aus einer Datei
        if connected:
            # Verwende das "verbunden"-Icon (grün)
            image = Image.open("./icon_connected.png")  # Dein benutzerdefiniertes Bild für verbunden
        else:
            # Verwende das "nicht verbunden"-Icon (rot)
            image = Image.open("./icon_disconnected.png")  # Dein benutzerdefiniertes Bild für getrennt

        return image

    # Funktion, die das Icon in der Taskleiste anzeigt
    def show_icon(self):
        self.icon = Icon("websocket_icon", self.load_icon(self.connected), menu=Menu(MenuItem('Beenden', self.quit_app)))
        self.icon.run()

    # Funktion zum Beenden des Icons
    def quit_app(self, icon, item):
        icon.stop()
        logging.info("Taskleistensymbol wurde beendet.")

    # Funktion zum Starten des Icons
    def start_icon(self):
        if self.icon is None:
            icon_thread = threading.Thread(target=self.show_icon)
            icon_thread.daemon = True
            icon_thread.start()

    # Funktion zum Aktualisieren des Verbindungsstatus
    def update_connection_status(self, connected):
        self.connected = connected
        if self.icon is not None:
            self.icon.icon = self.load_icon(self.connected)  # Update das Icon