#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>

// Konfigurasi WiFi
const char* ssid = "NLA";
const char* password = "Naufal27";

// Konfigurasi MQTT
const char* mqtt_server = "31.187.75.181"; //IP Server : 31.187.75.181 
const char* mqtt_username = "";
const char* mqtt_password = "";
const char* mqtt_cacert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDuzCCAqOgAwIBAgIUZRPyH/AusVtK7clIe57z7MBpEjIwDQYJKoZIhvcNAQEL
BQAwbTELMAkGA1UEBhMCSUQxEjAQBgNVBAgMCUVhc3QgSmF2YTEPMA0GA1UEBwwG
TWFsYW5nMRQwEgYDVQQKDAtKYXRpbm9teFVNTTELMAkGA1UECwwCQ0ExFjAUBgNV
BAMMDTMxLjE4Ny43NS4xODEwHhcNMjIxMTA2MTAwMDI2WhcNMjMxMTA2MTAwMDI2
WjBtMQswCQYDVQQGEwJJRDESMBAGA1UECAwJRWFzdCBKYXZhMQ8wDQYDVQQHDAZN
YWxhbmcxFDASBgNVBAoMC0phdGlub214VU1NMQswCQYDVQQLDAJDQTEWMBQGA1UE
AwwNMzEuMTg3Ljc1LjE4MTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AMtLq9Ndyj/OD3rqVgS6M44olG6u8Uq6bziesgfYrRJEKbD2KmWP6fNVJr1J7XwK
ZMyPtJcw73MJuiXF0ghTocu6VeBxjBHjU531JlxM5cRANBTwTShn4eZg9nWs+5Xo
numfKf1itIbO+LLg28vdEIHkfagaMX598rMIfhe8HEGTd5pxLkI7uqV/8/f95VHy
aksrSFkhhgCP+dCjBL88lIHDEWNm+jyUDC6ZkFy89bXnwhEJaeHAR1t5jXxP6IZN
wFD5ZCwsudGckuwTW9FXgQylrk8KA83cFRevSssC0AiHoTlO9Rm4tn/mOpbJinxJ
y7sf6Eqe1mIFtBpwQr+9AZUCAwEAAaNTMFEwHQYDVR0OBBYEFFaEd8d8MgpJqLmM
u5qg277lYBGNMB8GA1UdIwQYMBaAFFaEd8d8MgpJqLmMu5qg277lYBGNMA8GA1Ud
EwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAMMEvXHgSssgUKgVHF0URdLG
kXAlI7+Iwh7mwRrX7uGfB7g+Oqez6HyNgVJN0JnAK8jqBSvh5Xr21srYViyvtRmW
uwTOSQoovhMCKT1bQ9azIk1gSt/cc0qh0TDSggEQ7Oq3qrszOxZA+jljWjZOMe+H
l1mL2L1m7FaVmndKlSITE4kqohM89OKe9TsThCtaGpcXlric/ExE0CU8Jmv/wng5
4G82JY0vX8ajrJbTH245fUJU67i36aWDmY5eWHhNSZAgTZBtSWXGF8/CyG1ibKvX
S2KhrSCLZRT6U+3d2Chb4ytKVUv4Ac7SEU23DUDvWTAR7RpkLv0k9/W3/O0VSCk=
-----END CERTIFICATE-----
)EOF";
const char* mqtt_client_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDZTCCAk0CFCokTyiDi5NrZA1pmLUhV3u3n8SmMA0GCSqGSIb3DQEBCwUAMG0x
CzAJBgNVBAYTAklEMRIwEAYDVQQIDAlFYXN0IEphdmExDzANBgNVBAcMBk1hbGFu
ZzEUMBIGA1UECgwLSmF0aW5vbXhVTU0xCzAJBgNVBAsMAkNBMRYwFAYDVQQDDA0z
MS4xODcuNzUuMTgxMB4XDTIyMTEwNjEwMDAyN1oXDTIzMTEwNjEwMDAyN1owcTEL
MAkGA1UEBhMCSUQxEjAQBgNVBAgMCUVhc3QgSmF2YTEPMA0GA1UEBwwGQmxpdGFy
MRQwEgYDVQQKDAtKYXRpbm9teFVNTTEPMA0GA1UECwwGQ2xpZW50MRYwFAYDVQQD
DA0zMS4xODcuNzUuMTgxMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA
rnMVFlyUYSo7diTADRyem6lJn8nBKsj6fwwqPLIAFjpYoiL24oINsZX7wB8MOC4F
h2HOSvs/7snePBF8f0vTAjqjkXjTj9Vr38CqPZeLEDU01QorjxaY21B9mwC9XGwu
umgrdyH4fmX9Fu8YdJ2UYdkrHm3+b70XvvWsMELJ8KO/YLv5hgPxqqRN7u5UqHA9
TUBk9MMy4+VWlYwjL7MH/anpbxQZDvSgCrNToubD7vbefwX6lG9Trv3Gp5iUXA+N
Jr4GnDP7nT20NUVGU743g13aCuFvV/iD1Pj4V8mGaujkaYXdD9WprDRAzRNkoyF4
Q/gYhrPvtQ72rlZEpcU00wIDAQABMA0GCSqGSIb3DQEBCwUAA4IBAQCd/PNkXUV7
U63s1LPVxJSbJbmqCxdH81P6hxeNc3ngy21tegRj+zmhlZkwmmXuKyEhf1s3/o/c
1YtBYy2Uz5uldTK77sdiHG53ZwMuoEYvxiG1f+erhLs1v+2ONvqA9xcFS9qGXYEi
PdjO0cvWQxSc60KWWESerXOjFiQSRIS5I2H8AO5ugSsoyGbnverxoKl2r5xDj3KJ
FbWHWERK20DlgLvtrZCEH6PLVtYsAYfRAi5Tln2DWiap5XXhwQM2+SYA0sID/U36
5RvlmwTYSX4Qyvzl5xYFNxuhFEmRzlyjlR7Nca/B0L2Jsrsu9YHFs66zYd9bdcF5
0PFGov2vXSd+
-----END CERTIFICATE-----
)EOF";
const char* mqtt_client_key = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCucxUWXJRhKjt2
JMANHJ6bqUmfycEqyPp/DCo8sgAWOliiIvbigg2xlfvAHww4LgWHYc5K+z/uyd48
EXx/S9MCOqOReNOP1WvfwKo9l4sQNTTVCiuPFpjbUH2bAL1cbC66aCt3Ifh+Zf0W
7xh0nZRh2Ssebf5vvRe+9awwQsnwo79gu/mGA/GqpE3u7lSocD1NQGT0wzLj5VaV
jCMvswf9qelvFBkO9KAKs1Oi5sPu9t5/BfqUb1Ou/canmJRcD40mvgacM/udPbQ1
RUZTvjeDXdoK4W9X+IPU+PhXyYZq6ORphd0P1amsNEDNE2SjIXhD+BiGs++1Dvau
VkSlxTTTAgMBAAECggEATvgTjKreFFoMzr92HLle5zIr/ORUyCxwkMCOAinFtko1
Qzg2sHSVBFXTv7WOT4Qtxoo022Z/G3ZQXrq/s9Q8Md8fmkOq6YqAKarKpdULA4xC
XUocS8q76VO3eEpObh8ezA1J18UN+xHqDs6vMGMA/4ZSaZT7P9PGgxAtIS+qwH5w
NKN1kdIw27rKL1wTHzCq/gPWiaTjROxzf9V5Ujn9/jtcMIA8KAJ0WWvj9b5QBpuo
cKpWcHio16lCAjNj7vjA/KK5YpP19YXL3rFYxsZ8WuTBgvniqfiHM4zMBwnF7BOo
iLlB+6R2UZF9Hk721dlSkMW4XpTkXdxjLeAb405hYQKBgQDbmRTGLfsT2WKQvIV4
a1XW6oZmKhTbEqzaYIcCtNO6ZuF48m6UYDqfVr/2LKBurQcsAFNEdNggdLQc8CLY
Nh3GkT7ZUVwCDXOS/kMQzbjenwGTq3U1Aekp+b7jx5ytzgH+At6hwaXJ9sFKrFbO
1pmbv7dqJhO1AQDEq4rJkWZKkQKBgQDLXhH1TFGAIbwnBepnsz8FNRAOrHvsgdgE
QorF9o8wP4FJ6EYb7ntaLedkqW82DixIJ9A9Rja05Qf9VmM1A1yUIoH2xV/Gdun3
waIuIs0RJ6hwy7kveS3GmYz5qusk9vEYJza6T0MFr213pxUpazo4vmkSpSoqiwsb
FMIYJtxTIwKBgF+yhmsW/qPXyCxq+39Ox0mxSoCbNhuCN/GyvyNeyiYhT4D1pVrj
1Kg7lcwiBog0vztdqOvcP5NlSFiUDZtLeg5enZT277G1Svhz9aYNANODV5ySy7Ed
9A5m19lL0+uqKxQXDA5R5X3uGq4ADJR20Noe4j9P+KaYmU0btI/C3WEBAoGALMsM
g44KyYEksyevaKXndJsqbUD6jq5OySlq7Y2QHl1uebvqbU6K5uMDe32CXFKk2EPE
rype9FsZ9mfntA6IfqxXGeaBYPJiOErzENxjdKrag/WrVVSIi5zYm0lVP7AQjHRe
roy0w3TPVLJ9i8DtcwAlnpC4h+RWTuG0mqM8licCgYEA0aXdiYKxfNieYOQIsI3o
RNMeWJAkNPGrXA2EhyO6b0zzZ80oC/c4pTcNoJm/s3oyVVXbwm8fmWdZEWfMGKMA
ZrtRet/ebmnLBVLfYAO/KDmc8v6atDsFm63pcSCMTlBa5aaDYafZw14NsjwIw5yZ
6K1hZgKj4Mz0EbPYGKIzMw0=
-----END PRIVATE KEY-----
)EOF";
void callback(char* topic, byte* payload, unsigned int length);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
//sensor value
int value = 0;
//timestamp
char* times;

