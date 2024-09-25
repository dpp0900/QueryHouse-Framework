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

#include <filesystem>       // 추가된 코드: 파일 시스템 관련 함수를 사용하기 위해 추가

#include <vector>            // 추가된 코드: 여러 DBClient를 관리하기 위해 추가
#include <memory>            // 추가된 코드: 스마트 포인터 사용을 위해 추가


#include "absl/strings/str_format.h"
#include "client.h"
#include "config.h"
#include "types.h"
#include "yaml-cpp/yaml.h"

using namespace std;


int next_testcase(u8 *buf, size_t max_size) {
  ssize_t len = read(STDIN_FILENO, buf, max_size);
  if (len < 0) {
    perror("read");
    exit(-1);
  }
  return len;
}

int main(int argc, char *argv[]) {

  //set basedir as /home/$user/QueryHouse-Framwork
  string basedir = getenv("HOME");
  basedir += "/QueryHouse-Framework";
  cout << "Basedir: " << basedir << endl;
  string config_file_path = basedir + "/data/config/";
  vector<string> config_files;
  
  for (const auto & entry : filesystem::directory_iterator(config_file_path)) {
    config_files.push_back(entry.path());
    cout << "Load config file: " << entry.path() << endl;
  }
  vector<unique_ptr<client::DBClient>> db_clients;
  vector<YAML::Node> configs;
  vector<string> db_names;
  vector<string> startup_cmds;
  for (const auto & config_file : config_files) {
    YAML::Node config = YAML::LoadFile(config_file);
    string db_name = config["db"].as<string>();
    string startup_cmd = config["startup_cmd"].as<string>();
    db_names.push_back(db_name);
    startup_cmds.push_back(startup_cmd);
    configs.push_back(config);
    db_clients.emplace_back(client::create_client(db_name, config));
  }

  constexpr size_t kMaxInputSize = 0x100000;
  u8 *buf = (u8 *)malloc(kMaxInputSize);
  s32 len = 1;

  for (auto &db_client : db_clients) {
    if (!db_client->check_alive()) {
      cout << "DB Client is not alive." << endl;
      string startup_cmd = startup_cmds[&db_client - &db_clients[0]];
      cout << "Start server: " << startup_cmd << endl;
      system(startup_cmd.c_str());
      sleep(5);
    }
  }
  while (len > 0){
    cout << "[checkpoints] Start to get the next testcase" << endl;
    for (auto &db_client : db_clients) {
      len = next_testcase(buf, kMaxInputSize);
      db_client->prepare_env();
      client::ExecutionStatus status = db_client->execute((const char *)buf, len);
      if (status == client::kServerCrash) {
        while (!db_client->check_alive()) {
          sleep(5);
        }
      }
      db_client->clean_up_env();
    }
  }

  

  // while ((len = next_testcase(buf, kMaxInputSize)) > 0) {
  //   cerr << "Get the next testcase" << endl;
  //   string query((const char *)buf, len);
  //   database->prepare_env();

  //   client::ExecutionStatus status = database->execute((const char *)buf, len);

  //   if (status == client::kServerCrash) {
  //     while (!database->check_alive()) {
  //       sleep(5);
  //     }
  //   }
  //   database->clean_up_env();
  // }
  // assert(false && "Crash on parent?");

  return 0;
}