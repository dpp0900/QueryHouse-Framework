find_path(ORACLE_INCLUDE_DIR NAMES oci.h
  PATHS /opt/oracle/instantclient_19_8/sdk/include
)

find_library(ORACLE_LIBRARY NAMES clntsh
  PATHS /opt/oracle/instantclient_19_8
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Oracle DEFAULT_MSG ORACLE_LIBRARY ORACLE_INCLUDE_DIR)

if(ORACLE_FOUND)
  set(ORACLE_LIBRARIES ${ORACLE_LIBRARY})
  set(ORACLE_INCLUDE_DIRS ${ORACLE_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Oracle library not found.")
endif()