// Sensor PZEM-004Tv30
PZEM004Tv30 pzem1(&Serial2,15, 16, 17);
PZEM004Tv30 pzem2(&Serial2,18, 19, 20);
PZEM004Tv30 pzem3(&Serial2,21, 22, 23);

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setClock();
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float voltage1    = pzem1.voltage();
  float current1    = pzem1.current();
  float power1      = pzem1.power();
  float energy1     = pzem1.energy();
  
  float voltage2    = pzem2.voltage();
  float current2    = pzem2.current();
  float power2      = pzem2.power();
  float energy2     = pzem2.energy();

  float voltage3    = pzem3.voltage();
  float current3    = pzem3.current();
  float power3      = pzem3.power();
  float energy3     = pzem3.energy();
    
  // Kirim data
  client.publish("sensor1/voltage", String(voltage1).c_str());
  client.publish("sensor1/current", String(current1).c_str());
  client.publish("sensor1/power", String(power1).c_str());
  client.publish("sensor1/energy", String(energy1).c_str());

  client.publish("sensor2/voltage", String(voltage2).c_str());
  client.publish("sensor2/current", String(current2).c_str());
  client.publish("sensor2/power", String(power2).c_str());
  client.publish("sensor2/energy", String(energy2).c_str());

  client.publish("sensor3/voltage", String(voltage3).c_str());
  client.publish("sensor3/current", String(current3).c_str());
  client.publish("sensor3/power", String(power3).c_str());
  client.publish("sensor3/energy", String(energy3).c_str());

  delay(2000);

   unsigned long now = millis();
  //change the 2000 for the sending period
  if (now - lastMsg > 2000) {
    lastMsg = now;
    //value is the sensor value
    ++value;
    setClock();
    snprintf(msg, MSG_BUFFER_SIZE, "%ld,%s", value,times);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("jatinom/volts", msg);
    client.publish("jatinom/amperes", msg);
    client.publish("jatinom/watts", msg);
    client.publish("jatinom/kwhs", msg);
  }
}

