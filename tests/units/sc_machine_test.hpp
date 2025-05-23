/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/test/sc_test.hpp>

#include <filesystem>

class ScMachineTest : public testing::Test
{
public:
  static inline std::string const & SC_MACHINE_INI = "../Komma-test.ini";
  static inline std::string const & SC_MACHINE_KB_BIN = "../Komma-test-kb-bin";
  static inline std::string SC_MACHINE_EXTENSIONS;

protected:
  void SetUp() override {}

  void TearDown() override
  {
    std::filesystem::remove_all(SC_MACHINE_KB_BIN);
  }
};
