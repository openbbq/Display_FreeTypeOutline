
GEN=../../bin/gen

default: MaterialIcons-Regular-18.h MaterialIcons-Outlined-18.h Roboto-Light.h Roboto-Light-9.h Roboto-Light-12.h Roboto-Light-18.h Roboto-Light-24.h
	ls -la

MaterialIcons-Outlined-18.h: MaterialIconsOutlined-Regular.otf $(GEN)
	$(GEN) -wH -xF -iU -n materialicons::outlined18 -p 18 -o $@ $<

MaterialIcons-Regular-18.h: MaterialIcons-Regular.ttf $(GEN)
	$(GEN) -wH -xF -iU -n materialicons::regular18 -p 18 -o $@ $<

Roboto-Light-9.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -n roboto::light9 -p 9 -o $@ $<

Roboto-Light-12.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -n roboto::light12 -p 12 -o $@ $<

Roboto-Light-18.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -n roboto::light18 -p 18 -o $@ $<

Roboto-Light-24.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -n roboto::light24 -p 24 -o $@ $<
