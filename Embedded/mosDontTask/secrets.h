// const char* ssid = "Nha 92B";
// const char* password = "888888888";
// #define MQTT_MAX_PACKET_SIZE 512
// const char* ssid = "Xuong Rang Coffee";
// const char* password = "";
// const char* ssid = "E1-303-M";
// const char* password = "bebuoi216";

// const char* ntpServer = "asia.pool.ntp.org";
// const long  gmtOffset_sec = 25200;
// const int   daylightOffset_sec = 0;

const char* mqttServer = "a3rzyndk1wefwy-ats.iot.ap-southeast-2.amazonaws.com";
const int mqttPort = 8883;

const char* root = \
                   "-----BEGIN CERTIFICATE-----\n" \
                   "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
                   "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
                   "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
                   "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
                   "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
                   "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
                   "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
                   "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
                   "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
                   "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
                   "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
                   "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
                   "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
                   "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
                   "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
                   "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
                   "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
                   "rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
                   "-----END CERTIFICATE-----\n";
const char* cert = \
                    "-----BEGIN CERTIFICATE-----\n" \
                    "MIIDWjCCAkKgAwIBAgIVAJfPEjO26/mnxvoUtIQN3c1B+3f0MA0GCSqGSIb3DQEB\n" \
                    "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
                    "IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDAyMjUwNzQx\n" \
                    "MDdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
                    "dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC3VmN/otydQIUFbejJ\n" \
                    "ufWwBebXB2AfwS9Q2PLGNN/rLdNed1lX6AieQqDH/qBCrWhU14DbzeY1bb9PCczL\n" \
                    "ZSYiQbINc0v34NEqzi4270cYGTYb4icT24FNmO8/zlLLQyhIvUJ5wGaDYUyMHb6H\n" \
                    "mCXqt7peO+SBbEXmf4z4kRl0OuddkKb0Bp76PEj3JA/OpUS7OihrfUxkOJc++QYC\n" \
                    "HldvsZ+TC9Kz0GEYh39PVj+9OnkRvZOMDo4UzAJaMkeqrqnfbajpcsTDLXskhZqG\n" \
                    "+8cIhFxjxZVBsmMUxshqVby+VhkjVLG+xhjjtS7GCR7bg6/MurotrWwBU91uhkXk\n" \
                    "FvzdAgMBAAGjYDBeMB8GA1UdIwQYMBaAFG3YeEy2I4q2lc9FvL4zxoO63ZJzMB0G\n" \
                    "A1UdDgQWBBSl05HMubL7wP2e+KtaZRncPdTaXzAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
                    "DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAXfT+w6bV2aLN+CrHKe2WcTVR\n" \
                    "7xJ//DdSviM8CGrcE90F6wjDMiWS2On6NB/PcDfun47QN99Tj1sxBqwYbb4Cu05N\n" \
                    "amQxLq/tYYLCq2dtn0oM8PAmW5FDft6sD9D1XkiCFKFefm0vld9J+RQynl5/3VVB\n" \
                    "kRSc44zxsU/SOUPKGktY7CtHWhtRYrBJ86TIAryMC14sX6JRpnKQUsNJnWFmOwVx\n" \
                    "OBpo18hLd0BRh6NZV5decrQKGZ6+SWLKFVVLb7TXIdMz7dq8dEgjLDfbRqk+DFLg\n" \
                    "/t+3YJoR3EGiRnorxgmME8PYFSjAi+TB20GbIZAHeE8qKlBEgFjBWH7fwuZe8A==\n" \
                    "-----END CERTIFICATE-----\n";
