TARGET = test-persistent
QT = core sql

VALGRIND = true

# Needed for the database template
RESOURCES += ../../resources/resources-lite.qrc

HEADERS  +=						\
	../../lib/core/LogEntry.h			\
	../../lib/core/TSettings.h			\
	../../src/persistentmodel/archive.h		\
	../../src/persistentmodel/journal.h		\
	../../src/persistentmodel/persistentobject.h	\
	../../src/persistentmodel/persistentstore.h

SOURCES += test-persistent.cpp				\
	../../lib/core/TSettings.cpp			\
	../../src/persistentmodel/archive.cpp		\
	../../src/persistentmodel/journal.cpp		\
	../../src/persistentmodel/persistentobject.cpp	\
	../../src/persistentmodel/persistentstore.cpp

include(../tests-include.pro)

test_home_prep.commands += ; mkdir -p "$${TEST_HOME}/$${TARGET}";	\
	cp -r confdir/* "$${TEST_HOME}/$${TARGET}"
