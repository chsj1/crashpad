// Copyright 2015 The Crashpad Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <windows.h>

#include "base/logging.h"
#include "client/crashpad_client.h"
#include "client/simulate_crash.h"
#include "util/file/file_io.h"
#include "util/win/address_types.h"

__declspec(noinline) crashpad::WinVMAddress CurrentAddress() {
  return reinterpret_cast<crashpad::WinVMAddress>(_ReturnAddress());
}

int main(int argc, char* argv[]) {
  CHECK_EQ(argc, 2);

  crashpad::CrashpadClient client;
  CHECK(client.SetHandler(argv[1]));
  CHECK(client.UseHandler());

  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  PCHECK(out != INVALID_HANDLE_VALUE) << "GetStdHandle";
  crashpad::WinVMAddress current_address = CurrentAddress();
  crashpad::CheckedWriteFile(out, &current_address, sizeof(current_address));

  CRASHPAD_SIMULATE_CRASH();

  return 0;
}
