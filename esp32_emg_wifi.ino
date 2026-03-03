/*
 * ESP32 - EMG WiFi Bridge
 * ========================
 * Recibe datos del STM32 por UART2 (pin 16)
 * Los reenvía por WebSocket a cualquier browser en la red
 *
 * Librerías necesarias (instalar en Arduino IDE):
 *   - "WebSockets" de Markus Sattler (buscar en Library Manager)
 *
 * Conexión física:
 *   STM32 TX (USART3) (TX) --> ESP32 RX GPIO 16 (RX)
 *   STM32 GND         --> ESP32 GND
 *
 * IMPORTANTE: El STM32 trabaja a 3.3V igual que el ESP32,
 *             no hace falta divisor de tensión.
 */

#include <WiFi.h>
#include <WebSocketsServer.h>

/* ── Configuración de red WiFi ─────────────────────────── */
const char* WIFI_SSID     = "Fibertel WiFi302 2.4GHz";
const char* WIFI_PASSWORD = "01434521724";
/* ──────────────────────────────────────────────────────── */

#define UART_BAUD   4800
#define UART_RX_PIN 16
#define UART_TX_PIN 17

WebSocketsServer webSocket(81);   /* Puerto WebSocket */

/* Buffer para armar líneas completas del STM32 */
String serial_buffer = "";

void setup()
{
    delay(3000);
    Serial.begin(115200);         /* Debug por USB */
    Serial2.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

    Serial.println("Conectando WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado!");
    Serial.print("IP del ESP32: ");
    Serial.println(WiFi.localIP());
    Serial.println("Abrí el browser en:  http://<esta IP>");
    Serial.println("WebSocket en puerto: 81");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop()
{
    webSocket.loop();

    /* Leer UART del STM32 caracter a caracter */
    while (Serial2.available())
    {
        char c = Serial2.read();

        if (c == '\n')
        {
            /* Línea completa recibida - enviar a todos los clientes */
            serial_buffer.trim();
            if (serial_buffer.length() > 0)
            {
                webSocket.broadcastTXT(serial_buffer);
                Serial.println("TX: " + serial_buffer);   /* debug */
            }
            serial_buffer = "";
        }
        else
        {
            serial_buffer += c;
        }
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_CONNECTED)
    {
        Serial.printf("Cliente %u conectado\n", num);
    }
    else if (type == WStype_DISCONNECTED)
    {
        Serial.printf("Cliente %u desconectado\n", num);
    }
}
