#define SERIAL_TX_BUFFER_SIZE 256
#include <SparkFunESP8266WiFi.h>

#include <Servo.h>


static inline void AddDevice(const char *const mac, const char *const ip, const char *const gateway)
{
  char buffer[150] = {0};

  sprintf(buffer, "GET /api/adddevice.php?name=Servo&type=1&mac=%s&ip=%s HTTP/1.1\r\nHost: %s\r\n\r\n", mac, ip, gateway);

  esp8266.tcpSend(0, buffer, strlen(buffer));
}

static inline char Get()
{
  int c = esp8266.read();

  while (c == -1)
  {
    c = esp8266.read();
    yield();
  }

  return c;
}

static inline bool GetState(const char*const mac, const char *const gateway)
{
  char buffer[150] = {0};

  sprintf(buffer, "GET /api/getdevicestate.php?mac=%s HTTP/1.1\r\nHost: %s\r\n\r\n", mac, gateway);

  esp8266.tcpSend(0, buffer, strlen(buffer));

  char b[4] = {0};

  while (!(b[0] == '\r' && b[1] == '\n' && b[2] == '\r' && b[3] == '\n'))
  {
    b[0] = b[1];
    b[1] = b[2];
    b[2] = b[3];
    b[3] = Get();
  }

  b[0] = 0;
  b[1] = 0;
  b[2] = 0;
  b[3] = 0;

  while (!(b[0] == '\r' && b[1] == '\n' && b[2] == '\r' && b[3] == '\n'))
  {
    b[0] = b[1];
    b[1] = b[2];
    b[2] = b[3];
    b[3] = Get();
  }

  return Get() == '1';
}

char mac[18] = {0};
char ipa[16] = {0};
char gatewaya[16] = {0};

Servo serv;

void setup()
{
  pinMode(2, OUTPUT);

  digitalWrite(2, HIGH);
  
  serv.attach(9);

  while (esp8266.begin(115200, ESP8266_HARDWARE_SERIAL) != true)
  {
    delay(1000);
  }

  delay(1000);

  esp8266.setMode(ESP8266_MODE_STA);

  delay(1000);

  while (esp8266.connect("CentralHub", "mynamejeff123") < 0)
  {
    delay(1000);
  }


  delay(1000);


  IPAddress ip = esp8266.localIP();
  IPAddress gateway = ip;

  gateway[3] = 1;

  delay(1000);



  esp8266.localMAC(mac);

  delay(1000);

  if (esp8266.ping(gateway) > 0)
  {
  }

  delay(1000);


  sprintf(ipa, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  sprintf(gatewaya, "%d.%d.%d.%d", gateway[0], gateway[1], gateway[2], gateway[3]);

  if (esp8266.tcpConnect(0, gatewaya, 80, 0) == 1)
  {
  }

  delay(1000);

  AddDevice(mac, ipa, gatewaya);

  delay(1000);
  esp8266.close(0);

  delay(1000);

  //AddTrackedDevice(mac, "AA:BB:CC:DD:EE:FF", "10", gatewaya);
}


void loop()
{

  if (esp8266.tcpConnect(0, gatewaya, 80, 0) == 1)
  {
  }

  delay(1000);

  serv.write(GetState(mac, gatewaya) ? 180 : 0);

  delay(1000);

  esp8266.close(0);

  delay(1000);
}
