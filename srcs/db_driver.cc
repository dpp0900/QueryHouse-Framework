#include <iostream>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "absl/strings/str_format.h"
#include "client.h"
#include "config.h"
#include "types.h"
#include "yaml-cpp/yaml.h"

using namespace std;

constexpr char kConfigEnv[] = "SQUIRREL_CONFIG";

int next_testcase(u8 *buf, size_t max_size) {
  ssize_t len = read(STDIN_FILENO, buf, max_size);
  if (len < 0) {
    perror("read");
    exit(-1);
  }
  return len;
}

int main(int argc, char *argv[]) {
  const char *config_file_path = getenv(kConfigEnv);
  if (!config_file_path) {
    cerr << absl::StrFormat(
        "You should set the environment variable %s to "
        "the path of your config file.\n",
        kConfigEnv);
    exit(-1);
  }
  YAML::Node config = YAML::LoadFile(config_file_path);
  string db_name = config["db"].as<string>();
  string startup_cmd = config["startup_cmd"].as<string>();
  client::DBClient *database = client::create_client(db_name, config);
  database->initialize(config);

  constexpr size_t kMaxInputSize = 0x100000;
  u8 *buf = (u8 *)malloc(kMaxInputSize);
  s32 len;

  if (!database->check_alive()) {
    system(startup_cmd.c_str());
    sleep(5);
  }

  while ((len = next_testcase(buf, kMaxInputSize)) > 0) {
    cerr << "Get the next testcase" << endl;
    string query((const char *)buf, len);
    database->prepare_env();

    client::ExecutionStatus status = database->execute((const char *)buf, len);

    if (status == client::kServerCrash) {
      while (!database->check_alive()) {
        sleep(5);
      }
    }
    database->clean_up_env();
  }
  assert(false && "Crash on parent?");

  return 0;
}
