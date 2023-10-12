//
// Created by Aleksey Timin on 11/16/19.
//

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include "ConnectionManager.h"
#include "MessageRouter.h"
#include "SessionInfo.h"
#include "utils/Buffer.h"
#include "utils/Logger.h"
#include <cip/connectionManager/NetworkConnectionParams.h>
#include <cstdlib>
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

  auto si = std::make_shared<SessionInfo>("172.16.255.252", 0xAF12,
                                          std::chrono::seconds(10));
  auto messageRouter = std::make_shared<MessageRouter>();

  bool keepReading = true;
  int count(1);
  auto register_1 = EPath(0x7B, 0x0001, 1);
  while (keepReading) {
    //   // Read attribute
    //   auto response = messageRouter->sendRequest(
    //       si, ServiceCodes::GET_ATTRIBUTE_SINGLE, register_1, {});

    //   if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
    //     Buffer buffer(response.getData());
    //     CipUint vendorId;
    //     buffer >> vendorId;

    //     Logger(LogLevel::INFO) << "R[1] is " << vendorId;
    //   } else {
    //     Logger(LogLevel::ERROR)
    //         << "We got error=0x" << std::hex <<
    //         response.getGeneralStatusCode();
    //     keepReading = false;
    //   }

    // Write attribute
    Buffer register_1_val;
    register_1_val << CipInt(0) << CipInt(0) << CipReal(float(count++) / 1000.0)
                   << CipReal(0) << CipReal(0) << CipReal(0) << CipReal(0)
                   << CipReal(0) << CipSint(0) << CipSint(0) << CipSint(0)
                   << CipSint(0x70) << CipReal(0) << CipReal(0) << CipReal(0);

    auto response =
        messageRouter->sendRequest(si, ServiceCodes::SET_ATTRIBUTE_SINGLE,
                                   register_1, register_1_val.data());

    if (!response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
      Logger(LogLevel::ERROR)
          << "We got error=0x" << std::hex << response.getGeneralStatusCode();
      keepReading = false;
    }
  }
#if OS_Windows
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
