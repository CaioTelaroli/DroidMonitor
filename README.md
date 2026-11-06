
# DroidMonitor

Transforme seu dispositivo Android em um monitor adicional
para Windows através de conexão USB.

## Plataformas

- Windows 10 / Windows 11
- Android 12 ou superior

## Tecnologias

- C++20
- Kotlin
- CMake
- Gradle
- TCP/IPv4
- USB Tethering

## Estrutura

- windows/: servidor Windows
- android/: cliente Android
- shared/: protocolo compartilhado

## Compilar Windows

Requisitos:

- Visual Studio 2022
- MSVC
- CMake 3.24+
- Windows SDK

Na raiz:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

Executar:

```powershell
.\build\windows\Release\DroidMonitorHost.exe 192.168.42.129
```

## Compilar Android

Requisitos:

- Android Studio
- JDK 17
- Android SDK 35

Abra a pasta android/ no Android Studio.

Compile e execute o aplicativo em um dispositivo Android 12+.

## Conexão

1. Conecte o celular ao computador por USB.
2. Ative USB Tethering no Android.
3. Abra DroidMonitor Client.
4. Inicie o servidor.
5. Consulte o endereço IPv4 do Android.
6. Execute DroidMonitorHost.exe informando esse endereço.

Porta TCP: 24816.

## Estado do projeto

v0.1 - Comunicação TCP e handshake.

Próximos módulos:

- H.264
- MediaCodec
- Media Foundation
- IddCx
- Monitor virtual
