
GEN=../../bin/gen

default: MaterialIcons-Regular.h Roboto-Light.h Roboto-Light-12.h Roboto-Light-18.h
	ls -la

Roboto-Light.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -nroboto::light -o $@ $<

MaterialIcons-Regular.h: MaterialIcons-Regular.ttf $(GEN)
	$(GEN) -wH -xF -iU -nmaterialicons::regular -o $@ $<

Roboto-Light-12.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -nroboto::light12 -p 12 -o $@ $<

Roboto-Light-18.h: Roboto-Light.ttf $(GEN)
	$(GEN) -wH -iF -xU -nroboto::light18 -p 18 -o $@ $<
