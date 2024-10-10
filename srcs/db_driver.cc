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

u8 *__afl_area_ptr;

__thread u32 __afl_map_size = MAP_SIZE;

/* Error reporting to forkserver controller */

void send_forkserver_error(int error) {
  u32 status;
  if (!error || error > 0xffff) return;
  status = (FS_OPT_ERROR | FS_OPT_SET_ERROR(error));
  if (write(FORKSRV_FD + 1, (char *)&status, 4) != 4) return;
}

/* SHM setup. */

static void __afl_map_shm(void) {
  char *id_str = getenv(SHM_ENV_VAR);
  char *ptr;

  /* NOTE TODO BUG FIXME: if you want to supply a variable sized map then
     uncomment the following: */

  if ((ptr = getenv("AFL_MAP_SIZE")) != NULL) {
    u32 val = atoi(ptr);
    if (val > 0) __afl_map_size = val;
  }

  if (__afl_map_size > MAP_SIZE) {
    if (__afl_map_size > FS_OPT_MAX_MAPSIZE) {
      fprintf(stderr,
              "Error: AFL++ tools *require* to set AFL_MAP_SIZE to %u to "
              "be able to run this instrumented program!\n",
              __afl_map_size);
      if (id_str) {
        send_forkserver_error(FS_ERROR_MAP_SIZE);
        exit(-1);
      }

    } else {
      fprintf(stderr,
              "Warning: AFL++ tools will need to set AFL_MAP_SIZE to %u to "
              "be able to run this instrumented program!\n",
              __afl_map_size);
    }
  }

  if (id_str) {
#ifdef USEMMAP
    const char *shm_file_path = id_str;
    int shm_fd = -1;
    unsigned char *shm_base = NULL;

    /* create the shared memory segment as if it was a file */
    shm_fd = shm_open(shm_file_path, O_RDWR, 0600);
    if (shm_fd == -1) {
      fprintf(stderr, "shm_open() failed\n");
      send_forkserver_error(FS_ERROR_SHM_OPEN);
      exit(1);
    }

    /* map the shared memory segment to the address space of the process */
    shm_base =
        mmap(0, __afl_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    if (shm_base == MAP_FAILED) {
      close(shm_fd);
      shm_fd = -1;

      fprintf(stderr, "mmap() failed\n");
      send_forkserver_error(FS_ERROR_MMAP);
      exit(2);
    }

    __afl_area_ptr = shm_base;
#else
    u32 shm_id = atoi(id_str);

    __afl_area_ptr = (u8 *)shmat(shm_id, 0, 0);

#endif

    if (__afl_area_ptr == (void *)-1) {
      send_forkserver_error(FS_ERROR_SHMAT);
      exit(1);
    }

    /* Write something into the bitmap so that the parent doesn't give up */
    __afl_area_ptr[0] = 1;
  }
}

/* Fork server logic. */

static void __afl_start_forkserver(void) {
  u8 tmp[4] = {0, 0, 0, 0};
  u32 status = 0;

  if (__afl_map_size <= FS_OPT_MAX_MAPSIZE)
    status |= (FS_OPT_SET_MAPSIZE(__afl_map_size) | FS_OPT_MAPSIZE);
  if (status) status |= (FS_OPT_ENABLED);
  memcpy(tmp, &status, 4);

  /* Phone home and tell the parent that we're OK. */

  if (write(FORKSRV_FD + 1, tmp, 4) != 4) return;
}

static u32 __afl_next_testcase(u8 *buf, u32 max_len) {
  s32 status, res = 0xffffff;

  /* Wait for parent by reading from the pipe. Abort if read fails. */
  if (read(FORKSRV_FD, &status, 4) != 4) return 0;

  /* we have a testcase - read it */
  status = read(0, buf, max_len);

  /* report that we are starting the target */
  if (write(FORKSRV_FD + 1, &res, 4) != 4) return 0;

  return status;
}

static void __afl_end_testcase(client::ExecutionStatus status) {
  int waitpid_status = 0xffffff;
  if (status == client::kServerCrash) {
    waitpid_status = 0x6;  // raise.
  }

  if (write(FORKSRV_FD + 1, &waitpid_status, 4) != 4) exit(1);
}

int next_testcase(u8 *buf, size_t max_size) {
  ssize_t len = read(STDIN_FILENO, buf, max_size);
  if (len < 0) {
    perror("read");
    exit(-1);
  }
  return len;
}

bool isFloat(string myString) {
  std::istringstream iss(myString);
  float f;
  iss >> noskipws >> f; // noskipws considers leading whitespace invalid
  // Check the entire string was consumed and if either failbit or badbit is set
  return iss.eof() && !iss.fail();
}

bool comapre_result(const vector<vector<string>> &result1, const vector<vector<string>> &result2) {
  if (result1.size() != result2.size()) {
    return false;
  }
  for (size_t i = 0; i < result1.size(); i++) {
    if (result1[i].size() != result2[i].size()) {
      return false;
    }
    for (size_t j = 0; j < result1[i].size(); j++) {
      if (isFloat(result1[i][j]) && isFloat(result2[i][j])) {
        if (stof(result1[i][j]) != stof(result2[i][j])) {
          return false;
        }
      } else {
        
        if (result1[i][j] != result2[i][j]) {
          return false;
        }
      }
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  //set basedir as /home/$user/QueryHouse-Framwork
  string basedir = getenv("HOME");
  basedir += "/QueryHouse";
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
    cout << "DB Name: " << db_name << endl;
    cout << "Startup Command: " << startup_cmd << endl;
    db_clients.emplace_back(client::create_client(db_name, config));
  }

  constexpr size_t kMaxInputSize = 0x100000;
  u8 *buf = (u8 *)malloc(kMaxInputSize);
  s32 len = 1;

  __afl_map_size = MAP_SIZE;  // default is 65536

  /* then we initialize the shared memory map and start the forkserver */

  // Start the database server. In case that the driver
  // is stopped and restarted, we should not start another server.
  __afl_map_shm();


  for (auto &db_client : db_clients) {
    if (!db_client->check_alive()) {
      cout << "DB Client is not alive." << endl;
      string startup_cmd = startup_cmds[&db_client - &db_clients[0]];
      cout << "Start server: " << startup_cmd << endl;
      system(startup_cmd.c_str());
      sleep(5);
    }
  }
  __afl_start_forkserver();
  while ((len = __afl_next_testcase(buf, kMaxInputSize)) > 0) {
    FILE *fp = fopen("/tmp/db_driver_log", "a");
    fwrite(buf, 1, len, fp);
    fwrite("\n", 1, 1, fp);
    fclose(fp);
    vector<vector<vector<string>>> compare_queue;
    for (auto &db_client : db_clients) {
      cout << "DB Client: " << db_names[&db_client - &db_clients[0]] << endl;
      string query((const char *)buf, len);
      vector<vector<string>> result;
      db_client->prepare_env();
      client::ExecutionStatus status = db_client->execute((const char *)buf, len, result);
      for (const auto &row : result) {
        for (const auto &col : row) {
          cout << col << " ";
        }
        cout << endl;
      }
      compare_queue.push_back(result);
      if (status == client::kServerCrash) {
        while (!db_client->check_alive()) {
          sleep(5);
        }
      }
      db_client->clean_up_env();
    }
    for (size_t i = 0; i < compare_queue.size(); i++) {
      for (size_t j = i + 1; j < compare_queue.size(); j++) {
        if (!comapre_result(compare_queue[i], compare_queue[j])) {
          cout << "Result of " << db_names[i] << " and " << db_names[j] << " is different." << endl;
        }
        else {
          cout << "Result of " << db_names[i] << " and " << db_names[j] << " is same." << endl;
        }
      }
    }



    // remove this, this is just to make afl-fuzz not complain when run
    __afl_area_ptr[1] = 1;

    /* report the test case is done and wait for the next */
    __afl_end_testcase(client::kNormal);
  }


  return 0;
}