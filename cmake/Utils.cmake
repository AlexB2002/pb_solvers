################################################################################################
# Helper function to parse current linker libs into link directories, libflags and osx frameworks
# Usage:
#   PBSAM_parse_linker_libs(<PBSAM_LINKER_LIBS_var> <directories_var> <libflags_var> <frameworks_var>)
function(PBSAM_parse_linker_libs PBSAM_LINKER_LIBS_variable folders_var flags_var frameworks_var)

  set(__unspec "")
  set(__debug "")
  set(__optimized "")
  set(__framework "")
  set(__varname "__unspec")

  # split libs into debug, optimized, unspecified and frameworks
  foreach(list_elem ${${PBSAM_LINKER_LIBS_variable}})
    if(list_elem STREQUAL "debug")
      set(__varname "__debug")
    elseif(list_elem STREQUAL "optimized")
      set(__varname "__optimized")
    elseif(list_elem MATCHES "^-framework[ \t]+([^ \t].*)")
      list(APPEND __framework -framework ${CMAKE_MATCH_1})
    else()
      list(APPEND ${__varname} ${list_elem})
      set(__varname "__unspec")
    endif()
  endforeach()

  # attach debug or optimized libs to unspecified according to current configuration
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(__libs ${__unspec} ${__debug})
  else()
    set(__libs ${__unspec} ${__optimized})
  endif()

  set(libflags "")
  set(folders "")

  # convert linker libraries list to link flags
  foreach(lib ${__libs})
    if(TARGET ${lib})
      list(APPEND folders $<TARGET_LINKER_FILE_DIR:${lib}>)
      list(APPEND libflags -l${lib})
    elseif(lib MATCHES "^-l.*")
      list(APPEND libflags ${lib})
    elseif(IS_ABSOLUTE ${lib})
      get_filename_component(folder  ${lib} PATH)
      get_filename_component(filename ${lib} NAME)
      string(REGEX REPLACE "\\.[^.]*$" "" filename_without_shortest_ext ${filename})

      string(REGEX MATCH "^lib(.*)" __match ${filename_without_shortest_ext})
      list(APPEND libflags -l${CMAKE_MATCH_1})
      list(APPEND folders    ${folder})
    else()
      message(FATAL_ERROR "Logic error. Need to update cmake script")
    endif()
  endforeach()

  PBSAM_list_unique(libflags folders)

  set(${folders_var} ${folders} PARENT_SCOPE)
  set(${flags_var} ${libflags} PARENT_SCOPE)
  set(${frameworks_var} ${__framework} PARENT_SCOPE)
endfunction()
