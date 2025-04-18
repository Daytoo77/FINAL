OUT = projet
	CXX = g++
	CXXFLAGS = -Wall -std=c++17
	PKG_CONFIG = pkg-config
	GTKMM_INCLUDE = `$(PKG_CONFIG) --cflags gtkmm-4.0`
	GTKMM_LIBS = `$(PKG_CONFIG) --libs gtkmm-4.0`

	# Fichiers source
	PROJET_SRC = projet.cc
	GUI_SRC = gui.cc
	JEU_SRC = jeu.cc
	MOBILE_SRC = mobile.cc
	CHAINE_SRC = chaine.cc
	TOOLS_SRC = tools.cc
	GRAPHIC_SRC = graphic.cc
	MESSAGE_SRC = message.cc

	CXXFILES = $(PROJET_SRC) $(GUI_SRC) $(JEU_SRC) $(MOBILE_SRC) $(CHAINE_SRC) $(TOOLS_SRC) $(GRAPHIC_SRC) $(MESSAGE_SRC)
	OFILES = $(CXXFILES:.cc=.o)

	all: $(OUT)

	clean:
		@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
		@del /Q *.o *.x $(OUT).exe

	# Règles de compilation
	%.o: %.cc
		$(CXX) $(CXXFLAGS) $(GTKMM_INCLUDE) -c $< -o $@

	$(OUT): $(OFILES)
		$(CXX) $(CXXFLAGS) $(OFILES) -o $@ $(GTKMM_LIBS)

	.PHONY: all clean