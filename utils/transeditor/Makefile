CXX=g++
CXXFLAGS=-W -Wall -Werror -O2
INCPATH=-Isrc
LDFLAGS=-llog4cxx

QT_HOME=/home/rid/sources/qt-everywhere-opensource-src-4.8.6
MOC=$(QT_HOME)/bin/moc
RCC=$(QT_HOME)/bin/rcc
INCPATH+=-I$(QT_HOME)/include
LDFLAGS+=-L$(QT_HOME)/lib -lQtCore -lQtGui -lQtNetwork -lQtXml -lQtXmlPatterns

OBJS=src/main.o \
     src/Configuration.o \
     src/controller/xmlparser/XmlConfigHandler.o \
     src/controller/xmlparser/XmlConfigReader.o \
     src/controller/xmlparser/XmlScriptHandler.o \
     src/controller/xmlparser/XmlScriptReader.o \
     src/controller/xmlparser/XmlScriptTools.o \
     src/controller/xmlparser/XmlScriptWriter.o \
     src/controller/xmlparser/XmlValidator.o \
     src/controller/Manager.o \
     src/model/PointedText.o \
     src/model/Script.o \
     src/model/ScriptConfig.o \
     src/util/Logger.o \
     src/view/ControlBar.o \
     src/view/MainWindow.o \
     src/view/ScriptHighlighter.o \
     src/view/TextPanel.o \
     src/view/TranslationTextPanel.o \
     res/qrc_resources.o

MOCOBJS=src/controller/moc_Manager.o \
        src/model/moc_PointedText.o \
        src/model/moc_Script.o \
        src/view/moc_ControlBar.o \
        src/view/moc_MainWindow.o \
        src/view/moc_ScriptHighlighter.o \
        src/view/moc_TextPanel.o \
        src/view/moc_TranslationTextPanel.o

QRCCFG=res/resources.qrc
QRCSRC=res/xsd/config_schema.xsd \
       res/xsd/dump_schema.xsd


# 3.2. Variables internes
# Il existe plusieurs variables internes au Makefile, citons entre autres :
#
# $@ 	Le nom de la cible
# $< 	Le nom de la première dépendance
# $^ 	La liste des dépendances
# $? 	La liste des dépendances plus récentes que la cible
# $*

all: editor

editor:$(OBJS) $(MOCOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

src/main.o: src/main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/Configuration.o: src/Configuration.cpp src/Configuration.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/Manager.o: src/controller/Manager.cpp src/controller/Manager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlConfigHandler.o: src/controller/xmlparser/XmlConfigHandler.cpp src/controller/xmlparser/XmlConfigHandler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlConfigReader.o: src/controller/xmlparser/XmlConfigReader.cpp src/controller/xmlparser/XmlConfigReader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlScriptHandler.o: src/controller/xmlparser/XmlScriptHandler.cpp src/controller/xmlparser/XmlScriptHandler.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlScriptReader.o: src/controller/xmlparser/XmlScriptReader.cpp src/controller/xmlparser/XmlScriptReader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlScriptTools.o: src/controller/xmlparser/XmlScriptTools.cpp src/controller/xmlparser/XmlScriptTools.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlScriptWriter.o: src/controller/xmlparser/XmlScriptWriter.cpp src/controller/xmlparser/XmlScriptWriter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/xmlparser/XmlValidator.o: src/controller/xmlparser/XmlValidator.cpp src/controller/xmlparser/XmlValidator.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/model/PointedText.o: src/model/PointedText.cpp src/model/PointedText.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/model/Script.o: src/model/Script.cpp src/model/Script.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/model/ScriptConfig.o: src/model/ScriptConfig.cpp src/model/ScriptConfig.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/util/Logger.o: src/util/Logger.cpp src/util/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/ControlBar.o: src/view/ControlBar.cpp src/view/ControlBar.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/MainWindow.o: src/view/MainWindow.cpp src/view/MainWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/ScriptHighlighter.o: src/view/ScriptHighlighter.cpp src/view/ScriptHighlighter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/TextPanel.o: src/view/TextPanel.cpp src/view/TextPanel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/TranslationTextPanel.o: src/view/TranslationTextPanel.cpp src/view/TranslationTextPanel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

# MOCOBJS
src/controller/moc_Manager.o: src/controller/moc_Manager.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/model/moc_PointedText.o: src/model/moc_PointedText.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/model/moc_Script.o: src/model/moc_Script.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/moc_ControlBar.o: src/view/moc_ControlBar.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/moc_MainWindow.o: src/view/moc_MainWindow.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/moc_ScriptHighlighter.o: src/view/moc_ScriptHighlighter.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/moc_TextPanel.o: src/view/moc_TextPanel.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/view/moc_TranslationTextPanel.o: src/view/moc_TranslationTextPanel.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

src/controller/moc_Manager.cpp: src/controller/Manager.h
	$(MOC) $(INCPATH) $< -o $@

src/model/moc_PointedText.cpp: src/model/PointedText.h
	$(MOC) $(INCPATH) $< -o $@

src/model/moc_Script.cpp: src/model/Script.h
	$(MOC) $(INCPATH) $< -o $@

src/view/moc_ControlBar.cpp: src/view/ControlBar.h
	$(MOC) $(INCPATH) $< -o $@

src/view/moc_MainWindow.cpp: src/view/MainWindow.h
	$(MOC) $(INCPATH) $< -o $@

src/view/moc_ScriptHighlighter.cpp: src/view/ScriptHighlighter.h
	$(MOC) $(INCPATH) $< -o $@

src/view/moc_TextPanel.cpp: src/view/TextPanel.h
	$(MOC) $(INCPATH) $< -o $@

src/view/moc_TranslationTextPanel.cpp: src/view/TranslationTextPanel.h
	$(MOC) $(INCPATH) $< -o $@

# RCXXOBJ
res/qrc_resources.o: res/qrc_resources.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

res/qrc_resources.cpp: $(QRCCFG) $(QRCSRC)
	$(RCC) -name resources $< -o $@

clean:
	-rm -f $(OBJS) $(MOCOBJS) res/qrc_resources.cpp