void setClock() {
  configTime(7*3600, 0 , "pool.ntp.org", "time.nis.gov");
  //configTime("CET-1CEST,M3.5.0,M10.5.0/3" , "pool.ntp.org", "time.nis.gov");
  // Wait till time is synced
  Serial.print("Date Time");
  int i = 0;
  while (time(nullptr) < 1000000000ul && i<100) {
    Serial.print(".");
    delay(100);
    i++;
  }
  Serial.println();

  time_t tnow = time(nullptr);
  struct tm *timeinfo;
  char buffer [80];

  timeinfo = localtime (&tnow);
  strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
  times = buffer;
  Serial.println(times);
}

void setup_wifi() {
  delay(10);
  // Menghubungkan ke jaringan WiFi
  Serial.println();
  Serial.print("Menghubungkan ke jaringan ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    espClient.setCACert(mqtt_cacert);
    espClient.setCertificate(mqtt_client_cert);
    espClient.setPrivateKey(mqtt_client_key);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi terhubung");
  Serial.println("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Jika tidak terhubung, coba lagi
  while (!client.connected()) {
    Serial.print("Mencoba untuk terhubung ke MQTT server");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("terhubung");
    } else {
      Serial.print("Kesalahan, rc=");
      Serial.print(client.state());
      Serial.println(" mencoba lagi dalam 5 detik");
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  //Serial1.print("times.txt=\""+String(payload)+"\""+endChar);
  if ((char)payload[0] == '1') {
    //Serial1.print("CloudStatus.picc=1"+endChar);
  } else {
    //Serial1.print("CloudStatus.picc=0"+endChar);
  }
}