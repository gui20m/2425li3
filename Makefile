CC = gcc
LDFLAGS = $(shell pkg-config --libs glib-2.0) -lncurses
CFLAGS = -c -g -O3 -Iinclude -Wall -I$(SRC)/libs $(shell pkg-config --cflags glib-2.0)

SRC = src/Main
LIBS = src/Main
LIBS_E = src/Enteties
LIBS_S = src/Statistics
LIBS_I = src/Interactive
LIBS_T = src/Test

LIBS_H = include/Main
LIBS_H_E = include/Enteties
LIBS_H_S = include/Statistics
LIBS_H_I = include/Interactive
LIBS_H_T = include/Test

OUTPUT = programa-principal
OUTPUT_TEST = programa-testes
OUTPUT_INTERACTIVE = programa-interativo
DATASETS = datasets
OUTPUT_RESULTS = resultados

LIBS_OBJS = $(LIBS_I)/querysolver.o $(LIBS_I)/info.o $(LIBS_I)/home.o $(LIBS_I)/interactive.o $(SRC)/main.o \
            $(LIBS)/parser.o $(LIBS)/interpreter.o $(LIBS)/utils.o $(LIBS_E)/users.o $(LIBS_E)/history.o \
            $(LIBS)/queries.o $(LIBS_E)/spotilixo.o $(LIBS)/output.o $(LIBS_E)/artists.o $(LIBS_E)/albums.o \
            $(LIBS_E)/musics.o $(LIBS)/dataval.o $(LIBS_T)/test.o $(LIBS_T)/compare_files.o $(LIBS_E)/artists_t.o \
            $(LIBS_E)/albums_t.o $(LIBS_E)/musics_t.o $(LIBS_E)/users_t.o $(LIBS_E)/history_t.o $(LIBS_S)/agedata.o \
            $(LIBS_S)/individualalbums.o $(LIBS_S)/views.o $(LIBS_S)/statistics.o $(LIBS_S)/topartists.o \
            $(LIBS_S)/leaderboard.o $(LIBS_S)/annualsummary.o $(LIBS_S)/algorithm.o $(LIBS_S)/recomendador.o

all: dirs $(OUTPUT) $(OUTPUT_TEST) $(OUTPUT_INTERACTIVE)

$(OUTPUT): $(LIBS_OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "[Compiling] $<"

$(OUTPUT_TEST): $(LIBS_OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "[Compiling] $<"

$(OUTPUT_INTERACTIVE): $(LIBS_OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@
	@echo "[Compiling] $<"

trash/$(SRC)/%.o: $(SRC)/%.c $(LIBS_H)/%.h
	@$(CC) $(CFLAGS) $< -o $@
	@echo "[Linking] $<"

$(LIBS)/%.o: $(LIBS)/%.c $(LIBS_H)/%.h
	@$(CC) $(CFLAGS) $< -o $@
	@echo "[Linking] $<"

$(LIBS_S)/%.o: $(LIBS_S)/%.c $(LIBS_H_S)/%.h
	@$(CC) $(CFLAGS) $< -o $@
	@echo "[Linking] $<"

$(LIBS_E)/%.o: $(LIBS_E)/%.c $(LIBS_H_E)/%.h
	@$(CC) $(CFLAGS) $< -o $@
	@echo "[Linking] $<"

$(LIBS_I)/%.o: $(LIBS_I)/%.c $(LIBS_H_I)/%.h
	@$(CC) $(CFLAGS) $< -o $@
	@echo "[Linking] $<"

clean:
	@rm -rf $(OUTPUT)
	@rm -rf $(OUTPUT_RESULTS)/*.txt
	@rm -rf $(OUTPUT_RESULTS)/*.csv
	@find $(LIBS_S) -type f -name '*.o' ! -name 'recomendador.o' -exec rm -f {} +
	@rm -rf $(LIBS_E)/*.o
	@rm -rf src/*.o
	@rm -rf $(SRC)/*.o
	@rm -rf $(LIBS_T)/*.o
	@rm -rf $(OUTPUT_TEST)
	@rm -rf $(OUTPUT_INTERACTIVE)
	@rm -rf $(LIBS_I)/*.o
	@echo "[Clean] Cleaning directory"

dirs:
	@mkdir -p $(SRC) $(DATASETS) $(LIBS)
