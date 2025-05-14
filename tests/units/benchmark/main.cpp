/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "benchmark/benchmark.h"

#include "units/sc_server_complex.hpp"
#include "units/sc_server_generate_connector.hpp"
#include "units/sc_server_generate_node.hpp"
#include "units/sc_server_generate_link.hpp"
#include "units/sc_server_erase_elements.hpp"
#include "units/sc_server_search_template.hpp"

#include <atomic>

template <class BMType>
void BM_ServerThreaded(benchmark::State & state)
{
  static std::atomic_int clientsNum = {0};
  static std::atomic_int printerLock = {0};
  BMType test;
  if (state.thread_index() == 0)
    test.Initialize();

  while (test.m_server == nullptr)
    ;

  auto start = std::chrono::high_resolution_clock::now();

  std::unique_ptr<ScClient> client = std::make_unique<ScClient>();
  client->Connect(test.m_server->GetUri());
  client->Run();

  clientsNum.fetch_add(1);

  uint32_t iterations = 0;
  for (auto _ : state)
  {
    test.Run(client);
    ++iterations;
  }

  client->Stop();
  client = nullptr;
  SC_UNUSED(client);

  test.WaitServer();

  state.counters["rate"] = benchmark::Counter(iterations, benchmark::Counter::kIsRate);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;

  while (printerLock.load() != 0)
    ;

  printerLock.fetch_add(1);
  std::cout << "Elapsed " << elapsed.count() << " ms for thread " << state.thread_index() << "\n";
  printerLock.fetch_add(-1);

  if (state.thread_index() == 0)
  {
    clientsNum.fetch_add(-1);

    while (clientsNum.load() != 0)
      ;
    test.Shutdown();
  }
  else
  {
    clientsNum.fetch_add(-1);
  }
}

sc_int constexpr kNodeIters = 100;

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(1)
    ->Iterations(kNodeIters)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(2)
    ->Iterations(kNodeIters / 2)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(4)
    ->Iterations(kNodeIters / 4)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(8)
    ->Iterations(kNodeIters / 8)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(16)
    ->Iterations(kNodeIters / 16)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateNode)
    ->Threads(32)
    ->Iterations(kNodeIters / 32)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

sc_int constexpr kLinkIters = 100;

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(1)
    ->Iterations(kLinkIters)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(2)
    ->Iterations(kLinkIters / 2)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(4)
    ->Iterations(kLinkIters / 4)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(8)
    ->Iterations(kLinkIters / 8)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(16)
    ->Iterations(kLinkIters / 16)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

BENCHMARK_TEMPLATE(BM_ServerThreaded, TestGenerateLink)
    ->Threads(32)
    ->Iterations(kLinkIters / 32)
    ->Unit(benchmark::TimeUnit::kMicrosecond);

// ------------------------------------
template <class BMType>
void BM_ServerRanged(benchmark::State & state)
{
  BMType test;
  test.Initialize(50);
  uint32_t iterations = 0;

  auto start = std::chrono::high_resolution_clock::now();

  std::unique_ptr<ScClient> client = std::make_unique<ScClient>();
  client->Connect(test.m_server->GetUri());
  client->Run();

  for (auto _ : state)
  {
    test.Run(client);
    ++iterations;
  }

  client->Stop();
  client = nullptr;
  SC_UNUSED(client);

  test.WaitServer();

  state.counters["rate"] = benchmark::Counter(iterations, benchmark::Counter::kIsRate);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;

  std::cout << "Elapsed " << elapsed.count() << " ms\n";

  test.Shutdown();
}

BENCHMARK_TEMPLATE(BM_ServerRanged, TestScServerComplex)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_TEMPLATE(BM_ServerRanged, TestGenerateNode)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_TEMPLATE(BM_ServerRanged, TestGenerateConnector)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_TEMPLATE(BM_ServerRanged, TestGenerateLink)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_TEMPLATE(BM_ServerRanged, TestEraseElements)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_TEMPLATE(BM_ServerRanged, TestSearchTemplate)->Unit(benchmark::TimeUnit::kMicrosecond)->Iterations(1000);

BENCHMARK_MAIN();