const char* privateKey = \
                    "-----BEGIN RSA PRIVATE KEY-----\n" \
                    "MIIEpAIBAAKCAQEAt1Zjf6LcnUCFBW3oybn1sAXm1wdgH8EvUNjyxjTf6y3TXndZ\n" \
                    "V+gInkKgx/6gQq1oVNeA283mNW2/TwnMy2UmIkGyDXNL9+DRKs4uNu9HGBk2G+In\n" \
                    "E9uBTZjvP85Sy0MoSL1CecBmg2FMjB2+h5gl6re6XjvkgWxF5n+M+JEZdDrnXZCm\n" \
                    "9Aae+jxI9yQPzqVEuzooa31MZDiXPvkGAh5Xb7GfkwvSs9BhGId/T1Y/vTp5Eb2T\n" \
                    "jA6OFMwCWjJHqq6p322o6XLEwy17JIWahvvHCIRcY8WVQbJjFMbIalW8vlYZI1Sx\n" \
                    "vsYY47Uuxgke24OvzLq6La1sAVPdboZF5Bb83QIDAQABAoIBAQCNOirjQ12WdQED\n" \
                    "MatE2c78jU9JDRFq5LHCIZzCOjDTRTWxa1aTdNZOsxyPcwGY22OnF2rX50ASoDvi\n" \
                    "crYjaAN8GAdEfTBj/faidrj68E9frOwVhpx7m3d7gvepuxHkKhN6FPCEQUW2ptkE\n" \
                    "hlUitn1e1bDmrlRTrkgaSt8ZCw0Ejp2ZeKuxt6VY0KJjj0zZcQhGwupyt1me0EvZ\n" \
                    "1JPR/xhyADwE/TTJ44otQnkCNpG5DelVDTIXwixrhcl072Nc5NWRLzTc6EN/pd9g\n" \
                    "TnCcb+LlHkFq7IQO5D/G6LUSWiuigdevCgsRR6KgiYTpCAMXANvI39otBiumCUd7\n" \
                    "XQgF662xAoGBAN1tNzPzRZSy7GXJffVmKOF9iWv/NqS6+Es4IAGlAS6v1VdqQ8vK\n" \
                    "qC/D6g3NZXS7dSEQJfK4xSpdWEmpzkN5dIa2mtziHoz1Tv4RSul5fcwS7UqL5JXn\n" \
                    "HeY1v7ME2gO+ETZQsEwq5nDvOw1caq4HjQq8pm0DMlq4kCR8oTrtsNmLAoGBANP2\n" \
                    "r8p0NkOePN2J2YpZkV9eKyl62c+mR6/fVT79n7ujW8ZRQuV3tXfP6oKAdkexRFPf\n" \
                    "pRnVMSZPNAFpk3+346fYSxv3R2WLVO4GE4Tr0ZfJlssMHqo58Suo4105wW1w5Y/g\n" \
                    "5JBHNUDpxD1Zu2zrERnOhj1R6Q/2+eRzOH4t0MA3AoGBANniw6RqKTj2i6ppiHm3\n" \
                    "CllA973+x913YiZaVxxnKu+u9IbtOUNK6w79gM4OJFymrc5YHo9mhBV4/wyVfDNn\n" \
                    "S2PYQjFCjH92UEGv8IAoHcJ3iWqGl+tuHjzO/RECoCl4mWVSJvj8agANWJWAMDWJ\n" \
                    "lp9PHZE9yQn+fVB6FwlupUybAoGAVRphoE6ijumUlHpJbqPhm0HK4J+jnhysAbAQ\n" \
                    "oPbh0EuZiHmN+2Xh5VYEWRtM4iIJRgewWTvgvPnl7n8+KnRhoj1Ugsj40IttsRed\n" \
                    "dKvd/iyBL8sQ8NXMv/PSPdb3k4JVmTv5aPg+KG7zTJKfju6echcajLo7ch5+XLvq\n" \
                    "PbcVbmcCgYAmflHUEyBLv+pLGnZkmC5fX3NonC4jDyxQOuWjWuYfDDfc0joWV2cN\n" \
                    "p5+PVZQqlDemSifkeXYUseBVqRKc44QvbtrrgToXMBcnMgDAEAZFDDd/7/ndgBCn\n" \
                    "+UbxUE7UaJ0ozMXI1obR/JCTbllA4mfzC4OUKLX5FFmNrn7DF/m8HA==\n" \
                    "-----END RSA PRIVATE KEY-----\n";
