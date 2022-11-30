.default=all

meson:
	@[[ -d build ]] || meson setup build
	@meson setup build --reconfigure
	@meson compile -C build

build: meson

clean:
	@rm -rf build
all: meson test
tidy:
	@uncrustify -c ~/repos/c_deps/etc/uncrustify.cfg --replace \
		*/*.h */*.c
	@rm *unc-backup* */*unc-backup* 2>/dev/null||true

install: build
	@meson install -C build

test:
	@meson test -C build --verbose
