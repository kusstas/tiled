# Initialize the version
isEmpty(TILED_VERSION):TILED_VERSION = "1.4.1"

# See the README file for instructions about setting the install prefix.
isEmpty(PREFIX):PREFIX = /usr/local
isEmpty(LIBDIR):LIBDIR = $${PREFIX}/lib
isEmpty(RPATH):RPATH = yes
isEmpty(INSTALL_HEADERS):INSTALL_HEADERS = no
isEmpty(SYSTEM_ZSTD):SYSTEM_ZSTD = no

macx {
    # Do a universal build when possible
    contains(QT_CONFIG, ppc):CONFIG += x86 ppc
}

CONFIG += depend_includepath c++14 tiled_quick

!isEmpty(USE_FHS_PLUGIN_PATH) {
    DEFINES += TILED_PLUGIN_DIR=\\\"$${LIBDIR}/tiled/plugins/\\\"
}

tiled_zstd {
    LIBS += -lzstd
    DEFINES += TILED_ZSTD_SUPPORT
}

# Taken from Qt Creator project files
defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}
