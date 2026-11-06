
# DroidMonitor Protocol v1

Transport: TCP over IPv4
Port: 24816
Byte order: Big Endian

## Header (12 bytes)

| Offset | Size | Field |
|---|---:|---|
| 0 | 4 | Magic: DMON |
| 4 | 2 | Version: 1 |
| 6 | 2 | Message Type |
| 8 | 4 | Payload Length |

## Message Types

1. HELLO
2. HELLO_ACK
3. PING
4. PONG
5. DISCONNECT
10. VIDEO_CONFIG
11. VIDEO_FRAME
12. VIDEO_ACK

## Handshake

Windows sends:
Type: HELLO
Payload: DROIDMONITOR_HOST

Android responds:
Type: HELLO_ACK
Payload: DROIDMONITOR_ANDROID

## Limits

Maximum payload: 1 MiB for control messages.
Video frames will use a separately negotiated limit.

## Rules

- TCP stream framing is mandatory.
- A receiver must read the complete header.
- A receiver must read exactly Payload Length bytes.
- Unknown message types must be rejected.
- Invalid magic or protocol version must close the connection.
- All payloads in v1 control messages use UTF-8.
