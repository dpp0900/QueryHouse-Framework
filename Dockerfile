# Base image 설정
FROM ubuntu:22.04

# 새로운 사용자와 비밀번호 설정
ARG USERNAME=user
ARG USER_PASSWORD=password

# 필수 패키지 설치
RUN apt-get update && \
    apt-get -y install sudo

# 새로운 사용자 생성 및 sudo 권한 부여
RUN useradd -m -s /bin/bash $USERNAME && \
    echo "$USERNAME:$USER_PASSWORD" | chpasswd && \
    usermod -aG sudo $USERNAME

# 환경 변수 설정
ENV BASE_DIR=/home/$USERNAME/QueryHouse-Framework

# 나머지 필수 패키지 설치
RUN apt-get -y install make cmake build-essential vim git \
    clang libmysqlclient-dev ninja-build pkg-config clang-format \
    libpq-dev libyaml-cpp-dev lld llvm bison python3-fire apt-transport-https \
    curl software-properties-common gnutls-dev ltrace

# QueryHouse Framework 디렉토리 생성
RUN mkdir -p $BASE_DIR/bld

# 현재 디렉토리의 모든 파일을 컨테이너의 BASE_DIR로 복사
COPY . $BASE_DIR/

# 새로운 사용자로 작업 디렉토리 변경
USER $USERNAME

# MySQL 소스 빌드
WORKDIR $BASE_DIR/bld
RUN cmake $BASE_DIR/mysql/ -DDOWNLOAD_BOOST=1 -DWITH_BOOST=../boost && \
    make -j20 && \
    sudo cmake --install . --prefix /usr/local/mysql/

# mysql 디렉토리 권한 설정
RUN sudo mkdir -p /usr/local/mysql/mysql-files && \
    sudo chown -R $USERNAME:$USERNAME /usr/local/mysql/ && \
    sudo chmod 750 /usr/local/mysql/mysql-files

# MySQL 초기화
RUN sudo /usr/local/mysql/bin/mysqld --initialize-insecure --user=$USERNAME

# QueryHouse Framework 소스 빌드
WORKDIR $BASE_DIR
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -Wno-dev -DMYSQL=ON && \
    cmake --build build -j

# 빌드 시간 측정을 위한 스크립트 실행
CMD START_TIME=$(date +%s) && \
    MYSQL_END_TIME=$(date +%s) && \
    DRIVER_END_TIME=$(date +%s) && \
    echo "MYSQL BUILD TIME: $((MYSQL_END_TIME - START_TIME))s" && \
    echo "DRIVER BUILD TIME: $((DRIVER_END_TIME - MYSQL_END_TIME))s" && \
    echo "TOTAL BUILD TIME: $((DRIVER_END_TIME - START_TIME))s"
