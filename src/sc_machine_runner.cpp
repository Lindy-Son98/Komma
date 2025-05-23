/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "Komma-runner/sc_machine_runner.hpp"

#include <iostream>
#include <atomic>
#include <thread>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_debug.hpp>
#include <sc-memory/utils/sc_signal_handler.hpp>

#include <sc-config/sc_options.hpp>
#include <sc-config/sc_config.hpp>
#include <sc-config/sc_memory_config.hpp>

void PrintHelpMessage(std::string const & binaryName)
{
  std::cout
      << "Usage:\n"
      << "  " << binaryName << " [options]\n\n"
      << "Required options:\n"
      << "  --storage|-s <directory>                Provide a path to directory with knowledge base "
         "binaries. Knowledge base binaries should be generated by sc-builder binary.\n"
         "                                          This path can also be provided via the `storage` option in the "
         "[sc-memory] group of the configuration file specified with --config|-c.\n"
         "                                          If both options are provided, the value from --storage|-s takes "
         "precedence.\n"
      << "  --config|-c <config-name>.ini           Provide a path to configuration file. If `storage` "
         "option is not provided as argument, then this file should contain `storage` option in "
         "`[sc-memory]` group.\n"
         "                                          Configuration file can be used to set additional (optional) "
         "options for "
      << binaryName << ".\n\n"
      << "Options:\n"
      << "  --extensions|-e <directory>             Provide a path to directories containing "
         "extensions. Extensions should represent compiled dynamic libraries of agent sc-modules.\n"
         "                                          This path can also be provided via the `extensions` option in the "
         "[sc-memory] group of the configuration file specified with --config|-c.\n"
         "                                          If both options are provided, the value from --extensions|-e takes "
         "precedence.\n"
      << "  --clear                                 Run sc-memory in the mode when it overwrites "
         "existing knowledge base binaries.\n"
      << "  --verbose|-v                            Shutdown sc-memory without dumping its state into knowledge base "
         "binaries.\n"
      << "  --test|-t                               Test sc-memory state. "
      << "If this flag is specified, sc-memory will be initialized and shutdown immediately.\n"
      << "  --version                               Display version of " << binaryName << ".\n"
      << "  --help                                  Display this help message.\n\n"
      << "Environment variables:\n"
      << "  SC_SERVER_HOST                          Specifies a host for sc-server extension. If not set, the value "
         "defaults to the `host` option in the `[sc-server]` group of the configuration file.\n"
         "                                          If neither is specified, the default host is 127.0.0.1.\n"
      << "  SC_SERVER_PORT                          Specifies a port for sc-server extension. If not set, the value "
         "defaults to the `port` option in the `[sc-server]` group of the configuration file.\n"
         "                                          If neither is specified, the default port is 8090.\n";
}

sc_int RunMachine(sc_int argc, sc_char * argv[])
try
{
  std::string binaryName{argv[0]};

  ScOptions options{argc, argv};
  if (options.Has({"help"}))
  {
    PrintHelpMessage(binaryName);
    return EXIT_SUCCESS;
  }

  if (options.Has({"version"}))
  {
    std::cout << ScMemoryConfig::GetVersion() << std::endl;
    return EXIT_SUCCESS;
  }

  std::string configPath;
  if (options.Has({"config", "c"}))
    configPath = options[{"config", "c"}].second;

  sc_bool saveOnShutdown = !options.Has({"verbose", "v"});

  ScMemory::ms_configPath = configPath;

  ScConfig config{configPath, {"extensions", "repo_path", "storage", "log_file"}};
  ScParams memoryParams{options, {{"extensions", "e"}, {"storage", "s"}, {"clear"}}};
  ScMemoryConfig memoryConfig{config, memoryParams};

  if (!memoryConfig.HasKey("storage") && !memoryConfig.HasKey("repo_path"))
  {
    std::cout << "Error: Knowledge base binaries directory is required, but it is missing. Use --storage|-s to specify "
                 "the path or "
              << "in the configuration file (<config>.ini) under the `storage` option in the [sc-memory] group.\n";
    std::cout << "For more information, run with --help.\n";
    return EXIT_FAILURE;
  }

  std::atomic_bool isRun;
  if (!ScMemory::Initialize(memoryConfig.GetParams()))
    goto error;

  utils::ScSignalHandler::Initialize();

  isRun = !options.Has({"test", "t"});
  // LCOV_EXCL_START
  utils::ScSignalHandler::m_onTerminate = [&isRun]()
  {
    isRun = SC_FALSE;
  };

  while (isRun)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  // LCOV_EXCL_STOP

error:
  return ScMemory::Shutdown(saveOnShutdown) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// LCOV_EXCL_START
catch (utils::ScException const & e)
{
  std::cout << e.Message() << std::endl;
  return EXIT_FAILURE;
}

// LCOV_EXCL_STOP
