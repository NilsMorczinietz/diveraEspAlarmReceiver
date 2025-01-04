# system_tray.py
import threading
from pystray import Icon, MenuItem, Menu
from PIL import Image, ImageDraw
import logging

class TrayIcon:
    def __init__(self):
        self.icon = None
        self.connected = False

    # Funktion zum Erstellen eines einfachen Icons
    def create_icon(self):
        # Erstelle ein Bild für das Icon
        image = Image.new('RGBA', (32, 32), (255, 255, 255, 0))  # Transparenter Hintergrund
        draw = ImageDraw.Draw(image)

        # Wähle die Farbe des Icons abhängig vom Verbindungsstatus
        if self.connected:
            draw.rectangle([(0, 0), (32, 32)], fill="green")  # Grün, wenn verbunden
        else:
            draw.rectangle([(0, 0), (32, 32)], fill="red")  # Rot, wenn nicht verbunden

        return image

    # Funktion, die das Icon in der Taskleiste anzeigt
    def show_icon(self):
        self.icon = Icon("websocket_icon", self.create_icon(), menu=Menu(MenuItem('Beenden', self.quit_app)))
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
            self.icon.icon = self.create_icon()  # Update das Icon
