# Пути и переменные
TRANSLATIONS := assets/translations

# Основные цели
.PHONY: all
all: release

# release сборка
.PHONY: release
release:
	@cmake -S . -B build
	@cmake --build build --config Release

# dev сборка и тесты
.PHONY: dev
dev:
	@cmake --preset=$@
	@cmake --build --preset=$@
	@ctest --preset=$@

# Удаление сгенерированных файлов
.PHONY: clean
clean:
	@find . -name "build" -type d -exec rm -rf {} +
	@find . -name "*.user" -type f -delete

# Генерация документации
.PHONY: docs
docs:
	@cmake "-DPROJECT_SOURCE_DIR=$(shell pwd)" "-DPROJECT_BINARY_DIR=$(shell pwd)/build" -P cmake/docs-ci.cmake

# Приведение кодовой базу к единому стилю
.PHONY: clang-format
clang-format:	
	@cmake -D FORMAT_COMMAND=clang-format-18 -D FIX=YES -P cmake/lint.cmake

# Обновление существующих файлов переводов
.PHONY: update_translations
update_translations:
	@lupdate . -locations none -ts $(TRANSLATIONS)/*.ts