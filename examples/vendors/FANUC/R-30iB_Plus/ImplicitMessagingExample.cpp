//
// Created by Aleksey Timin on 11/16/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include "ConnectionManager.h"
#include "SessionInfo.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"
#include <cip/connectionManager/NetworkConnectionParams.h>
#include <sstream>

using namespace eipScanner::cip;
using eipScanner::ConnectionManager;
using eipScanner::MessageRouter;
using eipScanner::SessionInfo;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int main() {
  Logger::setLogLevel(LogLevel::DEBUG);

#if OS_Windows
  WSADATA wsaData;
  int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (winsockStart != 0) {
    Logger(LogLevel::ERROR)
        << "Failed to start WinSock - error code: " << winsockStart;
    return EXIT_FAILURE;
  }
#endif

  constexpr unsigned FANUC_SLOT_SIZE = 16;
  auto si = std::make_shared<SessionInfo>("172.16.255.254", 0xAF12);

  // Implicit messaging
  ConnectionManager connectionManager;

  ConnectionParameters parameters;
  parameters.connectionPath = {
      0x20, 0x04, 0x24, 100,
      0x2C, 151,  0x2C, 101}; // config Assm100, output Assm151, intput Assm101
  parameters.o2tRealTimeFormat = true;
  parameters.originatorVendorId = 356;
  parameters.originatorSerialNumber = 32423;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.t2oNetworkConnectionParams |=
      NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.t2oNetworkConnectionParams |= FANUC_SLOT_SIZE;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.o2tNetworkConnectionParams |=
      NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= FANUC_SLOT_SIZE;

  parameters.originatorSerialNumber = 0x12345;
  parameters.o2tRPI = 16000; // in uS
  parameters.t2oRPI = 16000;
  parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;

  eipScanner::IOConnection::WPtr io;
  try {
    io = connectionManager.forwardOpen(si, parameters);
    if (auto ptr = io.lock()) {
      ptr->setDataToSend(std::vector<uint8_t>(FANUC_SLOT_SIZE));

      ptr->setReceiveDataListener(
          [](auto realTimeHeader, auto sequence, auto data) {
            std::ostringstream ss;
            ss << "secNum=" << sequence << " data=";
            for (auto &byte : data) {
              ss << "[" << std::hex << (int)byte << "]";
            }

            Logger(LogLevel::INFO) << "Received: " << ss.str();
          });

      ptr->setCloseListener([]() { Logger(LogLevel::INFO) << "Closed"; });
    }

    int count = 200;
    while (connectionManager.hasOpenConnections() && count-- > 0) {
      connectionManager.handleConnections(std::chrono::milliseconds(100));
    }
  } catch (std::exception &e) {
    Logger(LogLevel::ERROR) << e.what();
  }
  connectionManager.forwardClose(si, io);

#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}