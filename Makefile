# Makefile de ejemplo para programa cliente-servidor en C/C++
# Creado: 27/04/2007 - Leandro Lucarella
# Modificado: 23/04/2014 - Pablo Roca
# Copyleft 2007 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]

# CONFIGURACION
################

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Si usa funciones de math.h, descomentar (quitar el '#' a) la siguiente línea.
# math = si

# Si usa pthreads, descomentar (quitar el '#' a) la siguiente línea.
pthreads = si

# Si es un programa GTK+, descomentar (quitar el '#' a) la siguiente línea.
#gtk = si

# Si es un programa gtkmm, descomentar (quitar el '#' a) la siguiente línea.
gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
verbose = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors

# Para optimizar el binario resultante lo mejor posible
CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
# CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++11

# Si se quiere compilar estáticamente, descomentar la siguiente línea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED	\
	-DGDKMM_DISABLE_DEPRECATED 	\
	-DGTKMM_DISABLE_DEPRECATED
LDFLAGS += $(shell pkg-config --libs gtkmm-3.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Agrego zm
CFLAGS += -I.

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif

# Directories
build_dir = build/
obj_dir = $(build_dir)objs

zm_dir = zm/
client_dir = $(zm_dir)client/
server_dir = $(zm_dir)server/

# Si no especifica archivos, tomo todos.
client_sources ?= $(wildcard $(client_dir)*.$(extension))
server_sources ?= $(wildcard $(server_dir)*.$(extension))
common_sources ?= $(wildcard $(zm_dir)*.$(extension))
all_sources = $(client_sources) $(server_sources) $(common_sources)

server_target = $(build_dir)server
client_target = $(build_dir)zm

o_common_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(common_sources))
o_server_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(server_sources))
o_client_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(client_sources))
o_server_all_files = $(o_server_only_files) $(o_common_files)
o_client_all_files = $(o_client_only_files) $(o_common_files) $(o_server_only_files)
o_all_files =  $(o_client_only_files) $(o_common_files) $(o_server_only_files)


# REGLAS
#########

.PHONY: all clean


all: client

$(o_all_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(CXXFLAGS) -c $< -o $@

# $(o_all_files)/%.o: %.(extension)
# 	$(LD) $(CXXFLAGS) -c $< -o $@

# $(o_server_only_files): $(server_sources)
# 	$(LD) $(CXXFLAGS) -c $< -o $@

# $(o_client_only_files): $(client_sources)
# 	$(LD) $(CXXFLAGS) -c $< -o $@

# $(o_common_files): $(obj_dir)/%.o : $(zm_dir)/%.o

# $(o_client_all_files): $(obj_dir)/%.o : $(SOURCEDIR)/%.cpp
#     $(CC) $(FLAGS) $< -o $@

create_dirs:
	@$(foreach file, $(o_client_all_files), mkdir -p $(dir $(file));)

client: create_dirs $(o_client_all_files)
	@if [ -z "$(o_client_all_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente (archivos client*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(o_client_all_files) -o $(client_target) $(LDFLAGS)

server: $(o_server_only_files)
	@if [ -z "$(o_server_only_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor (archivos server*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(o_server_only_files) -o $(server_target) $(LDFLAGS)

clean:
	@$(RM) -fv $(o_client_only_files) $(o_server_only_files) $(o_common_files) \
						 $(client_target) $(server_target)

