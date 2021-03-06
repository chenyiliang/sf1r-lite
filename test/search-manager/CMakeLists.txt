INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/core
  ${CMAKE_SOURCE_DIR}/process
  ${CMAKE_SOURCE_DIR}
  ${izenelib_INCLUDE_DIRS}
  ${ilplib_INCLUDE_DIRS}
  ${idmlib_INCLUDE_DIRS}
  ${imllib_INCLUDE_DIRS}
  ${izenecma_INCLUDE_DIRS}
  ${izenejma_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${XML2_INCLUDE_DIR}
  ${TokyoCabinet_INCLUDE_DIRS}
  ${Glog_INCLUDE_DIRS}
  )

# sequences is important for some linker
#if a depends b, a must precede b
SET(libs
  sf1r_bundle_index
  sf1r_bundle_mining

  sf1r_search_manager
  sf1r_mining_manager
  sf1r_document_manager
  sf1r_search_manager
  sf1r_ranking_manager
  sf1r_query_manager
  sf1r_la_manager
  sf1r_configuration_manager
  sf1r_index_manager
  sf1r_directory_manager
  sf1r_common
  sf1r_aggregator_manager
  sf1r_log_manager
  sf1r_node_manager
  sf1r_net

  ${idmlib_LIBRARIES}
  ${ilplib_LIBRARIES}
  ${izenecma_LIBRARIES}
  ${izenejma_LIBRARIES}
  ${izenelib_LIBRARIES}

  #external
  ${OpenSSL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${TokyoCabinet_LIBRARIES}
  ${Glog_LIBRARIES}
  ${XML2_LIBRARIES}
  ${SQLITE3_LIBRARIES}
  ${MYSQL_LIBRARIES}
  ${LibCURL_LIBRARIES}
  )
SET(Boost_USE_STATIC_LIBS OFF)
FIND_PACKAGE(Boost ${Boost_FIND_VERSION}
  COMPONENTS unit_test_framework)

IF(Boost_FOUND AND Boost_UNIT_TEST_FRAMEWORK_FOUND)
  INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
  )
  ADD_EXECUTABLE(t_search_manager
    Runner.cpp
    t_Sorter.cpp
    t_AndDocumentIterator.cpp
    t_OrDocumentIterator.cpp
    t_PhraseDocumentIterator.cpp
    t_ExactPhraseDocumentIterator.cpp
    t_MockIndexReader.cpp
    t_NearbyPhraseDocumentIterator.cpp
    t_OrderedPhraseDocumentIterator.cpp
    t_FilterDocumentIterator.cpp
    t_AllDocumentIterator.cpp
    t_CustomRanker.cpp
    t_dump_index.cpp
    ${CMAKE_SOURCE_DIR}/process/common/XmlConfigParser.cpp
    ${CMAKE_SOURCE_DIR}/process/common/CollectionMeta.cpp
    ${CMAKE_SOURCE_DIR}/process/common/XmlSchema.cpp
  )
  TARGET_LINK_LIBRARIES(t_search_manager
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${libs}
  )
  SET_TARGET_PROPERTIES(t_search_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SF1RENGINE_ROOT}/testbin
    )
  ADD_TEST(search_manager "${SF1RENGINE_ROOT}/testbin/t_search_manager")

ENDIF(Boost_FOUND AND Boost_UNIT_TEST_FRAMEWORK_FOUND)
