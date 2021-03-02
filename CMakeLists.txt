AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/npc_enchanter.cpp")
AC_ADD_SCRIPT_LOADER("NPCEnchanter" "${CMAKE_CURRENT_LIST_DIR}/src/loader_enchanter.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/npc_enchanter.conf.dist")