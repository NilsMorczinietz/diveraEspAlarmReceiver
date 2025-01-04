import asyncio
import json
import logging
import websockets
from notification_handler import show_notification
from my_secrets import websocket_url
from system_tray import TrayIcon

async def connect_to_server():
    uri = websocket_url
    tray_icon = TrayIcon()

    # Starte das Icon mit dem initialen Status (nicht verbunden)
    tray_icon.start_icon()

    while True:
        try:
            logging.info("Verbinde mit dem WebSocket-Server...")
            async with websockets.connect(uri) as websocket:
                logging.info("Erfolgreich mit dem Server verbunden.")
                tray_icon.update_connection_status(True)  # Icon auf grün setzen
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
                        tray_icon.update_connection_status(False)  # Icon auf rot setzen
                        break
                    except Exception as e:
                        logging.error(f"Unerwarteter Fehler beim Empfang von Nachrichten: {e}")

        except (websockets.exceptions.ConnectionClosedError, websockets.exceptions.ConnectionClosedOK):
            logging.warning("Verbindung zum Server abgebrochen. Versuche erneut zu verbinden...")
            tray_icon.update_connection_status(False)  # Icon auf rot setzen
        except Exception as e:
            logging.error(f"Fehler beim Verbindungsaufbau: {e}")

        logging.info("Warte 1 Sekunde vor erneutem Verbindungsversuch...")
        await asyncio.sleep(1)
