//
// Created by P.Glaubitz, READY Robotics on 2023-10-10
//

#include "EPath.h"

namespace eipScanner::vendor::FANUC::R_30iB_Plus {
/**
 * Collection of helper functions to create EPaths for accessing registers in
 * the R-30iB Plus controller.
 */
constexpr unsigned int MAX_NUM_REGISTERS = 200;
constexpr unsigned int MAX_NUM_REGISTERS_PER_READ_BLOCK = 124;
constexpr unsigned int MAX_NUM_REGISTERS_PER_WRITE_BLOCK = 115;

constexpr CipUsint SERVICE_BLOCK_READ = 0x32;
constexpr CipUsint SERVICE_BLOCK_WRITE = 0x33;

/**
 * Path for accessing a single register in the R-30iB Plus controller as an
 * integer. Suported services are GET_ATTRIBUTE_SINGLE and SET_ATTRIBUTE_SINGLE.
 */
cip::EPath intRegister(unsigned int index) {
  if (index < 1 || index > MAX_NUM_REGISTERS) {
    throw std::invalid_argument("Index must be between 1 and 200");
  }
  return cip::EPath(0x6B, 0x1, index);
}

/**
 * Path for accessing the first N registers in the R-30iB Plus controller as
 * integers. Suported services are GET_ATTRIBUTE_ALL (N=124) and
 * SET_ATTRIBUTE_ALL (N=115).
 */
cip::EPath intRegisters() { return cip::EPath(0x6B, 0x1, 0); }

/**
 * Path for accessing a block registers in the R-30iB Plus controller as
 * integers. Suported services are SERVICE_BLOCK_READ (N=124) and
 * SERVICE_BLOCK_WRITE (N=115).Block is limited to N registers.
 */
cip::EPath intRegisterBlock(unsigned firstIndex, unsigned lastIndex) {
  if (firstIndex < 1 || firstIndex > MAX_NUM_REGISTERS) {
    throw std::invalid_argument("First index must be between 1 and 200");
  }
  if (lastIndex < firstIndex || firstIndex > MAX_NUM_REGISTERS) {
    throw std::invalid_argument(
        "Last index must be between first index and 200");
  }
  unsigned int numRegisters = lastIndex - firstIndex + 1;
  if (numRegisters > MAX_NUM_REGISTERS_PER_READ_BLOCK) {
    throw std::invalid_argument(
        "Can't access a block of more than 124 registers");
  }
  return cip::EPath(0x6B, 0x10 * (numRegisters) + 0x1, firstIndex);
}

/**
 * Path for accessing a single register in the R-30iB Plus controller as a
 * float. Suported services are GET_ATTRIBUTE_SINGLE and SET_ATTRIBUTE_SINGLE.
 */
cip::EPath floatRegister(unsigned int index) {
  if (index < 1 || index > MAX_NUM_REGISTERS) {
    throw std::invalid_argument("Index must be between 1 and 200");
  }
  return cip::EPath(0x6C, 0x1, index);
}

/**
 * Path for accessing the first N registers in the R-30iB Plus controller as
 * floats. Suported services are GET_ATTRIBUTE_ALL (N=124) and SET_ATTRIBUTE_ALL
 * (N=115).
 */
cip::EPath floatRegisters() { return cip::EPath(0x6C, 0x1, 0); }

/**
 * Path for accessing a block of registers in the R-30iB Plus controller as
 * floats. Suported services are SERVICE_BLOCK_READ (N=124) and
 * SERVICE_BLOCK_WRITE (N=115).Block is limited to N registers.
 */
cip::EPath floatRegisterBlock(unsigned firstIndex, unsigned lastIndex) {
  if (firstIndex < 1 || firstIndex > MAX_NUM_REGISTERS) {
    throw std::invalid_argument("First index must be between 1 and 200");
  }
  if (lastIndex < firstIndex || firstIndex > MAX_NUM_REGISTERS) {
    throw std::invalid_argument(
        "Last index must be between first index and 200");
  }
  unsigned int numRegisters = lastIndex - firstIndex + 1;
  if (numRegisters > MAX_NUM_REGISTERS_PER_READ_BLOCK) {
    throw std::invalid_argument(
        "Can't access a block of more than 124 registers");
  }
  return cip::EPath(0x6C, 0x10 * (numRegisters) + 0x1, firstIndex);
}

constexpr unsigned int MAX_STR_REGISTERS = 25;
constexpr unsigned int MAX_STR_REGISTERS_PER_BLOCK = 5;

/**
 * Path for accessing a single string register in the R-30iB Plus controller.
 * Suported services are GET_ATTRIBUTE_SINGLE and SET_ATTRIBUTE_SINGLE.
 */
cip::EPath strRegister(unsigned int index) {
  if (index < 1 || index > MAX_NUM_REGISTERS) {
    throw std::invalid_argument("Index must be between 1 and 200");
  }
  return cip::EPath(0x6D, 0x1, index);
}

/**
 * Path for accessing the first 5 string registers in the R-30iB Plus
 * controller. Suported services are GET_ATTRIBUTE_ALL and SET_ATTRIBUTE_ALL.
 */
cip::EPath strRegisters() { return cip::EPath(0x6D, 0x1, 0); }

/**
 * Path for accessing a block of string registers in the R-30iB Plus controller.
 * Suported services are SERVICE_BLOCK_READ and SERVICE_BLOCK_WRITE. Block is
 * limited to 5 registers.
 */
cip::EPath intRegisterBlock(unsigned firstIndex, unsigned lastIndex) {
  if (firstIndex < 1 || firstIndex > MAX_STR_REGISTERS) {
    throw std::invalid_argument("First index must be between 1 and 25");
  }
  if (lastIndex < firstIndex || firstIndex > MAX_STR_REGISTERS) {
    throw std::invalid_argument(
        "Last index must be between first index and 25");
  }
  unsigned int numRegisters = lastIndex - firstIndex + 1;
  if (numRegisters > MAX_NUM_REGISTERS_PER_READ_BLOCK) {
    throw std::invalid_argument(
        "Can't access a block of more than 5 registers");
  }
  return cip::EPath(0x6D, 0x10 * (numRegisters) + 0x1, firstIndex);
}

constexpr unsigned int MAX_POSN_REGISTERS = 100;

} // namespace eipScanner::vendor::FANUC::R_30iB_Plus
