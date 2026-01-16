# Paho MQTT C and C++ Libraries Configuration

set(PAHO_INSTALL_DIR "${CMAKE_SOURCE_DIR}/external/paho_install")

# Paho C (Async)
add_library(paho-mqtt3a STATIC IMPORTED GLOBAL)
set_target_properties(paho-mqtt3a PROPERTIES
    IMPORTED_LOCATION "${PAHO_INSTALL_DIR}/lib64/libpaho-mqtt3a.a"
    INTERFACE_INCLUDE_DIRECTORIES "${PAHO_INSTALL_DIR}/include"
)

# Paho C++
add_library(paho-mqttpp3 STATIC IMPORTED GLOBAL)
set_target_properties(paho-mqttpp3 PROPERTIES
    IMPORTED_LOCATION "${PAHO_INSTALL_DIR}/usr/local/lib64/libpaho-mqttpp3.a"
    INTERFACE_INCLUDE_DIRECTORIES "${PAHO_INSTALL_DIR}/usr/local/include"
    INTERFACE_LINK_LIBRARIES paho-mqtt3a
)