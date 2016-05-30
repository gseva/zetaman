# Makefile de ejemplo para programa cliente-servidor en C/C++
# Creado: 27/04/2007 - Leandro Lucarella
# Modificado: 23/04/2014 - Pablo Roca
# Copyleft 2007 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]

# CONFIGURACION
################

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp
header_extension = h

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

# Si se usa box2d
box2d = si



# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ para tratamiento de errores y warnings.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors -ansi

# Para optimizar el binario resultante lo mejor posible
#CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
# CSTD = c99

# Estandar de C++ a usar
# CXXSTD = c++11
CXXSTD = gnu++11

# Si se quiere compilar estáticamente, descomentar la siguiente línea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
GTKCFLAGS += $(shell pkg-config --cflags gtk+-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED
GTKLDFLAGS += $(shell pkg-config --libs gtk+-3.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
GTKMMCFLAGS = $(shell pkg-config --cflags gtkmm-3.0) \
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED	\
	-DGDKMM_DISABLE_DEPRECATED 	\
	-DGTKMM_DISABLE_DEPRECATED
GTKMMLDFLAGS = $(shell pkg-config --libs gtkmm-3.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
PTHREAD_FLAGS = -lpthread -pthread
endif

ifdef box2d
BOX2D_FLAGS = -lBox2D
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

SRV_CFLAGS = $(CXXFLAGS)
SRV_LDFLAGS = $(LDFLAGS) $(BOX2D_FLAGS) $(PTHREAD_FLAGS)
CL_CFLAGS = $(CXXFLAGS) $(GTKMMCFLAGS)
CL_LDFLAGS = $(LDFLAGS) $(GTKMMLDFLAGS) $(PTHREAD_FLAGS)
EDITOR_CFLAGS = $(CXXFLAGS) $(GTKMMCFLAGS)
EDITOR_LDLAGS = $(LDFLAGS) $(GTKMMLDFLAGS)

GLIB_COMPILE_RESOURCES = glib-compile-resources
compile_res = @$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(dir $<) --generate-source $<
link_res = cc $(shell pkg-config --cflags gtk+-3.0) -c $< -o $@

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
json_dir = $(zm_dir)json/
editor_dir = $(zm_dir)editor/

assets_dir = assets/
images_config = $(assets_dir)image.gresource.xml
editor_config = $(assets_dir)editor.gresource.xml
maps_dir = $(assets_dir)maps/

# Si no especifica archivos, tomo todos.
client_sources ?= $(wildcard $(client_dir)*.$(extension))
server_sources ?= $(wildcard $(server_dir)*.$(extension)) $(wildcard $(server_dir)**/*.$(extension))
common_sources ?= $(wildcard $(zm_dir)*.$(extension))
editor_sources ?= $(wildcard $(editor_dir)*.$(extension))
json_sources ?= $(wildcard $(json_dir)*.$(extension))
all_sources = $(client_sources) $(server_sources) $(common_sources) $(json_sources) $(editor_sources)
all_headers = $(wildcard $(zm_dir)*.$(header_extension) $(zm_dir)**/*.$(header_extension))

resources_dir = $(build_dir)resources/
client_resources = $(resources_dir)client_resources.c
editor_resources = $(resources_dir)editor_resources.c

server_target = $(build_dir)server
client_target = $(build_dir)zm
editor_target = $(build_dir)editor

o_common_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(common_sources))
o_server_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(server_sources))
o_client_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(client_sources))
o_editor_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(editor_sources))
o_json_only_files = $(patsubst %.$(extension),$(obj_dir)/%.o,$(json_sources))
o_server_all_files = $(o_server_only_files) $(o_common_files) $(o_json_only_files)
o_client_all_files = $(o_client_only_files) $(o_common_files) $(o_json_only_files)
o_editor_all_files = $(o_editor_only_files) $(o_json_only_files)

o_all_files =  $(o_client_only_files) $(o_common_files) $(o_server_only_files) $(o_editor_only_files) $(o_json_only_files)

o_client_resources = $(resources_dir)client_resources.o
o_editor_resources = $(resources_dir)editor_resources.o

lint_extensions = --extensions=$(header_extension),$(extension)
lint_filters =  --filter=`cat lint/filter_options`
lint_command = python lint/cpplint.py $(lint_extensions) $(lint_filters)

# REGLAS
#########

.PHONY: all clean lint assets

all: lint client editor

$(o_common_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(CXXFLAGS) -c $< -o $@

$(o_json_only_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(CXXFLAGS) -c $< -o $@

$(o_server_only_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(SRV_CFLAGS) -c $< -o $@

$(o_client_only_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(CL_CFLAGS) -c $< -o $@

$(o_editor_only_files): $(obj_dir)/%.o : %.$(extension)
	$(LD) $(EDITOR_CFLAGS) -c $< -o $@

$(client_resources): $(images_config)
	$(compile_res)

$(editor_resources): $(editor_config)
	$(compile_res)

$(o_client_resources): $(client_resources)
	$(link_res)

$(o_editor_resources): $(editor_resources)
	$(link_res)

create_dirs:
	@$(foreach file, $(o_all_files) $(resources_dir) , mkdir -p $(dir $(file));)
	@mkdir -p $(resources_dir)

client: create_dirs client_assets $(o_client_all_files)
	$(LD) $(o_client_all_files) $(o_client_resources) -o $(client_target) $(CL_LDFLAGS)

server: $(o_server_all_files)
	$(LD) $(o_server_all_files) -o $(server_target) $(SRV_LDFLAGS)

editor: create_dirs editor_assets $(o_editor_all_files)
	$(LD) $(o_editor_all_files) $(o_editor_resources) -o $(editor_target) $(EDITOR_LDFLAGS)

lint:
	$(lint_command) $(all_sources) $(all_headers)

client_assets: $(client_resources) $(o_client_resources)
	@cp -r $(maps_dir) $(build_dir)maps

editor_assets: $(editor_resources) $(o_editor_resources)

clean_assets:
	@$(RM) -fv $(client_resources) $(o_client_resources) $(editor_resources) $(o_editor_resources)

clean: clean_assets
	@$(RM) -fv $(o_client_only_files) $(o_server_only_files) $(o_common_files) \
						 $(o_editor_only_files) $(client_target) $(server_target) $(editor_target)

