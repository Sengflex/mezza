cmake_minimum_required(VERSION 3.18)

project(mezza)

SET( NAME mezza )
SET( APP_EXE ${NAME}_tests )
SET( TEST_DIR _tests )

SET( LIB_BASE_SRCS base/TMemMgr.c base/TList.c base/Exceptions.c base/TObject.c )
SET( LIB_COLL_SRCS collections/Map.c collections/TokenSet.c )
SET( LIB_DBASE_SRCS dbase/TDBField.c dbase/TDBRow.c dbase/TDBTable.c dbase/TSQLite3Conn.c )
SET( LIB_STR_SRCS str/StringBuilder.c str/StringUtils.c str/TString.c str/TStringVector.c )
SET( LIB_SRCS Conf.c FileUtil.c )

include_directories(../sqlite3)

ADD_LIBRARY( ${NAME} STATIC ${LIB_BASE_SRCS} ${LIB_COLL_SRCS} ${LIB_DBASE_SRCS} ${LIB_STR_SRCS} ${LIB_SRCS} )


ADD_EXECUTABLE( ${APP_EXE} ${TEST_DIR}/main.c ${TEST_DIR}/collections.c ${TEST_DIR}/conf.c ${TEST_DIR}/dbase.c  ${TEST_DIR}/exceptions.c ${TEST_DIR}/file.c ${TEST_DIR}/list.c ${TEST_DIR}/mem.c ${TEST_DIR}/objects.c ${TEST_DIR}/str.c )

TARGET_LINK_LIBRARIES( ${APP_EXE} lib${NAME}.a )