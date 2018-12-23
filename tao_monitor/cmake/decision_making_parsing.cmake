FILE(MAKE_DIRECTORY "/home/juan/robots_ws/devel/share/tao_monitor/graphs" )
FILE(GLOB_RECURSE FOR_DEL /home/juan/robots_ws/devel/share/tao_monitor/graphs/* )
#message("delete files from /home/juan/robots_ws/devel/share/tao_monitor/graphs/ : ${FOR_DEL}")
if( FOR_DEL )
	FILE(REMOVE ${FOR_DEL})
endif()
execute_process(COMMAND /home/juan/robots_ws/devel/lib/decision_making_parser/decision_making_parser -pe -xml -dot -src "/home/juan/robots_ws/src/tao_monitor" -dst "/home/juan/robots_ws/devel/share/tao_monitor/graphs" -f "/home/juan/robots_ws/src/tao_monitor/src/monitor.cpp:" RESULT_VARIABLE rv)
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/juan/robots_ws/devel/share/tao_monitor/graphs/ /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.scxml /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.btxml  /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.taoxml)
message("   -- Created XML files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
FILE(GLOB_RECURSE CREATED_FILES_ABS /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.scxml /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.btxml /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.taoxml)
execute_process(COMMAND "python" /home/juan/robots_ws/devel/lib/decision_making_parser/decision_making_xml_parser.py -i "/home/juan/robots_ws/src/tao_monitor" "/home/juan/robots_ws/devel/share/tao_monitor/graphs" "${CREATED_FILES_ABS}" RESULT_VARIABLE rv)
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/juan/robots_ws/devel/share/tao_monitor/graphs/ /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.dot /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.xot)
message("   -- Created DOT files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
FILE(GLOB_RECURSE CREATED_FILES_ABS /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.dot)
foreach( f ${CREATED_FILES_ABS} )
	execute_process(COMMAND "dot" -q1 -Tgif -o${f}.gif  ${f} RESULT_VARIABLE rv)
endforeach()
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/juan/robots_ws/devel/share/tao_monitor/graphs/ /home/juan/robots_ws/devel/share/tao_monitor/graphs/*.gif)
message("   -- Created GIF files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
